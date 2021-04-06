// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <padamose/AbstractVersionedBranch.h>
#include <padamose/VersionedStoreLock.h>

namespace Padamose {

//================================================================//
// VersionedStoreLock
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
void VersionedStoreLock::lock ( const HasVersionedBranch& other ) {
    this->lock ( other.getSourceBranch (), other.getVersion ());
}

//----------------------------------------------------------------//
// TODO: doxygen
void VersionedStoreLock::lock ( shared_ptr < AbstractVersionedBranch > branch, size_t version ) {

    if ( this->mSourceBranch ) {
        this->mSourceBranch->unlock ();
    }
    
    if ( branch ) {
        branch->lock ();
    }

    this->mSourceBranch = branch;
    this->mVersion = version;
}

//----------------------------------------------------------------//
void VersionedStoreLock::unlock () {
    this->lock ( NULL, 0 );
}

//----------------------------------------------------------------//
VersionedStoreLock::VersionedStoreLock () {
}

//----------------------------------------------------------------//
VersionedStoreLock::VersionedStoreLock ( const HasVersionedBranch& other ) {
    this->lock ( other.getSourceBranch (), other.getVersion ());
}

//----------------------------------------------------------------//
VersionedStoreLock::~VersionedStoreLock () {
    this->unlock ();
}

} // namespace Padamose
