// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <padamose/AbstractPersistenceProvider.h>
#include <padamose/AbstractVersionedBranch.h>
#include <padamose/AbstractVersionedBranchClient.h>

namespace Padamose {

//================================================================//
// AbstractVersionedBranchClient
//================================================================//

//----------------------------------------------------------------//
AbstractVersionedBranchClient::AbstractVersionedBranchClient () {
}

//----------------------------------------------------------------//
AbstractVersionedBranchClient::~AbstractVersionedBranchClient () {
    if ( this->mSourceBranch ) {
        this->mSourceBranch->eraseClient ( *this );
    }
}

//----------------------------------------------------------------//
// TODO: doxygen
void AbstractVersionedBranchClient::clear () {
    this->setParent ( NULL, 0 );
}

//----------------------------------------------------------------//
// TODO: doxygen
size_t AbstractVersionedBranchClient::countBranches () const {

    size_t count = 1;
    shared_ptr < AbstractVersionedBranch > cursor = this->mSourceBranch;
    for ( ; cursor; cursor = cursor->mSourceBranch ) {
        count++;
    }
    return count;
}

//----------------------------------------------------------------//
// TODO: doxygen
BranchInspector AbstractVersionedBranchClient::getBranchInspector ( size_t depth ) {

    shared_ptr < AbstractVersionedBranch > cursor = this->mSourceBranch;
    for ( size_t i = 0; cursor && ( i < depth ); ++i ) {
        cursor = cursor ? cursor->mSourceBranch : NULL;
    }
    return BranchInspector ( cursor );
}

//----------------------------------------------------------------//
/** \brief Return the version depended on in the branch by this snapshot.
    This version must not be altered by any other snapshot.

    All values are held in a branch. The "dependent version" is the
    version below which all layers must be unique to the snapshot. If any other
    snapshot needs to alter any layer below the depdent layer, a new branch
    must be created to hold the changes. The new branch will contain a
    full copy of the layer being changed.
 
    For snapshots, since they reference the "current version" where
    values will be written, the "dependent version" is mVersion + 1.
 
    \return             The dependent version.
*/
size_t AbstractVersionedBranchClient::getVersionDependency () const {
    return this->AbstractVersionedBranchClient_getVersionDependency ();
}

//----------------------------------------------------------------//
// TODO: doxygen
void AbstractVersionedBranchClient::printTree ( string lgnFilter ) const {

    LGN_LOG ( lgnFilter.c_str (), INFO, "PRINT TREE" );

    const AbstractVersionedBranchClient* base = this;
    while ( base->mSourceBranch ) {
        base = base->mSourceBranch.get ();
    }
    base->AbstractVersionedBranchClient_print ( lgnFilter, "" );
}

//----------------------------------------------------------------//
/** \brief Set the debug name.
 
    \param  debugName   The debug name.
*/
void AbstractVersionedBranchClient::setDebugName ( string debugName ) {

    this->mDebugName = debugName;
}

//----------------------------------------------------------------//
/** \brief Remove the client from the existing branch (if any) and add
    it to the new branch. Update the client's version.

    Branches internally maintain a set of their clients. This method updates
    the branch client sets correctly. Any version may be specified that
    is greater than the base version of the new branch. This will add a
    dependency on all lesser versions held in the branch.
 
    When the snapshot is removed from its original, the original branch
    will be deleted or optimized. The original branch is only deleted
    if the snapshot was the last reference to it.
 
    \param  branch      The new branch for the snapshot.
    \param  version     The version referenced by the snapshot.
*/
void AbstractVersionedBranchClient::setParent ( shared_ptr < AbstractVersionedBranch > branch, size_t version ) {

    bool didChange = false;
    weak_ptr < AbstractVersionedBranch > prevBranchWeak = this->mSourceBranch;

    if ( this->mSourceBranch != branch ) {
        
        didChange = true;
        
        LGN_LOG_SCOPE ( PDM_FILTER_ROOT, INFO, "VersionedStoreTag::setParent () - changing branch" );
        
        if ( this->mSourceBranch ) {
            this->mSourceBranch->eraseClient ( *this );
        }
        
        this->mSourceBranch = branch;
        branch = NULL;
    }
    
    if ( this->mSourceBranch ) {
        assert ( version >= this->mSourceBranch->mVersion );
        this->mVersion = version;
        this->mSourceBranch->insertClient ( *this );
    }
    else {
        this->mVersion = 0;
    }

    if ( !prevBranchWeak.expired ()) {
        branch = prevBranchWeak.lock ();
        branch->optimize ();
    }
    
    if ( didChange ) {
        this->AbstractVersionedBranchClient_sourceBranchDidChange ();
    }
}

//================================================================//
// overrides
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
size_t AbstractVersionedBranchClient::AbstractVersionedBranchClient_getVersionDependency () const {
    return this->mVersion + 1;
}

//----------------------------------------------------------------//
void AbstractVersionedBranchClient::AbstractVersionedBranchClient_print ( string lgnFilter, string prefix ) const {

    LGN_LOG ( lgnFilter.c_str (), INFO, "%s[%d]: client %p", prefix.c_str (), ( int )this->mVersion, this );
}

//----------------------------------------------------------------//
// TODO: doxygen
void AbstractVersionedBranchClient::AbstractVersionedBranchClient_sourceBranchDidChange () {
}

} // namespace Padamose
