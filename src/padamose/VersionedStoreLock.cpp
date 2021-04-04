// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <padamose/AbstractVersionedBranch.h>
#include <padamose/VersionedStoreLock.h>

namespace Padamose {

//================================================================//
// VersionedStoreLock
//================================================================//

//----------------------------------------------------------------//
void VersionedStoreLock::lock ( const AbstractVersionedBranchClient& other ) {

    if (( this->mSourceBranch == other.mSourceBranch ) && ( this->mVersion == other.mVersion )) return;
    
    this->unlock ();
    
    this->setBranch ( other.mSourceBranch, other.mVersion );
    
    if ( this->mSourceBranch ) {
        this->mSourceBranch->lock ();
    }
}

//----------------------------------------------------------------//
void VersionedStoreLock::unlock () {

    if ( this->mSourceBranch ) {
        shared_ptr < AbstractVersionedBranch > sourceBranch = this->mSourceBranch;
        this->setBranch ( NULL );
        sourceBranch->unlock ();
    }
}

//----------------------------------------------------------------//
VersionedStoreLock::VersionedStoreLock () {
}

//----------------------------------------------------------------//
VersionedStoreLock::VersionedStoreLock ( const AbstractVersionedBranchClient& other ) {
    
    this->lock ( other );
}

//----------------------------------------------------------------//
VersionedStoreLock::~VersionedStoreLock () {

    this->unlock ();
}

//================================================================//
// overrides
//================================================================//

//----------------------------------------------------------------//
void VersionedStoreLock::AbstractVersionedBranchClient_print ( string prefix ) const {

    LGN_LOG ( PDM_FILTER_TREE, INFO, "%s[%d]: lock %p", prefix.c_str (), ( int )this->mVersion, this );
}

} // namespace Padamose
