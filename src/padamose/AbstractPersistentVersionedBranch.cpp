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
void AbstractPersistentVersionedBranch::setProvider ( shared_ptr < AbstractPersistenceProvider > provider ) {

    this->mProvider = provider;
}

} // namespace Padamose
