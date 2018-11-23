// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <padamose/VersionedCollectionSnapshot.h>

namespace Padamose {

//================================================================//
// VersionedStore
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
VersionedCollectionSnapshot::VersionedCollectionSnapshot ( const VersionedStoreSnapshot& snapshot, string name ) :
    mSnapshot ( snapshot ) {

    if ( !this->mSnapshot.hasKey ( name )) throw VersionedSetNotFoundException ();

    this->setName ( name );
    this->mState = this->mSnapshot.getValue < VersionedCollectionState >( this->mName );
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
