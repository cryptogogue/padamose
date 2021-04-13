// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <padamose/AbstractPersistenceProvider.h>
#include <padamose/AbstractPersistentVersionedBranch.h>
#include <padamose/AbstractVersionedBranch.h>

namespace Padamose {

//================================================================//
// PersistenceTag
//================================================================//

//----------------------------------------------------------------//
string PersistenceTag::getName () const {
    return this->mTagName;
}

//----------------------------------------------------------------//
PersistenceTag::PersistenceTag () :
    mOwner ( NULL ) {
}

//----------------------------------------------------------------//
PersistenceTag::~PersistenceTag () {
}

//================================================================//
// overrides
//================================================================//

//----------------------------------------------------------------//
void PersistenceTag::AbstractVersionedBranchClient_sourceBranchDidChange () {

    if ( !( this->mOwner && this->mSourceBranch && this->mSourceBranch->isPersistent ())) return;

    this->mOwner->AbstractPersistenceProvider_tagDidChange ( *this );
}

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
void AbstractPersistenceProvider::freeze () {

    this->mIsFrozen = true;
}

//----------------------------------------------------------------//
// TODO: doxygen
const VersionedStoreTag& AbstractPersistenceProvider::getTag ( string branchName ) const {

    map < string, PersistenceTag >::const_iterator tagIt = this->mTags.find ( branchName );
    assert ( tagIt != this->mTags.end ()); // TODO: throw exception
    return tagIt->second;
}

//----------------------------------------------------------------//
// TODO: doxygen
bool AbstractPersistenceProvider::hasTag ( string branchName ) const {

    map < string, PersistenceTag >::const_iterator tagIt = this->mTags.find ( branchName );
    return ( tagIt != this->mTags.end ());
}

//----------------------------------------------------------------//
// TODO: doxygen
bool AbstractPersistenceProvider::isFrozen () const {

    return this->mIsFrozen;
}

//----------------------------------------------------------------//
shared_ptr < AbstractPersistentVersionedBranch > AbstractPersistenceProvider::makePersistentBranch ( AbstractVersionedBranch& from ) {

    return this->AbstractPersistenceProvider_makePersistentBranch ( from );
}

//----------------------------------------------------------------//
void AbstractPersistenceProvider::persist ( VersionedStoreTag& tag, string tagName ) {

    if ( !tag.getSourceBranch ()) return;

    try {
        this->begin ();

        PersistenceTag& persistedTag = this->mTags [ tagName ];
        
        persistedTag.mTagName   = tagName;
        persistedTag.mOwner     = this;
        
        persistedTag.takeSnapshot ( tag );
        persistedTag.getSourceBranch ()->persistSelf ( *this );
        
        this->AbstractPersistenceProvider_tagDidChange ( persistedTag );
        
        this->commit ();
    }
    catch ( ... ) {

        // TODO: try to recover and gracefully report the error
        exit ( 1 );
    }
}

//----------------------------------------------------------------//
void AbstractPersistenceProvider::remove ( string tagName ) {

    if ( !this->hasTag ( tagName )) return;
    
    PersistenceTag& tag = this->mTags [ tagName ];
    this->AbstractPersistenceProvider_removeTag ( tag );
    this->mTags.erase ( tagName );
}

//----------------------------------------------------------------//
VersionedStoreTag AbstractPersistenceProvider::restore ( string tagName ) {

    map < string, PersistenceTag >::const_iterator tagIt = this->mTags.find ( tagName );
    if ( tagIt != this->mTags.end ()) {
        return VersionedStoreTag ( tagIt->second );
    }
    return VersionedStoreTag ();
}

} // namespace Padamose
