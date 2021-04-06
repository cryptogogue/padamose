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
void VersionedStoreLock::lock ( VersionedStoreRef& other ) {
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
//VersionedStoreInspector VersionedStoreLock::seek ( size_t version ) {
//
//    LGN_LOG_SCOPE ( PDM_FILTER_ROOT, INFO, "VersionedStoreLock::seek ( %d )", ( int )version );
//
//    VersionedStoreRef& ref = this->getRef ();
//    VersionedStoreInspector seek ( ref );
//
//    if ( version > seek.mVersion ) throw VersionOutOfBoundsException ();
//
//    if (( seek.mSourceBranch ) && ( version < seek.mVersion )) {
//    
//        shared_ptr < AbstractVersionedBranch > branch = seek.mSourceBranch;
//        
//        size_t top = seek.mVersion;
//        for ( ; branch && ( version < branch->mVersion ); branch = branch->mSourceBranch ) {
//            top = branch->mVersion;
//        }
//        
//        assert ( branch );
//        assert ( branch->mVersion <= version );
//        assert ( version < top );
//        
//        seek.mSourceBranch = branch;
//        seek.mVersion = version;
//    }
//    return seek;
//}

//----------------------------------------------------------------//
void VersionedStoreLock::unlock () {
    this->lock ( NULL, 0 );
}

//----------------------------------------------------------------//
VersionedStoreLock::VersionedStoreLock () {
}

//----------------------------------------------------------------//
VersionedStoreLock::VersionedStoreLock ( VersionedStoreRef& other ) {
    this->lock ( other.getSourceBranch (), other.getVersion ());
}

//----------------------------------------------------------------//
VersionedStoreLock::~VersionedStoreLock () {
    this->unlock ();
}

} // namespace Padamose
