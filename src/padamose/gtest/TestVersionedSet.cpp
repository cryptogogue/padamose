// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <gtest-helpers.h>
#include <padamose/VersionedSet.h>

using namespace Padamose;

static const string STR0    = "abc";
static const string STR1    = "def";
static const string STR2    = "ghi";
static const string STR3    = "jkl";
static const string STR4    = "mno";
static const string STR5    = "pqr";
static const string STR6    = "stu";

//----------------------------------------------------------------//
TEST ( VersionedSet, test_insert_value_inserts_value_and_returns_valid_key ) {

    VersionedStore store;
    
    VersionedSet versionedSet ( store, "test" );

    string key0 = versionedSet.insertValue < string >( STR0 );
    string key1 = versionedSet.insertValue < string >( STR1 );
    string key2 = versionedSet.insertValue < string >( STR2 );

    ASSERT_TRUE ( versionedSet.getValue < string >( key0 ) == STR0 );
    ASSERT_TRUE ( versionedSet.getValue < string >( key1 ) == STR1 );
    ASSERT_TRUE ( versionedSet.getValue < string >( key2 ) == STR2 );

    versionedSet.setValue < string >( key1, STR3 );
    ASSERT_TRUE ( versionedSet.getValue < string >( key1 ) == STR3 );
}

//----------------------------------------------------------------//
TEST ( VersionedSet, test_deleted_keys_are_reused ) {

    VersionedStore store;
    
    VersionedSet versionedSet ( store, "test" );

    string key0 = versionedSet.insertValue < string >( STR0 );
    string key1 = versionedSet.insertValue < string >( STR1 );
    string key2 = versionedSet.insertValue < string >( STR2 );

    versionedSet.deleteKey ( key1 );
    string key3 = versionedSet.insertValue < string >( STR3 );
    
    ASSERT_TRUE ( key1 == key3 );
    ASSERT_TRUE ( versionedSet.getValue < string >( key3 ) == STR3 );
    
    versionedSet.deleteKey ( key0 );
    versionedSet.deleteKey ( key2 );
    
    string key4 = versionedSet.insertValue < string >( STR4 );
    string key5 = versionedSet.insertValue < string >( STR5 );

    // assumes implementation uses a stack for deleted nodes; keys
    // should be reallocated in reverse order they were deleted.
    ASSERT_TRUE ( key4 == key2 );
    ASSERT_TRUE ( key5 == key0 );

    ASSERT_TRUE ( versionedSet.getValue < string >( key4 ) == STR4 );
    ASSERT_TRUE ( versionedSet.getValue < string >( key5 ) == STR5 );

    // should be head of the list
    versionedSet.deleteKey ( key5 );
    
    string key6 = versionedSet.insertValue < string >( STR6 );

    ASSERT_TRUE ( key5 == key6 );
    ASSERT_TRUE ( versionedSet.getValue < string >( key6 ) == STR6 );
}

//----------------------------------------------------------------//
TEST ( VersionedSet, test_element_count_is_updated_correctly ) {

    VersionedStore store;
    
    VersionedSet versionedSet ( store, "test" );

    string key0 = versionedSet.insertValue < int >( 0 );
    string key1 = versionedSet.insertValue < int >( 1 );
    string key2 = versionedSet.insertValue < int >( 2 );

    ASSERT_TRUE ( versionedSet.getSize () == 3 );

    versionedSet.deleteKey ( key1 );
    
    ASSERT_TRUE ( versionedSet.getSize () == 2 );
    
    string key3 = versionedSet.insertValue < int >( 3 );
    string key4 = versionedSet.insertValue < int >( 4 );
    string key5 = versionedSet.insertValue < int >( 5 );
    
    ASSERT_TRUE ( versionedSet.getSize () == 5 );
    
    versionedSet.deleteKey ( key0 );
    versionedSet.deleteKey ( key2 );
    versionedSet.deleteKey ( key3 );
    versionedSet.deleteKey ( key4 );
    versionedSet.deleteKey ( key5 );
    
    ASSERT_TRUE ( versionedSet.getSize () == 0 );
}
