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
SQLiteStringStore::SQLiteStringStore ( string filename, string prefix ) :
    AbstractStringStore ( prefix ) {
    
    this->mDB.open ( filename );
    assert ( this->mDB );

    this->mDB.exec ( "CREATE TABLE IF NOT EXISTS padamose ( key TEXT PRIMARY KEY, value TEXT NOT NULL )" );
    
    this->loadFromStore ();
}

//----------------------------------------------------------------//
SQLiteStringStore::~SQLiteStringStore () {
}

//================================================================//
// overrides
//================================================================//

//----------------------------------------------------------------//
void SQLiteStringStore::AbstractStringStore_eraseString ( string key ) {

    assert ( this->mDB );
    
    this->mDB.exec (
         "DELETE FROM padamose WHERE key = ?1",
        [ key ]( sqlite3_stmt* stmt ) {
            sqlite3_bind_text ( stmt, 1, key.c_str (), ( int )key.size (), SQLITE_TRANSIENT );
        }
    );
}

//----------------------------------------------------------------//
string SQLiteStringStore::AbstractStringStore_getString ( string key ) const {

    assert ( this->mDB );

    string result;
    
    this->mDB.exec (
        "SELECT value FROM padamose WHERE key = ?1",
        [ key ]( sqlite3_stmt* stmt ) {
            sqlite3_bind_text ( stmt, 1, key.c_str (), ( int )key.size (), SQLITE_TRANSIENT );
        },
        [ &result ]( int row, const map < string, int >& columns, sqlite3_stmt* stmt ) {
            UNUSED ( row );
            UNUSED ( columns );
            result = ( cc8* )sqlite3_column_text ( stmt, 0 );
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
        [ key ]( sqlite3_stmt* stmt ) {
            sqlite3_bind_text ( stmt, 1, key.c_str (), ( int )key.size (), SQLITE_TRANSIENT );
        },
        [ &result ]( int row, const map < string, int >& columns, sqlite3_stmt* stmt ) {
            UNUSED ( row );
            UNUSED ( columns );
            result = ( sqlite3_column_int ( stmt, 0 ) == 1 );
        }
    );

    return result;
}

//----------------------------------------------------------------//
void SQLiteStringStore::AbstractStringStore_setString ( string key, string value ) {

    assert ( this->mDB );
    
    this->mDB.exec (
        "REPLACE INTO padamose ( key, value ) VALUES ( ?1, ?2 )",
        [ key, value ]( sqlite3_stmt* stmt ) {
            sqlite3_bind_text ( stmt, 1, key.c_str (), ( int )key.size (), SQLITE_TRANSIENT );
            sqlite3_bind_text ( stmt, 2, value.c_str (), ( int )value.size (), SQLITE_TRANSIENT );
        }
    );
}

} // namespace Padamose
