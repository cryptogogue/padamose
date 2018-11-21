// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <padamose/VersionedSetSnapshot.h>

namespace Padamose {

//================================================================//
// VersionedStore
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
VersionedSetSnapshot::VersionedSetSnapshot ( const VersionedStoreSnapshot& snapshot, string mapName ) :
    mSnapshot ( snapshot ) {

    if ( !this->mSnapshot.hasKey ( mapName )) throw VersionedSetNotFoundException ();

    this->setName ( mapName );
    this->mState = this->mSnapshot.getValue < VersionedSetState >( this->mMapName );
}

//----------------------------------------------------------------//
// TODO: doxygen
VersionedSetSnapshot::~VersionedSetSnapshot () {
}

//================================================================//
// overrides
//================================================================//

//----------------------------------------------------------------//
const VersionedStoreSnapshot& VersionedSetSnapshot::AbstractVersionedSetSnapshot_getSnapshot () const {

    return this->mSnapshot;
}

} // namespace Padamose
