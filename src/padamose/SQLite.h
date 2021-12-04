// Copyright (c) 2017-2018 Cryptogogue, Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_SQLITE_H
#define PADAMOSE_SQLITE_H

#include <padamose/padamose-common.h>
#include <sqlite3.h>

#define SQL_STR(...) #__VA_ARGS__

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
    void            bind                    ( int key, bool value );
    void            bind                    ( int key, double value );
    void            bind                    ( int key, int value );
    void            bind                    ( int key, s64 value );
    void            bind                    ( int key, string value );
    void            bind                    ( int key, u64 value );
    bool            isNull                  ( int idx ) const;
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
template <> bool        SQLiteStatement::getValue < bool >          ( int idx ) const;
template <> double      SQLiteStatement::getValue < double >        ( int idx ) const;
template <> int         SQLiteStatement::getValue < int >           ( int idx ) const;
template <> s64         SQLiteStatement::getValue < s64 >           ( int idx ) const;
template <> string      SQLiteStatement::getValue < string >        ( int idx ) const;
template <> u64         SQLiteStatement::getValue < u64 >           ( int idx ) const;

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
    static cc8*     getCodeName         ( int code );
    string          getMessage          () const;
    static bool     isErrorCode         ( int code );
    void            reportWithAssert    () const;
                    SQLiteResult        ();
                    SQLiteResult        ( SQLite& sqlite, int result );
                    SQLiteResult        ( const SQLiteResult& other );
};

//================================================================//
// SQLiteConfig
//================================================================//
class SQLiteConfig {
public:

    enum JournalMode {
        JOURNAL_MODE_ROLLBACK,
        JOURNAL_MODE_WAL,
        JOURNAL_MODE_UNKNOWN,
    };

    int             mFlags;
    JournalMode     mJournalMode;
    
    //----------------------------------------------------------------//
    SQLiteConfig () :
        mFlags ( SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE ),
        mJournalMode ( JOURNAL_MODE_WAL ) {
    }
    
    //----------------------------------------------------------------//
    SQLiteConfig ( int flags, JournalMode journalMode ) :
        mFlags ( flags ),
        mJournalMode ( journalMode ) {
    }
};

//================================================================//
// SQLite
//================================================================//
class SQLite {
public:

//    enum ExecType {
//        READ,
//        WRITE,
//    };

    typedef std::function < void ( SQLiteStatement& )>                  SQLPrepareCallbackFunc;
    typedef std::function < void ( int, const SQLiteStatement& )>       SQLRowCallbackFunc;
    
protected:

    friend class SQLiteResult;

    sqlite3*                mDB;
    size_t                  mTransactionDepth;

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
    void                    beginTransaction        ();
    SQLiteResult            close                   ();
    void                    commitTransaction       ();
    SQLiteResult            exec                    ( string sql );
    SQLiteResult            exec                    ( string sql, SQLPrepareCallbackFunc onPrepare );
    SQLiteResult            exec                    ( string sql, SQLPrepareCallbackFunc onPrepare, SQLRowCallbackFunc onRow );
    size_t                  getTransactionDepth     () const;
    SQLiteResult            open                    ( string filename, SQLiteConfig config = SQLiteConfig ());
                            SQLite                  ();
                            SQLite                  ( string filename );
                            ~SQLite                 ();
};

} // namespace Padamose
#endif
