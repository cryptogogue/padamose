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
AbstractVersionedBranchClient::AbstractVersionedBranchClient () :
    mVersion ( 0 ) {
}

//----------------------------------------------------------------//
AbstractVersionedBranchClient::~AbstractVersionedBranchClient () {
    if ( this->mSourceBranch ) {
        this->mSourceBranch->eraseClient ( *this );
    }
}

//----------------------------------------------------------------//
/** \brief Returns true if client supports the join operation.

    \return     True if the join operation is supported (i.e. client is a branch). Otherwise false.
*/
bool AbstractVersionedBranchClient::canJoin () const {
    return this->AbstractVersionedBranchClient_canJoin ();
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
/** \brief If a client supports the join operation, returns the
    score to use to select a winner from two valid candidates.

    \return     The join score (i.e. the branch's top version).
*/
size_t AbstractVersionedBranchClient::getJoinScore () const {
    return this->AbstractVersionedBranchClient_getJoinScore ();
}

//----------------------------------------------------------------//
// TODO: doxygen
AbstractVersionedBranchClient::BranchPtr AbstractVersionedBranchClient::getSourceBranch () {

    return this->mSourceBranch;
}

//----------------------------------------------------------------//
// TODO: doxygen
AbstractVersionedBranchClient::ConstBranchPtr AbstractVersionedBranchClient::getSourceBranch () const {

    return this->mSourceBranch;
}

//----------------------------------------------------------------//
// TODO: doxygen
size_t AbstractVersionedBranchClient::getVersion () const {

    return this->mVersion;
}

//----------------------------------------------------------------//
/** \brief Returns the version below which all verions are depended
    on (to not change) by client. If any version below the dependent
    verion is changed, the branch must be forked and the change applied
    to the new branch.

    \return     The version below which no other version should be changed.
*/
size_t AbstractVersionedBranchClient::getVersionDependency () const {
    return this->AbstractVersionedBranchClient_getVersionDependency ();
}

//----------------------------------------------------------------//
/** \brief If a client supports the join operation (i.e. client is a branch),
    appends the contents of the client to the given branch.
*/
void AbstractVersionedBranchClient::joinBranch ( AbstractVersionedBranch& branch ) {
    this->AbstractVersionedBranchClient_joinBranch ( branch );
}

//----------------------------------------------------------------//
/** \brief Used by client to prevent a join from happening. Only considered
    is canJoin() returns true.

    \return     True if join should be prevented.
*/
bool AbstractVersionedBranchClient::preventJoin () const {
    return this->AbstractVersionedBranchClient_preventJoin ();
}

//----------------------------------------------------------------//
// TODO: doxygen
void AbstractVersionedBranchClient::printTree () const {

    LGN_LOG ( PDM_FILTER_TREE, INFO, "PRINT TREE" );

    const AbstractVersionedBranchClient* base = this;
    while ( base->mSourceBranch ) {
        base = base->mSourceBranch.get ();
    }
    base->AbstractVersionedBranchClient_print ( "" );
}

//----------------------------------------------------------------//
/** \brief Remove the client from the existing branch (if any) and add
    it to the new branch without changing the client's version.
 
    \param  branch      The new branch for the snapshot.
*/
void AbstractVersionedBranchClient::setBranch ( shared_ptr < AbstractVersionedBranch > branch ) {
    this->setBranch ( branch, this->mVersion );
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
void AbstractVersionedBranchClient::setBranch ( shared_ptr < AbstractVersionedBranch > branch, size_t version ) {

    bool didChange = false;
    weak_ptr < AbstractVersionedBranch > prevBranchWeak;

    if ( this->mSourceBranch != branch ) {
        
        didChange = true;
        
        LGN_LOG_SCOPE ( PDM_FILTER_ROOT, INFO, "ConstVersionedStoreTag::setBranch () - changing branch" );
        
        if ( this->mSourceBranch ) {
            prevBranchWeak = this->mSourceBranch;
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

//----------------------------------------------------------------//
/** \brief Copy a snapshot.

    This is a relatively low-cost operation. Taking a snapshot will
    add a dependency on the shared branch but won't do anything else
    until the branch is altered.
 
    \param  other   The snapshot to copy.
*/
void AbstractVersionedBranchClient::takeSnapshot ( const AbstractVersionedBranchClient& other ) {
    
    this->setBranch ( other.mSourceBranch, other.mVersion );
}


//================================================================//
// overrides
//================================================================//

//----------------------------------------------------------------//
void AbstractVersionedBranchClient::AbstractVersionedBranchClient_print ( string prefix ) const {

    LGN_LOG ( PDM_FILTER_TREE, INFO, "%s[%d]: client %p", prefix.c_str (), ( int )this->mVersion, this );
}

} // namespace Padamose
