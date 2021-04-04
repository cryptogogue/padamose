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
AbstractPersistentVersionedBranch::ConstProviderPtr AbstractPersistentVersionedBranch::getProvider () const {

    return this->AbstractPersistentVersionedBranch_getProvider ();
}

//----------------------------------------------------------------//
// TODO: doxygen
bool AbstractPersistentVersionedBranch::isFrozen () const {

    ConstProviderPtr provider = this->getProvider ();
    assert ( provider );
    return provider->isFrozen ();
}

} // namespace Padamose
