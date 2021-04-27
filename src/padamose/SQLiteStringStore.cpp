// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <padamose/SQLiteStringStore.h>
#include <padamose/padamose-hiredis.h>

namespace Padamose {

//================================================================//
// SQLiteStringStore
//================================================================//

//----------------------------------------------------------------//
void SQLiteStringStore::clear () {

    SQLite& db = this->mDB;
    assert ( db );

    db.exec ( "DELETE FROM padamose" );
}

//----------------------------------------------------------------//
void SQLiteStringStore::open ( string filename, SQLiteConfig config ) {

    this->mDB.open ( filename, config );
    assert ( this->mDB );

    this->mDB.exec ( "CREATE TABLE IF NOT EXISTS padamose ( key TEXT PRIMARY KEY, value TEXT NOT NULL )" );
    
    this->loadFromStore ();
}

//----------------------------------------------------------------//
SQLiteStringStore::SQLiteStringStore () :
    AbstractStringStore ( "" ) {
}

//----------------------------------------------------------------//
SQLiteStringStore::~SQLiteStringStore () {
}

//================================================================//
// overrides
//================================================================//

//----------------------------------------------------------------//
void SQLiteStringStore::AbstractPersistenceProvider_begin () {

    assert ( this->mDB );
    this->mDB.beginTransaction ();
}

//----------------------------------------------------------------//
void SQLiteStringStore::AbstractPersistenceProvider_commit () {

    assert ( this->mDB );
    this->mDB.commitTransaction ();
}

//----------------------------------------------------------------//
void SQLiteStringStore::AbstractStringStore_eraseString ( string key ) {

    assert ( this->mDB );
    
    this->mDB.exec (
    
        "DELETE FROM padamose WHERE key = ?1",
        
        //--------------------------------//
        [ & ]( SQLiteStatement& stmt ) {
            stmt.bind ( 1, key );
        }
    );
}

//----------------------------------------------------------------//
string SQLiteStringStore::AbstractStringStore_getString ( string key ) const {

    assert ( this->mDB );

    string result;
    
    this->mDB.exec (
    
        "SELECT value FROM padamose WHERE key = ?1",
        
        //--------------------------------//
        [ & ]( SQLiteStatement& stmt ) {
            stmt.bind ( 1, key );
        },
        
        //--------------------------------//
        [ & ]( int, const SQLiteStatement& stmt ) {
            result = stmt.getValue < string >( 0 );
        }
    );

    return result;
}

//----------------------------------------------------------------//
bool SQLiteStringStore::AbstractStringStore_hasString ( string key ) const {

    assert ( this->mDB );

    bool result = false;
    
    this->mDB.exec (
    
        "SELECT EXISTS ( SELECT 1 FROM padamose WHERE key = ?1 )",
        
        //--------------------------------//
        [ & ]( SQLiteStatement& stmt ) {
            stmt.bind ( 1, key );
        },
        
        //--------------------------------//
        [ & ]( int, const SQLiteStatement& stmt ) {
            result = ( stmt.getValue < int >( 0 ) == 1 );
        }
    );

    return result;
}

//----------------------------------------------------------------//
void SQLiteStringStore::AbstractStringStore_setString ( string key, string value ) {

    assert ( this->mDB );
    
    this->mDB.exec (
    
        "REPLACE INTO padamose ( key, value ) VALUES ( ?1, ?2 )",
        
        //--------------------------------//
        [ & ]( SQLiteStatement& stmt ) {
            stmt.bind ( 1, key );
            stmt.bind ( 2, value );
        }
    );
}

} // namespace Padamose
