// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <padamose/AbstractPersistenceProvider.h>
#include <padamose/AbstractPersistentVersionedBranch.h>
#include <padamose/AbstractVersionedBranch.h>
#include <padamose/VersionedStoreTag.h>

namespace Padamose {

//================================================================//
// AbstractVersionedBranch
//================================================================//

//----------------------------------------------------------------//
AbstractVersionedBranch::AbstractVersionedBranch () :
    mLockCount ( 0 ) {
}

//----------------------------------------------------------------//
/** \brief Asserts that no direct references remain.
*/
AbstractVersionedBranch::~AbstractVersionedBranch () {
    assert ( this->mLockCount == 0 );
}

//----------------------------------------------------------------//
// TODO: doxygen
void AbstractVersionedBranch::begin () {
    this->AbstractVersionedBranch_begin ();
}

//----------------------------------------------------------------//
// TODO: doxygen
void AbstractVersionedBranch::commit () {
    this->AbstractVersionedBranch_commit ();
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

    return this->mLockCount;
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
bool AbstractVersionedBranch::isLocked () const {

    return ( this->mLockCount > 0 );
}

//----------------------------------------------------------------//
// TODO: doxygen
bool AbstractVersionedBranch::isPersistent () const {

    return this->AbstractVersionedBranch_isPersistent ();
}

//----------------------------------------------------------------//
/** \brief If a client supports the join operation (i.e. client is a branch),
    appends the contents of the client to the given branch.
*/
void AbstractVersionedBranch::joinBranch ( AbstractVersionedBranch& branch ) {
    this->begin ();
    this->AbstractVersionedBranch_joinBranch ( branch );
    this->commit ();
}

//----------------------------------------------------------------//
void AbstractVersionedBranch::lock () {
    this->mLockCount++;
    if ( this->mSourceBranch ) {
        this->mSourceBranch->lock ();
    }
}

//----------------------------------------------------------------//
/** \brief Attempts to optimize the branch.

    "Optimizing" a branch means two things: trim any unused versions from the
    top of the branch, and concatenate the longest child branch if the child
    branch extends from the top of the current branch.
 
    Unused versions are simply those equal to or greatet than the "immutable top"
    version. The immutable top can fall below the top version following the
    removal of the top client. In this case, the branch can be truncated.
 
    Following truncation, if there are child branches with their bases at the
    top of the branch, one of those children may be joined to the branch. In
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

    if ( this->isLocked ()) return; // don't allow join if there are any direct references to the current branch. (May be over-cautious.)

    if ( this->mSourceBranch ) {
        this->mSourceBranch->optimize ();
    }

    this->begin ();

    LGN_LOG_SCOPE ( PDM_FILTER_ROOT, INFO, "EphemeralVersionedBranch::optimize ()" );
    
    // use one loop to find the immutable top and to identify a child branch that
    // may be joined to the parent branch.
    
    size_t immutableTop = this->mVersion; // immutable top won't be less than the branch's base version.
    BranchPtr bestJoin = NULL; // a join will be performed if this is non-NULL.
        
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
        
        BranchPtr branch = client->asBranch ();
        
        // if nothing is preventing a join, check to see if we can (and should) select
        // the current client as our new candidate for join. only conder client if dependency
        // is greater or equal to the current immutable top.
        if ( branch && ( branch->getVersionDependency () >= immutableTop )) {

            assert ( !branch->isLocked ());

            // if there are no dependencies, then it's a dead branch, so don't consider it.
            if ( branch->countDependencies ()) {

                // the "join score" is just the length of the branch. if we don't yet have a join
                // candidate, pick the current client. if we already have a candidate, pick the
                // one with the higher join score.
                if (( !bestJoin ) || ( bestJoin->getTopVersion () < branch->getTopVersion ())) {
                    if ( this->isPersistent () == branch->isPersistent ()) {
                        LGN_LOG ( PDM_FILTER_ROOT, INFO, "found a client that can join!" );
                        LGN_LOG ( PDM_FILTER_ROOT, INFO, "bestJoin dependency: %d", ( int )client->getVersionDependency ());
                        bestJoin = branch;
                    }
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
    
    this->commit ();
}

//----------------------------------------------------------------//
// TODO: doxygen
void AbstractVersionedBranch::persistSelf ( AbstractPersistenceProvider& provider ) {
    
    if ( this->isPersistent ()) return;
    
    if ( this->mSourceBranch ) {
        this->mSourceBranch->persistSelf ( provider );
        if ( !this->mSourceBranch->isLocked ()) {
            size_t immutableTop = this->mSourceBranch->findImmutableTop ();
            if ( this->mVersion >= immutableTop ) {
                this->mSourceBranch->truncate ( immutableTop );
                this->joinBranch ( *this->mSourceBranch );
                return;
            }
        }
    }

    shared_ptr < AbstractPersistentVersionedBranch > persist = provider.makePersistentBranch ();
    assert ( &( *persist->getProvider ()) == &provider );
    
    // set the source branch and version manually
    persist->mSourceBranch = this->mSourceBranch;
    persist->mVersion = this->mVersion;
    if ( persist->mSourceBranch ) {
        persist->mSourceBranch->insertClient ( *persist );
    }
    
    // force an update
    persist->AbstractVersionedBranchClient_sourceBranchDidChange ();
    
    shared_ptr < AbstractVersionedBranch > pinThis = this->shared_from_this ();
    this->AbstractVersionedBranch_persist ( persist ); // this should move over all clients and orphan the branch
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

    if ( this->isLocked ()) return;
    this->begin ();
    this->AbstractVersionedBranch_truncate ( topVersion );
    this->commit ();
}

//----------------------------------------------------------------//
void AbstractVersionedBranch::unlock () {

    this->begin ();

    if ( this->mLockCount ) {
    
        this->mLockCount--;
    
        if ( this->mSourceBranch ) {
            this->mSourceBranch->unlock ();
        }
        else if ( this->mLockCount == 0 ) {
            this->optimize ();
        }
    }
    
    this->commit ();
}

//================================================================//
// overrides
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
void AbstractVersionedBranch::AbstractVersionedBranch_begin () {
}

//----------------------------------------------------------------//
// TODO: doxygen
void AbstractVersionedBranch::AbstractVersionedBranch_commit () {
}

//----------------------------------------------------------------//
// TODO: doxygen
void AbstractVersionedBranch::AbstractVersionedBranch_print ( string prefix ) const {

    LGN_LOG ( PDM_FILTER_TREE, INFO,
        "%s[%d-%d]: branch %p (refs: %d)",
        prefix.c_str (),
        ( int )this->mVersion,
        ( int )this->getTopVersion (),
        this,
        ( int )this->mLockCount
    );
}

//----------------------------------------------------------------//
// TODO: doxygen
AbstractVersionedBranch::BranchPtr AbstractVersionedBranch::AbstractVersionedBranchClient_asBranch () {
    return this->shared_from_this ();
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
// TODO: doxygen
void AbstractVersionedBranch::AbstractVersionedBranchClient_print ( string prefix ) const {

    this->AbstractVersionedBranch_print ( prefix );
    
    prefix += "....";
    
    set < AbstractVersionedBranchClient* >::const_iterator clientIt = this->mClients.cbegin ();
    for ( ; clientIt != this->mClients.cend (); ++clientIt ) {
        AbstractVersionedBranchClient* client = *clientIt;
        client->AbstractVersionedBranchClient_print ( prefix );
    }
}

} // namespace Padamose
