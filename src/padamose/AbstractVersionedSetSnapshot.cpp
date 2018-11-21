// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <padamose/AbstractVersionedSetSnapshot.h>

namespace Padamose {

//================================================================//
// VersionedStore
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
AbstractVersionedSetSnapshot::AbstractVersionedSetSnapshot () {
}

//----------------------------------------------------------------//
// TODO: doxygen
AbstractVersionedSetSnapshot::~AbstractVersionedSetSnapshot () {
}

//----------------------------------------------------------------//
// TODO: doxygen
size_t AbstractVersionedSetSnapshot::getSize () const {
    return this->mState.mSize;
}

//----------------------------------------------------------------//
void AbstractVersionedSetSnapshot::setName ( string name ) {

    this->mMapName = name;
    this->mNodePrefix = this->mMapName + SET_NODES_POSTFIX;
    this->mValuePrefix = this->mMapName + SET_VALUES_POSTFIX;
}

} // namespace Padamose
