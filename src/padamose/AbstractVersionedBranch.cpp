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
// TODO: doxygen
void AbstractVersionedBranch::optimize () {

    this->AbstractVersionedBranch_optimize ();
}

//----------------------------------------------------------------//
// TODO: doxygen
void AbstractVersionedBranch::setValueVariant ( size_t version, string key, const Variant& value ) {

    this->AbstractVersionedBranch_setValueVariant ( version, key, value );
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
