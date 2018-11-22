// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <padamose/AbstractVersionedSet.h>

namespace Padamose {

//================================================================//
// VersionedStore
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
AbstractVersionedSet::AbstractVersionedSet () {
}

//----------------------------------------------------------------//
// TODO: doxygen
AbstractVersionedSet::~AbstractVersionedSet () {
}

//----------------------------------------------------------------//
// TODO: doxygen
size_t AbstractVersionedSet::getSize () const {
    return this->mState.mSize;
}

//----------------------------------------------------------------//
string AbstractVersionedSet::getName () const {

    return this->mMapName;
}

//----------------------------------------------------------------//
void AbstractVersionedSet::setName ( string name ) {

    this->mMapName = name;
    this->mNodePrefix = this->mMapName + SET_NODES_POSTFIX;
    this->mValuePrefix = this->mMapName + SET_VALUES_POSTFIX;
}

} // namespace Padamose
