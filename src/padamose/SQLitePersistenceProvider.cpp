// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <padamose/SQLitePersistenceProvider.h>
#include <padamose/SQLiteVersionedBranch.h>

namespace Padamose {

//================================================================//
// SQLitePersistenceProvider
//================================================================//

//----------------------------------------------------------------//
shared_ptr < SQLiteVersionedBranch > SQLitePersistenceProvider::affirmBranch ( u64 branchID ) {

    if ( !branchID ) return NULL;

    std::map < u64, weak_ptr < SQLiteVersionedBranch >>::iterator findIt = this->mBranchesByID.find ( branchID );
    if ( this->mBranchesByID.find ( branchID ) != this->mBranchesByID.end ()) {
    
        weak_ptr < SQLiteVersionedBranch > weakBranch = findIt->second;
        assert ( !weakBranch.expired ());
        return weakBranch.lock ();
    }

    shared_ptr < SQLiteVersionedBranch > branch = make_shared < SQLiteVersionedBranch >( this->shared_from_this (), branchID );
    this->insertBranch ( branch );
    
    return branch;
}

//----------------------------------------------------------------//
// TODO: doxygen
void SQLitePersistenceProvider::eraseBranch ( SQLiteVersionedBranch& branch ) {

    assert ( this->mBranchesByID.find ( branch.mBranchID ) != this->mBranchesByID.end ());
    this->mBranchesByID.erase ( branch.mBranchID );
    this->mIDsByBranch.erase ( &branch );
}

//----------------------------------------------------------------//
// TODO: doxygen
u64 SQLitePersistenceProvider::getIDForBranch ( const AbstractVersionedBranch& branch ) const {

    const AbstractVersionedBranch* branchPtr = &branch;
    std::map < const AbstractVersionedBranch*, u64 >::const_iterator findIt = this->mIDsByBranch.find ( branchPtr );
    assert ( findIt != this->mIDsByBranch.end ());
    return findIt->second;
}

//----------------------------------------------------------------//
void SQLitePersistenceProvider::insertBranch ( shared_ptr < SQLiteVersionedBranch > branch ) {

    assert ( branch );
    this->mBranchesByID [ branch->mBranchID ] = branch;
    this->mIDsByBranch [ branch.get ()] = branch->mBranchID;
}

//----------------------------------------------------------------//
// TODO: doxygen
void SQLitePersistenceProvider::loadFromStore () {

    assert ( this->mDB );

    this->begin ();

    SQLiteResult result = this->mDB.exec (
        
        "SELECT name, branchID, version FROM tags", NULL,
        
        //--------------------------------//
        [ & ]( int, const SQLiteStatement& stmt ) {
            
            string tagName      = stmt.getValue < string >( 0 );
            u64 branchID        = stmt.getValue < int >( 1 );
            u64 version         = stmt.getValue < int >( 2 );
            
            shared_ptr < SQLiteVersionedBranch > branch = this->affirmBranch ( branchID );
            assert ( branch );
            
            VersionedStoreTag& tag = this->mTags [ tagName ];
            tag.setParent ( branch, version );
        }
    );
    result.reportWithAssert ();

    std::vector < weak_ptr < SQLiteVersionedBranch >> branches;
    std::map < u64, weak_ptr < SQLiteVersionedBranch >>::iterator branchIt = this->mBranchesByID.begin ();
    for ( ; branchIt != this->mBranchesByID.end (); ++branchIt ) {
        branches.push_back ( branchIt->second );
    }

    for ( size_t i = 0; i < branches.size (); ++i ) {

        shared_ptr < SQLiteVersionedBranch > branch = branches [ i ].lock ();
        if ( !branch ) continue;

        branch->optimize ();
    }

    this->commit ();
}

//----------------------------------------------------------------//
void SQLitePersistenceProvider::open ( string filename, int flags, bool enableWAL ) {

    SQLiteResult result = this->mDB.open ( filename, flags, enableWAL );
    result.reportWithAssert ();
    assert ( this->mDB );
    
    // tags
    result = this->mDB.exec ( SQL_STR (
        CREATE TABLE IF NOT EXISTS tags (
            name            STRING                  PRIMARY KEY,
            branchID        INTEGER                 NOT NULL DEFAULT 0,
            version         INTEGER                 NOT NULL DEFAULT 0,
            
            FOREIGN KEY ( branchID ) REFERENCES branches ( id )
        )
    ));
    result.reportWithAssert ();
    
    // branches
    result = this->mDB.exec ( SQL_STR (
        CREATE TABLE IF NOT EXISTS branches (
            branchID        INTEGER                 PRIMARY KEY,
            sourceBranchID  INTEGER                 NOT NULL DEFAULT 0,
            version         INTEGER                 NOT NULL DEFAULT 0,
            top             INTEGER                 NOT NULL DEFAULT 0,
            
            FOREIGN KEY ( sourceBranchID ) REFERENCES branches ( id )
        )
    ));
    result.reportWithAssert ();
    
    // tuples
    result = this->mDB.exec ( SQL_STR (
        CREATE TABLE IF NOT EXISTS tuples (
            branchID        INTEGER                 NOT NULL DEFAULT 0,
            version         INTEGER                 NOT NULL DEFAULT 0,
            key             TEXT                    NOT NULL DEFAULT "",
            type            INTEGER                 NOT NULL DEFAULT 0,
            stringVal       TEXT                    NOT NULL DEFAULT "",
            intVal          INTEGER                 NOT NULL DEFAULT 0,
            realVal         INTEGER                 NOT NULL DEFAULT 0
        )
    ));
    result.reportWithAssert ();
    
    // indices

    result = this->mDB.exec ( SQL_STR ( CREATE UNIQUE INDEX IF NOT EXISTS tuplesIndex ON tuples ( key, version, branchID )));
    result.reportWithAssert ();
    
    this->loadFromStore ();
}

//----------------------------------------------------------------//
// TODO: doxygen
SQLitePersistenceProvider::SQLitePersistenceProvider () {
}

//----------------------------------------------------------------//
// TODO: doxygen
SQLitePersistenceProvider::~SQLitePersistenceProvider () {

    this->freeze ();
    this->mTags.clear ();
}

//================================================================//
// overrides
//================================================================//

//----------------------------------------------------------------//
void SQLitePersistenceProvider::AbstractPersistenceProvider_begin () {

    assert ( this->mDB );
    this->mDB.beginTransaction ();
}

//----------------------------------------------------------------//
void SQLitePersistenceProvider::AbstractPersistenceProvider_commit () {

    assert ( this->mDB );
    this->mDB.commitTransaction ();
}

//----------------------------------------------------------------//
// TODO: doxygen
shared_ptr < AbstractPersistentVersionedBranch > SQLitePersistenceProvider::AbstractPersistenceProvider_makePersistentBranch ( AbstractVersionedBranch& from ) {

    shared_ptr < SQLiteVersionedBranch > branch = make_shared < SQLiteVersionedBranch >( this->shared_from_this (), from );
    this->insertBranch ( branch );

    return branch;
}

//----------------------------------------------------------------//
// TODO: doxygen
void SQLitePersistenceProvider::AbstractPersistenceProvider_removeTag ( const PersistenceTag& tag ) {

    string name = tag.getName ();

    assert ( this->mDB );

    this->begin ();

    SQLiteResult result = this->mDB.exec (

        "DELETE FROM tags WHERE name = ?1",

        //--------------------------------//
        [ & ]( SQLiteStatement& stmt ) {
            stmt.bind ( 1, name );
        }
    );
    result.reportWithAssert ();
    
    this->commit ();
}

//----------------------------------------------------------------//
// TODO: doxygen
void SQLitePersistenceProvider::AbstractPersistenceProvider_tagDidChange ( const PersistenceTag& tag ) {

    string name = tag.getName ();

    assert ( this->mDB );

    this->begin ();
        
    u64 branchID = this->getIDForBranch ( *tag.getSourceBranch ());
    
    SQLiteResult result = this->mDB.exec (
    
        "INSERT OR REPLACE INTO tags ( name, branchID, version ) VALUES ( ?1, ?2, ?3 )",
        
        //--------------------------------//
        [ & ]( SQLiteStatement& stmt ) {
            stmt.bind ( 1, name );
            stmt.bind ( 2, branchID );
            stmt.bind ( 3, ( u64 )tag.getVersion ());
        }
    );
    result.reportWithAssert ();
    
    this->commit ();
}

} // namespace Padamose
