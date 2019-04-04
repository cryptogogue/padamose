// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <padamose/VersionedStoreSnapshot.h>
#include <padamose/AbstractVersionedBranch.h>

namespace Padamose {

//================================================================//
// AbstractVersionedBranch
//================================================================//

//----------------------------------------------------------------//
AbstractVersionedBranch::AbstractVersionedBranch () :
    mDirectReferenceCount ( 0 ) {
}

//----------------------------------------------------------------//
/** \brief Asserts that no direct references remain.
*/
AbstractVersionedBranch::~AbstractVersionedBranch () {

    assert ( this->mDirectReferenceCount == 0 );
}

//----------------------------------------------------------------//
/** \brief Returns the total number of dependencies (i.e. clients).

    \return     Total clients.
*/
size_t AbstractVersionedBranch::countDependencies () const {

    return this->mClients.size ();
}

//----------------------------------------------------------------//
/** \brief Removes the client from the branch's client set.

    \param      client      Client to erase.
*/
void AbstractVersionedBranch::eraseClient ( AbstractVersionedBranchClient& client ) {

    this->mClients.erase ( &client );
}

//----------------------------------------------------------------//
/** \brief Iterates through all clients to find the top immutable version
    in the branch. All versions less than the top must not be changed.
 
    The "immutable top" is the top of versions in the branch considered
    immutable. A version is immutable if a client depends on it.
 
    The "ignore" parameter is provided to handle the case where a
    single client wishes to change the contents of a version. If the
    version only has the client as a dependent, then it is safe to
    change (without creating a new branch).

    \param      ignore      Client to ignore (or NULL).
    \return                 The upper limit of immutable versions.
*/
size_t AbstractVersionedBranch::findImmutableTop ( const AbstractVersionedBranchClient* ignore ) const {

    LGN_LOG_SCOPE ( PDM_FILTER_ROOT, INFO, "EphemeralVersionedBranch::findImmutableTop ()" );

    size_t immutableTop = this->getVersionDependency ();

    set < AbstractVersionedBranchClient* >::const_iterator clientIt = this->mClients.cbegin ();
    for ( ; clientIt != this->mClients.cend (); ++clientIt ) {

        const AbstractVersionedBranchClient* client = *clientIt;
        if ( client != ignore ) {
        
            size_t clientVersion = client->getVersionDependency ();
            
            if ( immutableTop < clientVersion ) {
                immutableTop = clientVersion;
            }
        }
    }
    
    LGN_LOG ( PDM_FILTER_ROOT, INFO, "immutableTop: %d", ( int )immutableTop );
    
    return immutableTop;
}

//----------------------------------------------------------------//
// TODO: doxygen
shared_ptr < AbstractVersionedBranch > AbstractVersionedBranch::fork ( size_t baseVersion ) {

    return this->AbstractVersionedBranch_fork ( baseVersion );
}

//----------------------------------------------------------------//
// TODO: doxygen
size_t AbstractVersionedBranch::getDirectReferenceCount () const {

    return this->mDirectReferenceCount;
}

//----------------------------------------------------------------//
// TODO: doxygen
size_t AbstractVersionedBranch::getTopVersion () const {

    return this->AbstractVersionedBranch_getTopVersion ();
}

//----------------------------------------------------------------//
size_t AbstractVersionedBranch::getValueNextVersion ( string key, size_t version ) const {

    return this->AbstractVersionedBranch_getValueNextVersion ( key, version );
}

//----------------------------------------------------------------//
size_t AbstractVersionedBranch::getValuePrevVersion ( string key, size_t version ) const {

    return this->AbstractVersionedBranch_getValuePrevVersion ( key, version );
}

//----------------------------------------------------------------//
/** \brief Recursively searches the branch to find the value for the key. The most recent version
    equal to or earlier will be returned.

    A pointer to the value or NULL is returned.

    \param      version     Search for this version or the most recent lesser version of the value.
    \param      key         The key.
    \return                 A raw pointer to the value for the key or NULL.
*/
Variant AbstractVersionedBranch::getValueVariant ( size_t version, string key ) const {

    Variant value;

    // start searching at the current branch.
    const AbstractVersionedBranch* branch = this;

    // iterate through parent branches.
    for ( ; branch; branch = branch->mSourceBranch.get ()) {
    
        value = branch->AbstractVersionedBranch_getValueVariant ( version, key );
    
        if ( !value.isNull ()) {
            return value;
        }
        
        // cap the version at the base version before moving to the parent branch.
        // necessary because branches don't always emerge from the top.
        version = branch->mVersion;
    }
    return value;
}

//----------------------------------------------------------------//
// TODO: doxygen
bool AbstractVersionedBranch::getValueVersionExtents ( string key, size_t upperBound, size_t& first, size_t& last ) const {

    return this->AbstractVersionedBranch_getValueVersionExtents ( key, upperBound, first, last );
}

//----------------------------------------------------------------//
/** \brief Recursively searches the branch to see if a the given key exists.

    \param      version     Search this version or the most recent lesser version for the key.
    \param      key         The key.
    \return                 TRUE if the key is found. FALSE if not.
*/
bool AbstractVersionedBranch::hasKey ( size_t version, string key ) const {

    // start searching at the current branch.
    const AbstractVersionedBranch* branch = this;
    
    // iterate through parent branches.
    for ( ; branch; branch = branch->mSourceBranch.get ()) {
    
        // ignore branches above the version we're searching for.
        if ( branch->AbstractVersionedBranch_hasKey ( key, version )) {
            return true;
        }
        
        // cap the version at the base version before moving to the parent branch.
        // necessary because branches don't always emerge from the top.
        version = branch->mVersion;
    }
    return false;
}

//----------------------------------------------------------------//
/** \brief Inserts a client into the branch's client set. Inserting a client
    adds a dependency on a specific layer in the branch.
*/
void AbstractVersionedBranch::insertClient ( AbstractVersionedBranchClient& client ) {

    this->mClients.insert ( &client );
}

//----------------------------------------------------------------//
/** \brief Attempts to optimize the branch.

    "Optimizing" a branch means two things: trim any unused versions from the
    top of the branch, and concatenate the longest child branch if the child
    branches from the top of the current branch.
 
    Unused versions are simply those equal to or greatet than the "immutable top"
    version. The immutable top can fall below the top version following the
    removal of the top client. In this case, the branch can be truncated.
 
    Following truncation, if there are child branches with their bases at the
    top pf the branch, one of those children may be joined to the branch. In
    this case, we select the longest child branch. When the child is joined to
    the parent, all of its clients must also be moved to the parent.
 
    Optimization is performed recursively on any child branch selected to be
    be joined to the parent.
 
    The presence of "direct references" will prevent the join optimization.
    A "direct reference" is a pointer to a branch internal, such as a value.
    As the join operation may invalidate internals, it must be prevented as
    long as direct references exist. This is accomplished using a reference
    count maintained by the VersionedValue object.
*/
void AbstractVersionedBranch::optimize () {

    LGN_LOG_SCOPE ( PDM_FILTER_ROOT, INFO, "EphemeralVersionedBranch::optimize ()" );
    
    // use one loop to find the immutable top and to identify a child branch that
    // may be joined to the parent branch.
    
    size_t immutableTop = this->mVersion; // immutable top won't be less than the branch's base version.
    bool preventJoin = this->preventJoin (); // don't allow join if there are any direct references to the current branch. (May be over-cautious.)
    AbstractVersionedBranchClient* bestJoin = NULL; // a join will be performed if this is non-NULL.
    
    // loop through every client...
    LGN_LOG ( PDM_FILTER_ROOT, INFO, "evaluating clients for possible concatenation..." );
    set < AbstractVersionedBranchClient* >::const_iterator clientIt = this->mClients.cbegin ();
    for ( ; clientIt != this->mClients.cend (); ++clientIt ) {

        AbstractVersionedBranchClient* client = *clientIt;
        LGN_LOG_SCOPE ( PDM_FILTER_ROOT, INFO, "client %p", client );
        
        size_t clientVersion = client->getVersionDependency (); // store the client's version dependency to avoid extra function calls.
        
        // the immutable top is the *highest* client version dependency. update it if and only if
        // a client with a higher depenency is found.
        if ( immutableTop < clientVersion ) {
        
            LGN_LOG ( PDM_FILTER_ROOT, INFO, "found new immutable top: %d -> %d", ( int )immutableTop, ( int )clientVersion );
        
            immutableTop = clientVersion;
            
            // if we've found a more recent client, invalidate any candidate for joining.
            // we can only optimize by joining a child branch located at the top of
            // the branch.
            bestJoin = NULL;
        }
        
        // if nothing is preventing a join, check to see if we can (and should) select
        // the current client as our new candidate for join. only conder client if dependency
        // is greater or equal to the current immutable top.
        if (( !preventJoin ) && client->canJoin () && ( client->getVersionDependency () >= immutableTop )) {

            // a client branch with direct refereces will prevent any join.
            if ( client->preventJoin ()) {
                preventJoin = true; // stop considering join candidates.
                bestJoin = NULL; // clear out any existing join candidate.
            }
            else {
            
                // the "join score" is just the length of the branch. if we don't yet have a join
                // candidate, pick the current client. if we already have a candidate, pick the
                // one with the higher join score.
                if (( !bestJoin ) || ( bestJoin->getJoinScore () < client->getJoinScore ())) {
                    LGN_LOG ( PDM_FILTER_ROOT, INFO, "found a client that can join!" );
                    LGN_LOG ( PDM_FILTER_ROOT, INFO, "bestJoin dependency: %d", ( int )client->getVersionDependency ());
                    bestJoin = client;
                }
            }
        }
    }
    
    LGN_LOG ( PDM_FILTER_ROOT, INFO, "immutableTop: %d", ( int )immutableTop );
    LGN_LOG ( PDM_FILTER_ROOT, INFO, "topVersion: %d", ( int )this->getTopVersion ());
    
    // throw away any versions equal to or greater than the immutable top.
    this->truncate ( immutableTop );
    
    // if we have a join candidate, perform the join.
    if ( bestJoin ) {
        assert ( bestJoin->getVersionDependency () >= immutableTop );
        bestJoin->joinBranch ( *this );
    }
}

//----------------------------------------------------------------//
// TODO: doxygen
void AbstractVersionedBranch::persistSelf ( AbstractPersistenceProvider& provider ) {

    this->persistSource ( provider );
    this->AbstractVersionedBranch_persistSelf ( provider );
}

//----------------------------------------------------------------//
// TODO: doxygen
void AbstractVersionedBranch::setValueVariant ( size_t version, string key, const Variant& value ) {

    this->AbstractVersionedBranch_setValueVariant ( version, key, value );
}

//----------------------------------------------------------------//
// TODO: doxygen
void AbstractVersionedBranch::transferClients ( AbstractVersionedBranch& other ) {

    shared_ptr < AbstractVersionedBranch > pinThis = this->shared_from_this ();

    // copy the clients
    set < AbstractVersionedBranchClient* >::iterator clientIt = this->mClients.begin ();
    for ( ; clientIt != this->mClients.end (); ++clientIt ) {
        AbstractVersionedBranchClient* client = *clientIt;
        other.insertClient ( *client );
        client->mSourceBranch = other.shared_from_this ();
    }
    this->mClients.clear ();
    
    pinThis = NULL;
}

//----------------------------------------------------------------//
// TODO: doxygen
void AbstractVersionedBranch::truncate ( size_t topVersion ) {

    this->AbstractVersionedBranch_truncate ( topVersion );
}

//================================================================//
// overrides
//================================================================//

//----------------------------------------------------------------//
/** \brief Implementation of virtual method. Always returns true.
    \return     Always returns true.
*/
bool AbstractVersionedBranch::AbstractVersionedBranchClient_canJoin () const {
    return true;
}

//----------------------------------------------------------------//
/** \brief Implementation of virtual method. Returns the top version.
    \return     The top version.
*/
size_t AbstractVersionedBranch::AbstractVersionedBranchClient_getJoinScore () const {
    return this->getTopVersion ();
}

//----------------------------------------------------------------//
/** \brief Implementation of virtual method. Returns the base version;
    the branch depends on all versions less than the base version.
 
    \return     The base version.
*/
size_t AbstractVersionedBranch::AbstractVersionedBranchClient_getVersionDependency () const {
    return this->mVersion;
}

//----------------------------------------------------------------//
/** \brief Implementation of virtual method. Prevents a join optimization
    from happening if the branch has any direct references.
 
    \return     True if the branch has any direct references. False otherwise.
*/
bool AbstractVersionedBranch::AbstractVersionedBranchClient_preventJoin () const {
    return ( this->mDirectReferenceCount > 0 );
}

} // namespace Padamose
