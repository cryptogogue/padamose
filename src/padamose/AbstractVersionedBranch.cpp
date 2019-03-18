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
size_t AbstractVersionedBranch::getTopVersion () const {

    return this->AbstractVersionedBranch_getTopVersion ();
}

//----------------------------------------------------------------//
// TODO: doxygen
bool AbstractVersionedBranch::hasKey ( size_t version, string key ) const {

    return this->AbstractVersionedBranch_hasKey ( version, key );
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

////----------------------------------------------------------------//
///** \brief Implementation of virtual method. Appends the contents of
//    the branch onto the given branch and transfers all clients and
//    children to the given branch.
// 
//    The branch is optimized before being appended. Optimization may
//    recursively trigger additional joins.
// 
//    Neither branch is permitted to have direct references.
// 
//    \param      branch      The branch to be appended to.
//*/
//void AbstractVersionedBranch::AbstractVersionedBranchClient_joinBranch ( AbstractVersionedBranch& branch ) {
//
//    assert ( branch.mDirectReferenceCount == 0 );
//    assert ( this->mDirectReferenceCount == 0 );
//
//    LGN_LOG_SCOPE ( PDM_FILTER_ROOT, INFO, "AbstractVersionedBranch::AbstractVersionedBranchClient_joinBranch ()" );
//    LGN_LOG ( PDM_FILTER_ROOT, INFO, "JOINING PARENT BRANCH" );
//    
//    this->optimize ();
//    
//    shared_ptr < AbstractVersionedBranch > pinThis = this->shared_from_this ();
//    
//    // merge the branch layers
//    branch.mLayers.insert ( this->mLayers.begin(), this->mLayers.end ());
//
//    // copy the value stacks
//    map < string, unique_ptr < AbstractValueStack >>::iterator valueStackIt = this->mValueStacksByKey.begin ();
//    for ( ; valueStackIt != this->mValueStacksByKey.end (); ++valueStackIt ) {
//        
//        const AbstractValueStack* fromStack = this->findValueStack ( valueStackIt->first );
//        assert ( fromStack );
//        
//        unique_ptr < AbstractValueStack >& toStack = branch.mValueStacksByKey [ valueStackIt->first ];
//        if ( !toStack ) {
//            toStack = fromStack->makeEmptyCopy ();
//        }
//        fromStack->join ( *toStack );
//    }
//
//    // copy the clients
//    set < AbstractVersionedBranchClient* >::iterator clientIt = this->mClients.begin ();
//    for ( ; clientIt != this->mClients.end (); ++clientIt ) {
//        AbstractVersionedBranchClient* client = *clientIt;
//        branch.insertClient ( *client );
//        client->mSourceBranch = branch.shared_from_this ();
//    }
//    
//    pinThis = NULL;
//}

//----------------------------------------------------------------//
/** \brief Implementation of virtual method. Prevents a join optimization
    from happening if the branch has any direct references.
 
    \return     True if the branch has any direct references. False otherwise.
*/
bool AbstractVersionedBranch::AbstractVersionedBranchClient_preventJoin () const {
    return ( this->mDirectReferenceCount > 0 );
}

} // namespace Padamose
