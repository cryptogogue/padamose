// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <padamose/AbstractPersistenceProvider.h>
#include <padamose/AbstractPersistentVersionedBranch.h>

namespace Padamose {

//================================================================//
// AbstractPersistentVersionedBranch
//================================================================//

//----------------------------------------------------------------//
AbstractPersistentVersionedBranch::AbstractPersistentVersionedBranch () {
}

//----------------------------------------------------------------//
AbstractPersistentVersionedBranch::~AbstractPersistentVersionedBranch () {
}

//----------------------------------------------------------------//
const AbstractPersistenceProvider* AbstractPersistentVersionedBranch::getProvider () const {

    return this->AbstractPersistentVersionedBranch_getProvider ();
}

//----------------------------------------------------------------//
// TODO: doxygen
bool AbstractPersistentVersionedBranch::isFrozen () const {

    const AbstractPersistenceProvider* provider = this->getProvider ();
    assert ( provider );
    return provider->isFrozen ();
}

} // namespace Padamose
