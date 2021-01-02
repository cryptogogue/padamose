// Copyright (c) 2017-2018 Cryptogogue, Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_SQLITE_H
#define PADAMOSE_SQLITE_H

#include <padamose/padamose-common.h>
#include <sqlite3.h>

namespace Padamose {

class SQLite;

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

    typedef std::function < int ( int, char**, char** )>                                SQLCallbackFunc;
    typedef std::function < void ( sqlite3_stmt* )>                                     SQLPrepareCallbackFunc;
    typedef std::function < void ( int, const map < string, int >&, sqlite3_stmt* )>    SQLRowCallbackFunc;
    
protected:

    friend class SQLiteResult;

    sqlite3*                mDB;

    //----------------------------------------------------------------//
    SQLiteResult            prepare                 ( string sql, sqlite3_stmt** stmt, SQLPrepareCallbackFunc onPrepare = NULL );

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
    SQLiteResult            exec                    ( sqlite3_stmt* stmt, SQLRowCallbackFunc onRow );
    SQLiteResult            exec                    ( string sql, SQLPrepareCallbackFunc onPrepare = NULL, SQLRowCallbackFunc onRow = NULL );
    SQLiteResult            open                    ( string filename );
                            SQLite                  ();
                            SQLite                  ( string filename );
                            ~SQLite                 ();
};

} // namespace Padamose
#endif
