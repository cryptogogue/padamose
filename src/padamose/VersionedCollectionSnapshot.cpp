// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <padamose/VersionedCollectionSnapshot.h>

namespace Padamose {

//================================================================//
// VersionedStore
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
VersionedCollectionSnapshot::VersionedCollectionSnapshot ( const VersionedStoreSnapshot& snapshot, string mapName ) :
    mSnapshot ( snapshot ) {

    if ( !this->mSnapshot.hasKey ( mapName )) throw VersionedSetNotFoundException ();

    this->setName ( mapName );
    this->mState = this->mSnapshot.getValue < VersionedCollectionState >( this->mMapName );
}

//----------------------------------------------------------------//
// TODO: doxygen
VersionedCollectionSnapshot::~VersionedCollectionSnapshot () {
}

//================================================================//
// overrides
//================================================================//

//----------------------------------------------------------------//
const VersionedStoreSnapshot& VersionedCollectionSnapshot::AbstractVersionedCollection_getSnapshot () const {

    return this->mSnapshot;
}

} // namespace Padamose
