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

    this->unlock ();

    this->mSourceBranch = other.getSourceBranch ();
    this->mVersion = other.getVersion ();

    if ( this->mSourceBranch ) {
        this->mSourceBranch->lock ();
    }
}

//----------------------------------------------------------------//
void VersionedStoreLock::unlock () {
    
    if ( this->mSourceBranch ) {
        this->mSourceBranch->unlock ();
        this->mSourceBranch->optimize ();
        this->mSourceBranch = NULL;
    }
}

//----------------------------------------------------------------//
VersionedStoreLock::VersionedStoreLock () {
}

//----------------------------------------------------------------//
VersionedStoreLock::VersionedStoreLock ( const HasVersionedBranch& other ) {
    this->lock ( other );
}

//----------------------------------------------------------------//
VersionedStoreLock::~VersionedStoreLock () {
    this->unlock ();
}

} // namespace Padamose
