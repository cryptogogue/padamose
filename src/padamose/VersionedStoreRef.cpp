// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <padamose/VersionedStoreRef.h>

namespace Padamose {

//================================================================//
// VersionedStoreRef
//================================================================//

//----------------------------------------------------------------//
/** \brief Abandons branch and sets version to 0.
*/
void VersionedStoreRef::clear () {
    this->setBranch ( NULL, 0 );
}

//----------------------------------------------------------------//
// TODO: doxygen
VersionedStoreRef::BranchPtr VersionedStoreRef::getSourceBranch () {
    return this->mSourceBranch;
}

//----------------------------------------------------------------//
// TODO: doxygen
VersionedStoreRef::ConstBranchPtr VersionedStoreRef::getSourceBranch () const {
    return this->mSourceBranch;
}

//----------------------------------------------------------------//
// TODO: doxygen
size_t VersionedStoreRef::getVersion () const {
    return this->mVersion;
}

//----------------------------------------------------------------//
void VersionedStoreRef::setBranch ( const VersionedStoreRef& other ) {
    this->setBranch ( other.mSourceBranch, other.mVersion );
}

//----------------------------------------------------------------//
void VersionedStoreRef::setBranch ( shared_ptr < AbstractVersionedBranch > branch ) {
    this->setBranch ( branch, this->mVersion );
}

//----------------------------------------------------------------//
void VersionedStoreRef::setBranch ( shared_ptr < AbstractVersionedBranch > branch, size_t version ) {
    this->VersionedStoreRef_setBranch ( branch, version );
}

//----------------------------------------------------------------//
VersionedStoreRef::VersionedStoreRef () :
    mVersion ( 0 ) {
}

//----------------------------------------------------------------//
VersionedStoreRef::~VersionedStoreRef () {
}

//================================================================//
// overrides
//================================================================//

//----------------------------------------------------------------//
void VersionedStoreRef::VersionedStoreRef_setBranch ( shared_ptr < AbstractVersionedBranch > branch, size_t version ) {

    this->mSourceBranch = branch;
    this->mVersion = version;
}

} // namespace Padamose
