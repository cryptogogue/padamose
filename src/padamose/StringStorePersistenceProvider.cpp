// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <padamose/StringStorePersistenceProvider.h>
#include <padamose/StringStoreVersionedBranch.h>

namespace Padamose {

//================================================================//
// StringStorePersistenceProvider
//================================================================//

//----------------------------------------------------------------//
shared_ptr < StringStoreVersionedBranch > StringStorePersistenceProvider::affirmBranch ( string branchID ) {

    std::map < string, weak_ptr < StringStoreVersionedBranch >>::iterator findIt = this->mBranchesByID.find ( branchID );
    if ( this->mBranchesByID.find ( branchID ) != this->mBranchesByID.end ()) {
    
        weak_ptr < StringStoreVersionedBranch > weakBranch = findIt->second;
        assert ( !weakBranch.expired ());
        return weakBranch.lock ();
    }

    shared_ptr < StringStoreVersionedBranch > branch = make_shared < StringStoreVersionedBranch >( *this, branchID );
    branch->loadFromStore ();
    this->insertBranch ( branch );
    
    return branch;
}

//----------------------------------------------------------------//
string StringStorePersistenceProvider::formatKeyForBranchIDByTagName ( string tagName ) const {

    ostringstream stream;
    stream << this->mPrefix << ".tagList.branchIDByTagName." << tagName;
    return stream.str ();
}

//----------------------------------------------------------------//
string StringStorePersistenceProvider::formatKeyForTagListSize () const {

    ostringstream stream;
    stream << this->mPrefix << ".tagList.size";
    return stream.str ();
}

//----------------------------------------------------------------//
string StringStorePersistenceProvider::formatKeyForTagNameByTagListIndex ( size_t index ) const {

    ostringstream stream;
    stream << this->mPrefix << ".tagList.nameByIndex." << index;
    return stream.str ();
}

//----------------------------------------------------------------//
string StringStorePersistenceProvider::formatKeyForVersionByTagName ( string tagName ) const {

    ostringstream stream;
    stream << this->mPrefix << ".tagList.versionByTagName." << tagName;
    return stream.str ();
}

//----------------------------------------------------------------//
// TODO: doxygen
void StringStorePersistenceProvider::eraseBranch ( StringStoreVersionedBranch& branch ) {

    assert ( this->mBranchesByID.find ( branch.mBranchID ) != this->mBranchesByID.end ());
    this->mBranchesByID.erase ( branch.mBranchID );
    this->mIDsByBranch.erase ( &branch );
}

//----------------------------------------------------------------//
// TODO: doxygen
string StringStorePersistenceProvider::getIDForBranch ( const AbstractVersionedBranch& branch ) const {

    const AbstractVersionedBranch* branchPtr = &branch;
    std::map < const AbstractVersionedBranch*, string >::const_iterator findIt = this->mIDsByBranch.find ( branchPtr );
    assert ( findIt != this->mIDsByBranch.end ());
    return findIt->second;
}

//----------------------------------------------------------------//
// TODO: doxygen
string StringStorePersistenceProvider::getPrefix () const {

    return this->mPrefix;
}

//----------------------------------------------------------------//
void StringStorePersistenceProvider::insertBranch ( shared_ptr < StringStoreVersionedBranch > branch ) {

    assert ( branch );
    this->mBranchesByID [ branch->mBranchID ] = branch;
    this->mIDsByBranch [ branch.get ()] = branch->mBranchID;
}

//----------------------------------------------------------------//
// TODO: doxygen
void StringStorePersistenceProvider::loadFromStore () {

    AbstractStringStore& store = *this->mStore;

    string keyForTagListSize = this->formatKeyForTagListSize ();
    size_t tagListSize = store.get < size_t >( keyForTagListSize, 0 );

    for ( size_t i = 0; i < tagListSize; ++i ) {
    
        string keyForTagNameByTagListIndex = this->formatKeyForTagNameByTagListIndex ( i );
        string tagName = store.get < string >( keyForTagNameByTagListIndex, "" );
        assert ( tagName.size ());
        
        string keyForBranchIDByTagName = this->formatKeyForBranchIDByTagName ( tagName );
        string branchID = store.get < string >( keyForBranchIDByTagName, "" );
        assert ( branchID.size ());
        
        string keyForVersionByTagName = this->formatKeyForVersionByTagName ( tagName );
        size_t version = store.get < size_t >( keyForVersionByTagName, INVALID_VERSION );
        assert ( version != INVALID_VERSION );
        
        shared_ptr < StringStoreVersionedBranch > branch = this->affirmBranch ( branchID );
        assert ( branch );
        
        VersionedStoreSnapshot& tag = this->mTags [ tagName ];
        tag.setBranch ( branch, version );
    }
}

//----------------------------------------------------------------//
// TODO: doxygen
string StringStorePersistenceProvider::makeBranchID () const {

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
// TODO: doxygen
StringStorePersistenceProvider::StringStorePersistenceProvider () {

    assert ( false );
}

//----------------------------------------------------------------//
// TODO: doxygen
StringStorePersistenceProvider::StringStorePersistenceProvider ( shared_ptr < AbstractStringStore > store, string prefix ) :
    mStore ( store ),
    mPrefix ( prefix ) {

    assert ( this->mStore );
    
    if ( prefix.size () && ( prefix.at ( prefix.size () - 1 ) != '.' )) {
        this->mPrefix.append ( 1, '.' );
    }
    this->loadFromStore ();
}

//----------------------------------------------------------------//
// TODO: doxygen
StringStorePersistenceProvider::~StringStorePersistenceProvider () {

    this->freeze ();
    this->mTags.clear ();
}

//================================================================//
// overrides
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
shared_ptr < AbstractPersistentVersionedBranch > StringStorePersistenceProvider::AbstractPersistenceProvider_makePersistentBranch () {

    string branchID = this->makeBranchID ();
    
    shared_ptr < StringStoreVersionedBranch > branch = make_shared < StringStoreVersionedBranch >( *this, branchID );
    this->insertBranch ( branch );
    
    return branch;
}

//----------------------------------------------------------------//
// TODO: doxygen
void StringStorePersistenceProvider::AbstractPersistenceProvider_tagDidChange ( string name, const VersionedStoreSnapshot* snapshot ) {

    AbstractStringStore& store = *this->mStore;
    
    string keyForBranchIDByTagName = this->formatKeyForBranchIDByTagName ( name );
    string prevBranchID = store.get < string >( keyForBranchIDByTagName, "" );
    
    string keyForVersionByTagName = this->formatKeyForVersionByTagName ( name );
    
    if ( snapshot ) {
    
        string branchID = this->getIDForBranch ( *snapshot->getSourceBranch ());
        assert ( branchID.size ());
        
        if ( !prevBranchID.size ()) {
            string keyForTagListSize = this->formatKeyForTagListSize ();
            size_t tagListIndex = store.get < size_t >( keyForTagListSize, 0 );
            
            string keyForTagNameByTagListIndex = this->formatKeyForTagNameByTagListIndex ( tagListIndex );
            store.set < string >( keyForTagNameByTagListIndex, name );
            store.set < size_t >( keyForTagListSize, tagListIndex + 1 );
        }
        
        store.set < string >( keyForBranchIDByTagName, branchID );
        store.set < size_t >( keyForVersionByTagName, snapshot->getVersion ());
    }
    else if ( prevBranchID.size ()) {
    
        string keyForTagListSize = this->formatKeyForTagListSize ();
        size_t tagListSize = store.get < size_t >( keyForTagListSize, 0 );
        
        size_t cursor = 0;
        for ( size_t i = 0; i < tagListSize; ++i ) {
            string keyForTagNameByTagListIndex = this->formatKeyForTagNameByTagListIndex ( i );
            string moveName = store.get < string >( keyForTagNameByTagListIndex, "" );
            assert ( moveName.size ());
            if ( moveName != name ) {
                keyForTagNameByTagListIndex = this->formatKeyForTagNameByTagListIndex ( cursor++ );
                store.set < string >( keyForTagNameByTagListIndex, moveName );
            }
        }
        assert ( cursor == ( tagListSize - 1 ));
        store.eraseString ( keyForBranchIDByTagName );
        store.eraseString ( keyForVersionByTagName );
    }
}

} // namespace Padamose
