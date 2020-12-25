// Copyright (c) 2017-2018 Cryptogogue, Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_SQLITE_H
#define PADAMOSE_SQLITE_H

#include <padamose/padamose-common.h>
#include <sqlite3.h>

namespace Padamose {

//================================================================//
// SQLite
//================================================================//
class SQLite {
protected:

    sqlite3*                mDB;

public:

    typedef std::function < int ( int, char**, char** )>                                SQLCallbackFunc;
    typedef std::function < void ( sqlite3_stmt* )>                                     SQLPrepareCallbackFunc;
    typedef std::function < void ( int, const map < string, int >&, sqlite3_stmt* )>    SQLRowCallbackFunc;

    //----------------------------------------------------------------//
    operator bool () const {
        return ( this->mDB != NULL );
    }

    //----------------------------------------------------------------//
    operator sqlite3* () {
        return this->mDB;
    }

    //----------------------------------------------------------------//
    void                    close                   ();
    void                    exec                    ( sqlite3_stmt* stmt, SQLRowCallbackFunc onRow );
    void                    exec                    ( string sql, SQLPrepareCallbackFunc onPrepare = NULL, SQLRowCallbackFunc onRow = NULL );
    void                    open                    ( string filename );
    sqlite3_stmt*           prepare                 ( string sql, SQLPrepareCallbackFunc onPrepare = NULL );
                            SQLite                  ();
                            SQLite                  ( string filename );
                            ~SQLite                 ();
};

} // namespace Padamose
#endif
