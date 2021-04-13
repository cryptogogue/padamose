// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_GTEST_SQLITELOCALDB_H
#define PADAMOSE_GTEST_SQLITELOCALDB_H

#include <padamose/gtest/util.h>

namespace Padamose {
namespace Test {

//================================================================//
// SQLiteLocalDB
//================================================================//
class SQLiteLocalDB {
public:

    static constexpr const char* SQLITE_FILENAME        = "sqlite-test.db";
    static constexpr const char* SQLITE_FILENAME_SHM    = "sqlite-test.db-shm";
    static constexpr const char* SQLITE_FILENAME_WAL    = "sqlite-test.db-wal";

    //----------------------------------------------------------------//
    void cleanupSqliteFiles () {
    
        if ( exists ( SQLITE_FILENAME )) {
            ASSERT_EQ ( remove ( SQLITE_FILENAME ), 0 );
            ASSERT_EQ ( exists ( SQLITE_FILENAME ), false );
        }
        
        if ( exists ( SQLITE_FILENAME_SHM )) {
            ASSERT_EQ ( remove ( SQLITE_FILENAME_SHM ), 0 );
            ASSERT_EQ ( exists ( SQLITE_FILENAME_SHM ), false );
        }
        
        if ( exists ( SQLITE_FILENAME_WAL )) {
            ASSERT_EQ ( remove ( SQLITE_FILENAME_WAL ), 0 );
            ASSERT_EQ ( exists ( SQLITE_FILENAME_WAL ), false );
        }
    }

    //----------------------------------------------------------------//
    SQLiteLocalDB () {
        this->cleanupSqliteFiles ();
    }
    
    //----------------------------------------------------------------//
    ~SQLiteLocalDB () {
        this->cleanupSqliteFiles ();
    }
};

} // namespace Test
} // namespace Padamose
#endif
