// Copyright (c) 2017-2018 Cryptogogue, Inc. All Rights Reserved.
// http://cryptogogue.com

#include <padamose/SQLite.h>

namespace Padamose {

//================================================================//
// SQLiteStatement
//================================================================//

//----------------------------------------------------------------//
void SQLiteStatement::affirmColumnNames () const {

    if ( this->mColumns.size () > 0 ) return;
    
    int nCols = sqlite3_column_count ( this->mStmt );
    for ( int i = 0; i < nCols; i++ ) {
        string name = ( char* )sqlite3_column_name ( this->mStmt, i );
        this->mColumns [ name ] = i;
    }
}

//----------------------------------------------------------------//
void SQLiteStatement::bind ( int key, int value ) {

    sqlite3_bind_int ( this->mStmt, key, value );
}

//----------------------------------------------------------------//
void SQLiteStatement::bind ( int key, string value ) {

    sqlite3_bind_text ( this->mStmt, key, value.c_str (), ( int )value.size (), SQLITE_TRANSIENT );
}

//----------------------------------------------------------------//
template <>
int SQLiteStatement::getValue < int >( int idx ) const {
	
	return ( int )sqlite3_column_int ( this->mStmt, idx );
}

//----------------------------------------------------------------//
template <>
string SQLiteStatement::getValue < string >( int idx ) const {
	
	return ( cc8* )sqlite3_column_text ( this->mStmt, idx );
}

//----------------------------------------------------------------//
SQLiteStatement::SQLiteStatement ( sqlite3_stmt* stmt ) :
    mStmt ( stmt ) {
    assert ( this->mStmt );
}

//----------------------------------------------------------------//
SQLiteStatement::~SQLiteStatement () {
}

//================================================================//
// SQLiteResult
//================================================================//

//----------------------------------------------------------------//
SQLiteResult::operator bool () const {
    return ( !SQLiteResult::isErrorCode ( this->mCode ));
}

//----------------------------------------------------------------//
string SQLiteResult::getMessage () const {
    return this->mMessage;
}

//----------------------------------------------------------------//
bool SQLiteResult::isErrorCode ( int code ) {
    return !(( code == SQLITE_OK ) || ( code == SQLITE_ROW ) || ( code == SQLITE_DONE ));
}

//----------------------------------------------------------------//
SQLiteResult::SQLiteResult () :
    mCode ( SQLITE_OK ) {
}

//----------------------------------------------------------------//
SQLiteResult::SQLiteResult ( SQLite& sqlite, int result ) :
    mCode ( result ) {
    
    if ( result != SQLITE_OK ) {
        this->mMessage = sqlite3_errmsg ( sqlite.mDB );
    }
}

//----------------------------------------------------------------//
SQLiteResult::SQLiteResult ( const SQLiteResult& other ) {
    *this = other;
}

//================================================================//
// SQLite
//================================================================//

//----------------------------------------------------------------//
SQLiteResult SQLite::close () {

    if ( this->mDB ) {
        return SQLiteResult ( *this, sqlite3_close ( this->mDB ));
    }
    return SQLiteResult ();
}

//----------------------------------------------------------------//
SQLiteResult SQLite::exec ( string sql ) {

    return this->exec ( sql, NULL, ( SQLRowCallbackFunc )NULL );
}

//----------------------------------------------------------------//
SQLiteResult SQLite::exec ( string sql, SQLPrepareCallbackFunc onPrepare ) {

    return this->exec ( sql, onPrepare, ( SQLRowCallbackFunc )NULL, false );
}

//----------------------------------------------------------------//
SQLiteResult SQLite::exec ( string sql, SQLPrepareCallbackFunc onPrepare, SQLRowCallbackFunc onRow, bool getColumnNames ) {
    
    sqlite3_stmt* stmt;
    
    SQLiteResult result = this->prepare ( sql, &stmt, onPrepare );
    if ( !result ) return result;

    result = this->innerExec ( stmt, onRow, getColumnNames );
    sqlite3_finalize ( stmt );
    
    return result;
}

//----------------------------------------------------------------//
SQLiteResult SQLite::innerExec ( sqlite3_stmt* stmt, SQLRowCallbackFunc onRow, bool getColumnNames ) {

    int rows = 0;
    
    SQLiteStatement statement ( stmt );
    
    while ( true ) {
        
        if ( sqlite3_step ( stmt ) != SQLITE_ROW ) break;
        
        if ( onRow ) {
            onRow ( rows, statement );
        }
        rows++;
    }
    return SQLiteResult ( *this, sqlite3_reset ( stmt ) );
}

//----------------------------------------------------------------//
SQLiteResult SQLite::prepare ( string sql, sqlite3_stmt** stmt, SQLPrepareCallbackFunc onPrepare ) {

    assert ( stmt );

    *stmt = NULL;
    SQLiteResult result ( *this,
        sqlite3_prepare_v2 (
            this->mDB,
            sql.c_str (),
            ( int )sql.size (),
            stmt,
            NULL
        )
    );
    
    if ( !result ) {
        sqlite3_finalize ( *stmt );
        *stmt = NULL;
    }
    
    if ( *stmt && onPrepare ) {
        SQLiteStatement statement ( *stmt );
        onPrepare ( statement );
    }
    
    return result;
}

//----------------------------------------------------------------//
SQLiteResult SQLite::open ( string filename ) {

    SQLiteResult result ( *this, sqlite3_open ( filename.c_str (), &this->mDB ));
    if ( !result ) {
        sqlite3_close ( this->mDB );
        this->mDB = NULL;
    }
    return result;
}

//----------------------------------------------------------------//
SQLite::SQLite () :
    mDB ( NULL ) {
}

//----------------------------------------------------------------//
SQLite::SQLite ( string filename ) :
    mDB ( NULL ) {
    
    this->open ( filename );
}

//----------------------------------------------------------------//
SQLite::~SQLite () {

    this->close ();
}

} // namespace Padamose
