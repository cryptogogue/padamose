// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <padamose/AbstractVersionedCollection.h>

namespace Padamose {

//================================================================//
// VersionedStore
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
AbstractVersionedCollection::AbstractVersionedCollection () {
}

//----------------------------------------------------------------//
// TODO: doxygen
AbstractVersionedCollection::~AbstractVersionedCollection () {
}

//----------------------------------------------------------------//
// TODO: doxygen
size_t AbstractVersionedCollection::getSize () const {
    return this->mState.mSize;
}

//----------------------------------------------------------------//
const VersionedStoreSnapshot& AbstractVersionedCollection::getSnapshot () const {

    return AbstractVersionedCollection_getSnapshot ();
}

//----------------------------------------------------------------//
string AbstractVersionedCollection::getName () const {

    return this->mMapName;
}

//----------------------------------------------------------------//
void AbstractVersionedCollection::setName ( string name ) {

    if ( this->mMapName.find ( ':' ) != string::npos ) throw InvalidMapNameException ();

    this->mMapName = name;
    this->mNodePrefix = this->mMapName + SET_NODES_POSTFIX;
    this->mValuePrefix = this->mMapName + SET_VALUES_POSTFIX;
}

} // namespace Padamose
