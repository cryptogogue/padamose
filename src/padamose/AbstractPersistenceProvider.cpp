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
AbstractPersistenceProvider::AbstractPersistenceProvider () :
    mIsFrozen ( false ) {
}

//----------------------------------------------------------------//
AbstractPersistenceProvider::~AbstractPersistenceProvider () {

    this->mTags.clear ();
}

//----------------------------------------------------------------//
void AbstractPersistenceProvider::begin () {

    this->AbstractPersistenceProvider_begin ();
}

//----------------------------------------------------------------//
void AbstractPersistenceProvider::commit () {

    this->AbstractPersistenceProvider_commit ();
}

//----------------------------------------------------------------//
// TODO: doxygen
void AbstractPersistenceProvider::flush () {

    this->AbstractPersistenceProvider_flush ();
}

//----------------------------------------------------------------//
// TODO: doxygen
void AbstractPersistenceProvider::freeze () {

    this->mIsFrozen = true;
}

//----------------------------------------------------------------//
// TODO: doxygen
const VersionedStoreSnapshot& AbstractPersistenceProvider::getTag ( string branchName ) const {

    map < string, VersionedStoreSnapshot >::const_iterator tagIt = this->mTags.find ( branchName );
    assert ( tagIt != this->mTags.end ()); // TODO: throw exception
    return tagIt->second;
}

//----------------------------------------------------------------//
// TODO: doxygen
bool AbstractPersistenceProvider::hasTag ( string branchName ) const {

    map < string, VersionedStoreSnapshot >::const_iterator tagIt = this->mTags.find ( branchName );
    return ( tagIt != this->mTags.end ());
}

//----------------------------------------------------------------//
// TODO: doxygen
bool AbstractPersistenceProvider::isFrozen () const {

    return this->mIsFrozen;
}

//----------------------------------------------------------------//
shared_ptr < AbstractPersistentVersionedBranch > AbstractPersistenceProvider::makePersistentBranch () {

    return this->AbstractPersistenceProvider_makePersistentBranch ();
}

//----------------------------------------------------------------//
void AbstractPersistenceProvider::tagBranch ( AbstractVersionedBranch& branch, string branchName, size_t version ) {

    VersionedStoreSnapshot& tag = this->mTags [ branchName ];
    tag.setBranch ( branch.shared_from_this (), version );
    this->AbstractPersistenceProvider_tagDidChange ( branchName, &tag );
}

} // namespace Padamose
