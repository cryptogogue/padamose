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
const VersionedStoreTag& AbstractPersistenceProvider::getTag ( string branchName ) const {

    map < string, VersionedStoreTag >::const_iterator tagIt = this->mTags.find ( branchName );
    assert ( tagIt != this->mTags.end ()); // TODO: throw exception
    return tagIt->second;
}

//----------------------------------------------------------------//
// TODO: doxygen
bool AbstractPersistenceProvider::hasTag ( string branchName ) const {

    map < string, VersionedStoreTag >::const_iterator tagIt = this->mTags.find ( branchName );
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
void AbstractPersistenceProvider::persist ( VersionedStoreTag& tag, string tagName ) {

    VersionedStoreRef::BranchPtr branch = tag.getSourceBranch ();
    if ( !branch ) return;

    try {

        this->begin ();

        VersionedStoreTag& persistedTag = this->mTags [ tagName ];
        persistedTag.takeSnapshot ( tag );
        branch->persistSelf ( *this );
        this->AbstractPersistenceProvider_tagDidChange ( tagName, &persistedTag );
        
        this->commit ();
    }
    catch ( ... ) {

        // TODO: try to recover and gracefully report the error
        exit ( 1 );
    }
}

//----------------------------------------------------------------//
VersionedStoreTag AbstractPersistenceProvider::restore ( string tagName ) {

    map < string, VersionedStoreTag >::const_iterator tagIt = this->mTags.find ( tagName );
    if ( tagIt != this->mTags.end ()) {
        return VersionedStoreTag ( tagIt->second );
    }
    return VersionedStoreTag ();
}

} // namespace Padamose
