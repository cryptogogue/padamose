// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <padamose/AbstractStringStore.h>
#include <padamose/StringStoreVersionedBranch.h>

namespace Padamose {

//================================================================//
// AbstractStringStore
//================================================================//

//----------------------------------------------------------------//
shared_ptr < StringStoreVersionedBranch > AbstractStringStore::affirmBranch ( string branchID ) {

    std::map < string, weak_ptr < StringStoreVersionedBranch >>::iterator findIt = this->mBranchesByID.find ( branchID );
    if ( this->mBranchesByID.find ( branchID ) != this->mBranchesByID.end ()) {
    
        weak_ptr < StringStoreVersionedBranch > weakBranch = findIt->second;
        assert ( !weakBranch.expired ());
        return weakBranch.lock ();
    }

    shared_ptr < StringStoreVersionedBranch > branch = make_shared < StringStoreVersionedBranch >( this->shared_from_this (), branchID );
    branch->loadFromStore ();
    this->insertBranch ( branch );
    
    return branch;
}

//----------------------------------------------------------------//
string AbstractStringStore::formatKeyForBranchIDByTagName ( string tagName ) const {

    ostringstream stream;
    stream << this->mPrefix << ".tagList.branchIDByTagName." << tagName;
    return stream.str ();
}

//----------------------------------------------------------------//
string AbstractStringStore::formatKeyForTagListSize () const {

    ostringstream stream;
    stream << this->mPrefix << ".tagList.size";
    return stream.str ();
}

//----------------------------------------------------------------//
string AbstractStringStore::formatKeyForTagNameByTagListIndex ( size_t index ) const {

    ostringstream stream;
    stream << this->mPrefix << ".tagList.nameByIndex." << index;
    return stream.str ();
}

//----------------------------------------------------------------//
string AbstractStringStore::formatKeyForVersionByTagName ( string tagName ) const {

    ostringstream stream;
    stream << this->mPrefix << ".tagList.versionByTagName." << tagName;
    return stream.str ();
}

//----------------------------------------------------------------//
// TODO: doxygen
void AbstractStringStore::eraseBranch ( StringStoreVersionedBranch& branch ) {

    assert ( this->mBranchesByID.find ( branch.mBranchID ) != this->mBranchesByID.end ());
    this->mBranchesByID.erase ( branch.mBranchID );
    this->mIDsByBranch.erase ( &branch );
}

//----------------------------------------------------------------//
void AbstractStringStore::eraseString ( string key ) {

    this->AbstractStringStore_eraseString ( key );
}

//----------------------------------------------------------------//
// TODO: doxygen
string AbstractStringStore::getIDForBranch ( const AbstractVersionedBranch& branch ) const {

    const AbstractVersionedBranch* branchPtr = &branch;
    std::map < const AbstractVersionedBranch*, string >::const_iterator findIt = this->mIDsByBranch.find ( branchPtr );
    assert ( findIt != this->mIDsByBranch.end ());
    return findIt->second;
}

//----------------------------------------------------------------//
// TODO: doxygen
string AbstractStringStore::getPrefix () const {

    return this->mPrefix;
}

//----------------------------------------------------------------//
string AbstractStringStore::getString ( string key ) const {

    return this->AbstractStringStore_getString ( key );
}

//----------------------------------------------------------------//
bool AbstractStringStore::hasString ( string key ) const {

    return this->AbstractStringStore_hasString ( key );
}

//----------------------------------------------------------------//
void AbstractStringStore::insertBranch ( shared_ptr < StringStoreVersionedBranch > branch ) {

    assert ( branch );
    this->mBranchesByID [ branch->mBranchID ] = branch;
    this->mIDsByBranch [ branch.get ()] = branch->mBranchID;
}

//----------------------------------------------------------------//
// TODO: doxygen
void AbstractStringStore::loadFromStore () {

    this->begin ();

    string keyForTagListSize = this->formatKeyForTagListSize ();
    size_t tagListSize = this->get < u64 >( keyForTagListSize, 0 );

    for ( size_t i = 0; i < tagListSize; ++i ) {
    
        string keyForTagNameByTagListIndex = this->formatKeyForTagNameByTagListIndex ( i );
        string tagName = this->get < string >( keyForTagNameByTagListIndex, "" );
        assert ( tagName.size ());
        
        string keyForBranchIDByTagName = this->formatKeyForBranchIDByTagName ( tagName );
        string branchID = this->get < string >( keyForBranchIDByTagName, "" );
        assert ( branchID.size ());
        
        string keyForVersionByTagName = this->formatKeyForVersionByTagName ( tagName );
        size_t version = this->get < u64 >( keyForVersionByTagName, INVALID_VERSION );
        assert ( version != INVALID_VERSION );
        
        shared_ptr < StringStoreVersionedBranch > branch = this->affirmBranch ( branchID );
        assert ( branch );
        
        VersionedStoreTag& tag = this->mTags [ tagName ];
        tag.setParent ( branch, version );
    }
    
    std::map < string, weak_ptr < StringStoreVersionedBranch >>::iterator branchIt = this->mBranchesByID.begin ();
    for ( ; branchIt != this->mBranchesByID.end (); ++branchIt ) {
    
        shared_ptr < StringStoreVersionedBranch > branch = branchIt->second.lock ();
        branch->optimize ();
    }
    
    this->commit ();
}

//----------------------------------------------------------------//
// TODO: doxygen
string AbstractStringStore::makeBranchID () const {

    string branchID;
    do {
        static const size_t BUFF_SIZE = 5;
        char buffer [ BUFF_SIZE ];
        snprintf ( buffer, BUFF_SIZE, "%x", rand ());
        branchID = buffer;
    } while ( this->mBranchesByID.find ( branchID ) != this->mBranchesByID.cend ());
    
    return branchID;
}

//----------------------------------------------------------------//
void AbstractStringStore::setString ( string key, string value ) {

    this->AbstractStringStore_setString ( key, value );
}

//----------------------------------------------------------------//
// TODO: doxygen
AbstractStringStore::AbstractStringStore () {

    assert ( false );
}

//----------------------------------------------------------------//
// TODO: doxygen
AbstractStringStore::AbstractStringStore ( string prefix ) :
    mPrefix ( prefix ) {
    
    if ( prefix.size () && ( prefix.at ( prefix.size () - 1 ) != '.' )) {
        this->mPrefix.append ( 1, '.' );
    }
}

//----------------------------------------------------------------//
// TODO: doxygen
AbstractStringStore::~AbstractStringStore () {

    this->freeze ();
    this->mTags.clear ();
}

//================================================================//
// overrides
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
shared_ptr < AbstractPersistentVersionedBranch > AbstractStringStore::AbstractPersistenceProvider_makePersistentBranch () {

    string branchID = this->makeBranchID ();
    
    shared_ptr < StringStoreVersionedBranch > branch = make_shared < StringStoreVersionedBranch >( this->shared_from_this (), branchID );
    this->insertBranch ( branch );
    
    return branch;
}

//----------------------------------------------------------------//
// TODO: doxygen
void AbstractStringStore::AbstractPersistenceProvider_tagDidChange ( string name, const VersionedStoreTag* snapshot ) {
    
    this->begin ();
    
    string keyForBranchIDByTagName = this->formatKeyForBranchIDByTagName ( name );
    string prevBranchID = this->get < string >( keyForBranchIDByTagName, "" );
    
    string keyForVersionByTagName = this->formatKeyForVersionByTagName ( name );
    
    if ( snapshot ) {
    
        string branchID = this->getIDForBranch ( *snapshot->getSourceBranch ());
        assert ( branchID.size ());
        
        if ( !prevBranchID.size ()) {
            string keyForTagListSize = this->formatKeyForTagListSize ();
            size_t tagListIndex = this->get < u64 >( keyForTagListSize, 0 );
            
            string keyForTagNameByTagListIndex = this->formatKeyForTagNameByTagListIndex ( tagListIndex );
            this->set < string >( keyForTagNameByTagListIndex, name );
            this->set < u64 >( keyForTagListSize, tagListIndex + 1 );
        }
        
        this->set < string >( keyForBranchIDByTagName, branchID );
        this->set < u64 >( keyForVersionByTagName, snapshot->getVersion ());
    }
    else if ( prevBranchID.size ()) {
    
        string keyForTagListSize = this->formatKeyForTagListSize ();
        size_t tagListSize = this->get < u64 >( keyForTagListSize, 0 );
        
        size_t cursor = 0;
        for ( size_t i = 0; i < tagListSize; ++i ) {
            string keyForTagNameByTagListIndex = this->formatKeyForTagNameByTagListIndex ( i );
            string moveName = this->get < string >( keyForTagNameByTagListIndex, "" );
            assert ( moveName.size ());
            if ( moveName != name ) {
                keyForTagNameByTagListIndex = this->formatKeyForTagNameByTagListIndex ( cursor++ );
                this->set < string >( keyForTagNameByTagListIndex, moveName );
            }
        }
        assert ( cursor == ( tagListSize - 1 ));
        this->eraseString ( keyForBranchIDByTagName );
        this->eraseString ( keyForVersionByTagName );
    }
    
    this->commit ();
}

} // namespace Padamose
