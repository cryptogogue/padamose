// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <padamose/AbstractVersionedBranch.h>
#include <padamose/VersionedStoreLock.h>

namespace Padamose {

//================================================================//
// VersionedStoreLock
//================================================================//

//----------------------------------------------------------------//
VersionedStoreLock::VersionedStoreLock () {
}

//----------------------------------------------------------------//
VersionedStoreLock::VersionedStoreLock ( const VersionedStoreRef& other ) {
    
    this->setBranch ( other );
}

//----------------------------------------------------------------//
VersionedStoreLock::~VersionedStoreLock () {

    this->setBranch ( NULL );
}

//================================================================//
// overrides
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
void VersionedStoreLock::VersionedStoreRef_setBranch ( shared_ptr < AbstractVersionedBranch > branch, size_t version ) {

    if (( this->mSourceBranch == branch ) && ( this->mVersion == version )) return;
    
    if ( this->mSourceBranch != branch ) {
    
        if ( this->mSourceBranch ) {
            this->mSourceBranch->unlock ();
        }
        
        if ( branch ) {
            branch->lock ();
        }
    }
    
    this->mSourceBranch = branch;
    this->mVersion = version;
}

} // namespace Padamose
