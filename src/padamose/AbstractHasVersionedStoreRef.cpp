// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <padamose/AbstractVersionedBranch.h>
#include <padamose/AbstractHasVersionedStoreRef.h>
#include <padamose/VersionedStoreRef.h>

namespace Padamose {

//================================================================//
// AbstractHasVersionedStoreRef
//================================================================//

//----------------------------------------------------------------//
AbstractHasVersionedStoreRef::AbstractHasVersionedStoreRef () {
}

//----------------------------------------------------------------//
AbstractHasVersionedStoreRef::~AbstractHasVersionedStoreRef () {
}

//----------------------------------------------------------------//
// TODO: doxygen
VersionedStoreRef& AbstractHasVersionedStoreRef::getRef () {
    return this->AbstractHasVersionedStoreRef_getRef ();
}

//----------------------------------------------------------------//
// TODO: doxygen
const VersionedStoreRef& AbstractHasVersionedStoreRef::getRef () const {
    return this->AbstractHasVersionedStoreRef_getRef ();
}

////----------------------------------------------------------------//
//// TODO: doxygen
//VersionedStoreRef::ConstBranchPtr AbstractHasVersionedStoreRef::getSourceBranch () const {
//    return this->getRef ().mSourceBranch;
//}

//----------------------------------------------------------------//
// TODO: doxygen
VersionedStoreRef::BranchPtr AbstractHasVersionedStoreRef::getSourceBranch () const {
    return this->getRef ().mSourceBranch;
}

//----------------------------------------------------------------//
// TODO: doxygen
size_t AbstractHasVersionedStoreRef::getVersion () const {
    return this->getRef ().mVersion;
}

} // namespace Padamose
