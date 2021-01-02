// Copyright (c) 2017-2018 Cryptogogue, Inc. All Rights Reserved.
// http://cryptogogue.com

#include <padamose/SQLite.h>

namespace Padamose {

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
SQLiteResult SQLite::exec ( sqlite3_stmt* stmt, SQLRowCallbackFunc onRow ) {

    int rows = 0;
    
    map < string, int > columns;
    
    while ( true ) {
        
        if ( sqlite3_step ( stmt ) != SQLITE_ROW ) break;
        
        if ( rows == 0 ){
            int nCols = sqlite3_column_count ( stmt );
            for ( int i = 0; i < nCols; i++ ) {
                cc8* name = ( char* )sqlite3_column_name ( stmt, i );
                columns [ name ] = i;
            }
        }
        if ( onRow ) {
            onRow ( rows, columns, stmt );
        }
        rows++;
    }
    return SQLiteResult ( *this, sqlite3_reset ( stmt ) );
}

//----------------------------------------------------------------//
SQLiteResult SQLite::exec ( string sql, SQLPrepareCallbackFunc onPrepare, SQLRowCallbackFunc onRow ) {
    
    sqlite3_stmt* stmt;
    
    SQLiteResult result = this->prepare ( sql, &stmt, onPrepare );
    if ( !result ) return result;

    result = this->exec ( stmt, onRow );
    sqlite3_finalize ( stmt );
    
    return result;
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
        onPrepare ( *stmt );
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
