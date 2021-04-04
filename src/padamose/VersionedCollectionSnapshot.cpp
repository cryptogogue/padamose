// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <padamose/VersionedCollectionSnapshot.h>

namespace Padamose {

//================================================================//
// VersionedStoreTag
//================================================================//

//----------------------------------------------------------------//
/** \brief  Intitialize the snapshot with an existing collection.

    \param      collection      The existing collection.
*/
VersionedCollectionSnapshot::VersionedCollectionSnapshot ( const AbstractVersionedCollection& collection ) :
    mSnapshot ( collection ) {
    
    this->setName ( collection.getName ());
    this->loadState ();
}

//----------------------------------------------------------------//
/** \brief  Load the state of the collection for the given name. Throws
            a VersionedCollectionNotFoundException if the collection state
            cannot be loaded.

    \param      store       The versioned store that contains the collection.
    \param      name        The name of the collection.
 
    \throws     VersionedCollectionNotFoundException    No valid state exists for the collection.
*/
VersionedCollectionSnapshot::VersionedCollectionSnapshot ( const VersionedStoreTag& snapshot, string name ) :
    mSnapshot ( snapshot ) {

    if ( !this->mSnapshot.hasValue ( name )) throw VersionedCollectionNotFoundException ();

    this->setName ( name );
    this->loadState ();
}

//----------------------------------------------------------------//
VersionedCollectionSnapshot::~VersionedCollectionSnapshot () {
}

//================================================================//
// overrides
//================================================================//

//----------------------------------------------------------------//
/** \brief  Implementation of the pure virtual base method to return
            the versioned snapshot associated with the container.

    \return The VersionedStoreTag.
*/
const VersionedStoreTag& VersionedCollectionSnapshot::AbstractVersionedCollection_getSnapshot () const {

    return this->mSnapshot;
}

} // namespace Padamose
