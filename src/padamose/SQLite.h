// Copyright (c) 2017-2018 Cryptogogue, Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_SQLITE_H
#define PADAMOSE_SQLITE_H

#include <padamose/padamose-common.h>
#include <sqlite3.h>

namespace Padamose {

class SQLite;

//================================================================//
// SQLiteStatement
//================================================================//
class SQLiteStatement {
protected:

    friend class SQLite;

    mutable sqlite3_stmt*           mStmt;
    mutable map < string, int >     mColumns;

    //----------------------------------------------------------------//
    void            affirmColumnNames       () const;

public:

    //----------------------------------------------------------------//
    operator sqlite3_stmt* () {
        return mStmt;
    }

    //----------------------------------------------------------------//
    void            bind                    ( int key, int value );
    void            bind                    ( int key, string value );
                    SQLiteStatement         ( sqlite3_stmt* stmt );
                    ~SQLiteStatement        ();
    
    //----------------------------------------------------------------//
    template < typename TYPE > TYPE     getValue                ( int idx ) const;
    
    //----------------------------------------------------------------//
    template < typename TYPE >
    TYPE getValue ( string key ) const {
        this->affirmColumnNames ();
        return this->SQLiteStatement::getValue < TYPE >( this->mColumns.find ( key )->second );
    }
};

//----------------------------------------------------------------//
template <> int         SQLiteStatement::getValue < int >           ( int idx ) const;
template <> string      SQLiteStatement::getValue < string >        ( int idx ) const;

//================================================================//
// SQLiteResult
//================================================================//
class SQLiteResult {
private:

    int             mCode;
    string          mMessage;

public:

    //----------------------------------------------------------------//
    operator        bool                () const;
    string          getMessage          () const;
    static bool     isErrorCode         ( int code );
                    SQLiteResult        ();
                    SQLiteResult        ( SQLite& sqlite, int result );
                    SQLiteResult        ( const SQLiteResult& other );
};

//================================================================//
// SQLite
//================================================================//
class SQLite {
public:

    typedef std::function < void ( SQLiteStatement& )>                  SQLPrepareCallbackFunc;
    typedef std::function < void ( int, const SQLiteStatement& )>       SQLRowCallbackFunc;
    
protected:

    friend class SQLiteResult;

    sqlite3*                mDB;

    //----------------------------------------------------------------//
    SQLiteResult            innerExec               ( sqlite3_stmt* stmt, SQLRowCallbackFunc onRow );
    SQLiteResult            prepare                 ( string sql, sqlite3_stmt** stmt, SQLPrepareCallbackFunc onPrepare );

public:

    //----------------------------------------------------------------//
    operator bool () const {
        return ( this->mDB != NULL );
    }

    //----------------------------------------------------------------//
    operator sqlite3* () {
        return this->mDB;
    }

    //----------------------------------------------------------------//
    SQLiteResult            close                   ();
    SQLiteResult            exec                    ( string sql );
    SQLiteResult            exec                    ( string sql, SQLPrepareCallbackFunc onPrepare );
    SQLiteResult            exec                    ( string sql, SQLPrepareCallbackFunc onPrepare, SQLRowCallbackFunc onRow );
    SQLiteResult            open                    ( string filename );
                            SQLite                  ();
                            SQLite                  ( string filename );
                            ~SQLite                 ();
};

} // namespace Padamose
#endif
