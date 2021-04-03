// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <stdio.h>
#include <padamose/gtest/gtest-helpers.h>
#include <padamose/gtest/util.h>
#include <padamose/padamose.h>
#include <padamose/SQLiteStringStore.h>

namespace Padamose {
namespace Test {

static const string KEY0    = "test0";
static const string KEY1    = "test1";
static const string STR0    = "abc";
static const string STR1    = "def";
static const string STR2    = "ghi";
static const string STR3    = "jkl";
static const string STR4    = "mno";

static cc8* SQLITE_FILE         = "sqlite-test.db";
static cc8* SQLITE_FILE_SHM     = "sqlite-test.db-shm";
static cc8* SQLITE_FILE_WAL     = "sqlite-test.db-wal";

//----------------------------------------------------------------//
void cleanup () {

    ASSERT_EQ ( remove ( SQLITE_FILE ), 0 );
    ASSERT_EQ ( exists ( SQLITE_FILE ), false );
    
    ASSERT_EQ ( remove ( SQLITE_FILE_SHM ), 0 );
    ASSERT_EQ ( exists ( SQLITE_FILE_SHM ), false );
    
    ASSERT_EQ ( remove ( SQLITE_FILE_WAL ), 0 );
    ASSERT_EQ ( exists ( SQLITE_FILE_WAL ), false );
}

//----------------------------------------------------------------//
TEST ( SQLitePersistence, test_sqlite_string_store ) {

    if ( exists ( SQLITE_FILE )) {
        remove ( SQLITE_FILE );
    }
    ASSERT_EQ ( exists ( SQLITE_FILE ), false );

    {
        SQLiteStringStore store;
        store.open ( SQLITE_FILE );

        ASSERT_EQ (( bool )store, true );
        ASSERT_EQ ( exists ( SQLITE_FILE ), true );

        ASSERT_EQ ( store.hasString ( KEY0 ), false );
        
        store.setString ( KEY0, STR0 );
        
        ASSERT_EQ ( store.hasString ( KEY0 ), true );
        ASSERT_EQ ( store.getString ( KEY0 ), STR0 );
        
        store.setString ( KEY0, STR1 );
        
        ASSERT_EQ ( store.hasString ( KEY0 ), true );
        ASSERT_EQ ( store.getString ( KEY0 ), STR1 );
        
        store.eraseString ( KEY0 );
        
        ASSERT_EQ ( store.hasString ( KEY0 ), false );
        
        store.setString ( KEY0, STR2 );
        
        ASSERT_EQ ( store.hasString ( KEY0 ), true );
        ASSERT_EQ ( store.getString ( KEY0 ), STR2 );
        
        store.clear ();
        
        ASSERT_EQ ( store.hasString ( KEY0 ), false );
    }

    cleanup ();
}

//----------------------------------------------------------------//
TEST ( SQLitePersistence, test_sqlite_persistence ) {

    {
        shared_ptr < SQLiteStringStore > stringStore = SQLiteStringStore::make ( SQLITE_FILE );
        testWithProvider ( stringStore );

        VersionedStoreTag store ( stringStore, "master" );

        ASSERT_EQ ( store.getVersion (), 3 );
        ASSERT_EQ ( store.getValue < string >( KEY0, 0 ), STR0 );
        ASSERT_EQ ( store.getValue < string >( KEY0, 1 ), STR1 );
        ASSERT_EQ ( store.getValue < string >( KEY0, 2 ), STR2 );
        ASSERT_EQ ( store.getValue < string >( KEY0, 3 ), STR3 );
        
        store.revert ( 2 );
        
        ASSERT_EQ ( store.getVersion (), 2 );
        ASSERT_EQ ( store.getValue < string >( KEY0 ), STR2 );
        
        store.persist ( stringStore, "master" );
    }
    
    {
        shared_ptr < SQLiteStringStore > stringStore = SQLiteStringStore::make ( SQLITE_FILE );
        
        VersionedStoreTag store ( stringStore, "master" );
        
        ASSERT_EQ ( store.getVersion (), 2 );
        ASSERT_EQ ( store.getValue < string >( KEY0 ), STR2 );
    }
    
    cleanup ();
}

} // namespace Test
} // namespace Padamose
