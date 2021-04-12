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
cc8* SQLiteResult::getCodeName ( int code ) {

    switch ( code ) {
        case SQLITE_OK:             return "Successful result";
        case SQLITE_ERROR:          return "Generic error";
        case SQLITE_INTERNAL:       return "Internal logic error in SQLite";
        case SQLITE_PERM:           return "Access permission denied";
        case SQLITE_ABORT:          return "Callback routine requested an abort";
        case SQLITE_BUSY:           return "The database file is locked";
        case SQLITE_LOCKED:         return "A table in the database is locked";
        case SQLITE_NOMEM:          return "A malloc() failed";
        case SQLITE_READONLY:       return "Attempt to write a readonly database";
        case SQLITE_INTERRUPT:      return "Operation terminated by sqlite3_interrupt()";
        case SQLITE_IOERR:          return "Some kind of disk I/O error occurred";
        case SQLITE_CORRUPT:        return "The database disk image is malformed";
        case SQLITE_NOTFOUND:       return "Unknown opcode in sqlite3_file_control()";
        case SQLITE_FULL:           return "Insertion failed because database is full";
        case SQLITE_CANTOPEN:       return "Unable to open the database file";
        case SQLITE_PROTOCOL:       return "Database lock protocol error";
        case SQLITE_EMPTY:          return "Internal use only";
        case SQLITE_SCHEMA:         return "The database schema changed";
        case SQLITE_TOOBIG:         return "String or BLOB exceeds size limit";
        case SQLITE_CONSTRAINT:     return "Abort due to constraint violation";
        case SQLITE_MISMATCH:       return "Data type mismatch";
        case SQLITE_MISUSE:         return "Library used incorrectly";
        case SQLITE_NOLFS:          return "Uses OS features not supported on host";
        case SQLITE_AUTH:           return "Authorization denied";
        case SQLITE_FORMAT:         return "Not used";
        case SQLITE_RANGE:          return "2nd parameter to sqlite3_bind out of range";
        case SQLITE_NOTADB:         return "File opened that is not a database file";
        case SQLITE_NOTICE:         return "Notifications from sqlite3_log()";
        case SQLITE_WARNING:        return "Warnings from sqlite3_log()";
        case SQLITE_ROW:            return "sqlite3_step() has another row ready";
       case  SQLITE_DONE:           return "Done.";
    }
    return "Unknown";
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
void SQLiteResult::reportWithAssert () const {

    if ( SQLiteResult::isErrorCode ( this->mCode )) {
        LGN_LOG ( PDM_FILTER_SQLITE, ERROR, "%s", SQLiteResult::getCodeName ( this->mCode ));
        LGN_LOG ( PDM_FILTER_SQLITE, ERROR, "%s", this->mMessage.c_str ());
        assert ( false );
    }
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
SQLiteResult SQLite::beginTransaction () {

    SQLiteResult result;

    if ( this->mTransactionDepth == 0 ) {
        result = this->exec ( "BEGIN TRANSACTION" );
        if ( result ) {
            this->mTransactionDepth++;
        }
    }
    return result;
}

//----------------------------------------------------------------//
SQLiteResult SQLite::close () {

    SQLiteResult closeResult = this->mDB ? SQLiteResult ( *this, sqlite3_close ( this->mDB )) : SQLiteResult ();

    this->mDB = NULL;
    this->mTransactionDepth = 0;
    return closeResult;
}

//----------------------------------------------------------------//
SQLiteResult SQLite::commitTransaction () {

    SQLiteResult result;

    if ( this->mTransactionDepth == 1 ) {
        result = this->exec ( "COMMIT" );
    }
    
    if ( result ) {
        this->mTransactionDepth--;
    }
    
    return result;
}

//----------------------------------------------------------------//
SQLiteResult SQLite::exec ( string sql ) {

    return this->exec ( sql, NULL, ( SQLRowCallbackFunc )NULL );
}

//----------------------------------------------------------------//
SQLiteResult SQLite::exec ( string sql, SQLPrepareCallbackFunc onPrepare ) {

    return this->exec ( sql, onPrepare, ( SQLRowCallbackFunc )NULL );
}

//----------------------------------------------------------------//
SQLiteResult SQLite::exec ( string sql, SQLPrepareCallbackFunc onPrepare, SQLRowCallbackFunc onRow ) {
    
    sqlite3_stmt* stmt;
    
    SQLiteResult result = this->prepare ( sql, &stmt, onPrepare );
    if ( !result ) return result;

    result = this->innerExec ( stmt, onRow );
    sqlite3_finalize ( stmt );
    
    return result;
}

//----------------------------------------------------------------//
SQLiteResult SQLite::innerExec ( sqlite3_stmt* stmt, SQLRowCallbackFunc onRow ) {

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
SQLiteResult SQLite::open ( string filename, int flags ) {

    SQLiteResult result ( *this, sqlite3_open_v2 ( filename.c_str (), &this->mDB, flags, NULL ));
    if ( !result ) {
        sqlite3_close ( this->mDB );
        this->mDB = NULL;
        return result;
    }
    
    string mode;
    SQLiteResult pragmaResult = this->exec ( "PRAGMA journal_mode=WAL", NULL,
        
        //--------------------------------//
        [ & ]( int, const SQLiteStatement& stmt ) {
            mode = stmt.getValue < string >( 0 );
        }
    );
    pragmaResult.reportWithAssert ();
    assert (( mode == "wal" ) || ( mode == "" ));
    
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
        SQLiteStatement statement ( *stmt );
        onPrepare ( statement );
    }
    
    return result;
}

//----------------------------------------------------------------//
SQLite::SQLite () :
    mDB ( NULL ),
    mTransactionDepth ( 0 ) {
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
