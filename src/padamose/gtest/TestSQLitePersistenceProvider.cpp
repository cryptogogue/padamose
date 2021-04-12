// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <stdio.h>
#include <padamose/gtest/gtest-helpers.h>
#include <padamose/gtest/util.h>
#include <padamose/padamose.h>
#include <padamose/SQLitePersistenceProvider.h>

namespace Padamose {
namespace Test {

//static const string KEY0    = "test0";
//static const string KEY1    = "test1";
//static const string STR0    = "abc";
//static const string STR1    = "def";
//static const string STR2    = "ghi";
//static const string STR3    = "jkl";
//static const string STR4    = "mno";
//
//static cc8* SQLITE_FILE         = "sqlite-test.db";
//static cc8* SQLITE_FILE_SHM     = "sqlite-test.db-shm";
//static cc8* SQLITE_FILE_WAL     = "sqlite-test.db-wal";
//
////----------------------------------------------------------------//
//void cleanup () {
//
//    ASSERT_EQ ( remove ( SQLITE_FILE ), 0 );
//    ASSERT_EQ ( exists ( SQLITE_FILE ), false );
//    
//    if ( exists ( SQLITE_FILE_SHM )) {
//        ASSERT_EQ ( remove ( SQLITE_FILE_SHM ), 0 );
//        ASSERT_EQ ( exists ( SQLITE_FILE_SHM ), false );
//    }
//    
//    if ( exists ( SQLITE_FILE_WAL )) {
//        ASSERT_EQ ( remove ( SQLITE_FILE_WAL ), 0 );
//        ASSERT_EQ ( exists ( SQLITE_FILE_WAL ), false );
//    }
//}
//
////----------------------------------------------------------------//
//TEST ( SQLitePersistenceProvider, test_sqlite_persistence ) {
//
//    {
//        shared_ptr < SQLitePersistenceProvider > stringStore = SQLitePersistenceProvider::make ( SQLITE_FILE );
//        testWithProvider ( *stringStore );
//
//        VersionedStoreTag tag = stringStore->restore ( "master" );
//
//        ASSERT_EQ ( tag.getVersion (), 3 );
//        ASSERT_EQ ( tag.getValue < string >( KEY0, 0 ), STR0 );
//        ASSERT_EQ ( tag.getValue < string >( KEY0, 1 ), STR1 );
//        ASSERT_EQ ( tag.getValue < string >( KEY0, 2 ), STR2 );
//        ASSERT_EQ ( tag.getValue < string >( KEY0, 3 ), STR3 );
//        
//        tag.revert ( 2 );
//        
//        ASSERT_EQ ( tag.getVersion (), 2 );
//        ASSERT_EQ ( tag.getValue < string >( KEY0 ), STR2 );
//        
//        stringStore->persist ( tag, "master" );
//    }
//    
//    {
//        shared_ptr < SQLitePersistenceProvider > stringStore = SQLitePersistenceProvider::make ( SQLITE_FILE );
//        
//        VersionedStoreTag tag = stringStore->restore ( "master" );
//        
//        ASSERT_EQ ( tag.getVersion (), 2 );
//        ASSERT_EQ ( tag.getValue < string >( KEY0 ), STR2 );
//    }
//    
//    cleanup ();
//}

} // namespace Test
} // namespace Padamose
