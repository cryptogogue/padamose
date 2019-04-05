// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <padamose/AbstractPersistenceProvider.h>
#include <padamose/AbstractPersistentVersionedBranch.h>
#include <padamose/AbstractVersionedBranch.h>

namespace Padamose {

//================================================================//
// AbstractPersistenceProvider
//================================================================//

//----------------------------------------------------------------//
AbstractPersistenceProvider::AbstractPersistenceProvider () {
}

//----------------------------------------------------------------//
AbstractPersistenceProvider::~AbstractPersistenceProvider () {

    this->mTags.clear ();
}

//----------------------------------------------------------------//
// TODO: doxygen
const VersionedStoreSnapshot& AbstractPersistenceProvider::getTag ( string branchName ) const {

    map < string, VersionedStoreSnapshot >::const_iterator tagIt = this->mTags.find ( branchName );
    assert ( tagIt != this->mTags.end ()); // TODO: throw exception
    return tagIt->second;
}

//----------------------------------------------------------------//
shared_ptr < AbstractPersistentVersionedBranch > AbstractPersistenceProvider::makePersistentBranch () {

    return this->AbstractPersistenceProvider_makePersistentBranch ();
}

//----------------------------------------------------------------//
void AbstractPersistenceProvider::tagBranch ( AbstractVersionedBranch& branch, string branchName, size_t version ) {

    VersionedStoreSnapshot& tag = this->mTags [ branchName ];
    tag.setBranch ( branch.shared_from_this (), version );
}

} // namespace Padamose
