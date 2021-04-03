// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <padamose/gtest/gtest-helpers.h>
#include <padamose/padamose.h>

namespace Padamose {
namespace Test {

static const string STR0    = "abc";
static const string STR1    = "def";
static const string STR2    = "ghi";
static const string STR3    = "jkl";
static const string STR4    = "mno";
static const string STR5    = "pqr";
static const string STR6    = "stu";

//----------------------------------------------------------------//
TEST ( VersionedList, test_insert_value_inserts_value_and_returns_valid_key ) {

    VersionedStoreTag store;
    
    VersionedList versionedSet ( store, "test" );

    string key0 = versionedSet.pushBack < string >( STR0 );
    string key1 = versionedSet.pushBack < string >( STR1 );
    string key2 = versionedSet.pushBack < string >( STR2 );

    ASSERT_TRUE ( versionedSet.getValue < string >( key0 ) == STR0 );
    ASSERT_TRUE ( versionedSet.getValue < string >( key1 ) == STR1 );
    ASSERT_TRUE ( versionedSet.getValue < string >( key2 ) == STR2 );

    versionedSet.setValue < string >( key1, STR3 );
    ASSERT_TRUE ( versionedSet.getValue < string >( key1 ) == STR3 );
}

//----------------------------------------------------------------//
TEST ( VersionedList, test_deleted_keys_are_reused ) {

    VersionedStoreTag store;
    
    VersionedList versionedSet ( store, "test" );

    string key0 = versionedSet.pushBack < string >( STR0 );
    string key1 = versionedSet.pushBack < string >( STR1 );
    string key2 = versionedSet.pushBack < string >( STR2 );

    versionedSet.deleteKey ( key1 );
    string key3 = versionedSet.pushBack < string >( STR3 );
    
    ASSERT_TRUE ( key1 == key3 );
    ASSERT_TRUE ( versionedSet.getValue < string >( key3 ) == STR3 );
    
    versionedSet.deleteKey ( key0 );
    versionedSet.deleteKey ( key2 );
    
    string key4 = versionedSet.pushBack < string >( STR4 );
    string key5 = versionedSet.pushBack < string >( STR5 );

    // assumes implementation uses a stack for deleted nodes; keys
    // should be reallocated in reverse order they were deleted.
    ASSERT_TRUE ( key4 == key2 );
    ASSERT_TRUE ( key5 == key0 );

    ASSERT_TRUE ( versionedSet.getValue < string >( key4 ) == STR4 );
    ASSERT_TRUE ( versionedSet.getValue < string >( key5 ) == STR5 );

    // should be head of the list
    versionedSet.deleteKey ( key5 );
    
    string key6 = versionedSet.pushBack < string >( STR6 );

    ASSERT_TRUE ( key5 == key6 );
    ASSERT_TRUE ( versionedSet.getValue < string >( key6 ) == STR6 );
}

//----------------------------------------------------------------//
TEST ( VersionedList, test_element_count_is_updated_correctly ) {

    VersionedStoreTag store;
    
    VersionedList versionedSet ( store, "test" );

    string key0 = versionedSet.pushBack < s64 >( 0 );
    string key1 = versionedSet.pushBack < s64 >( 1 );
    string key2 = versionedSet.pushBack < s64 >( 2 );

    ASSERT_TRUE ( versionedSet.getSize () == 3 );

    versionedSet.deleteKey ( key1 );
    
    ASSERT_TRUE ( versionedSet.getSize () == 2 );
    
    string key3 = versionedSet.pushBack < s64 >( 3 );
    string key4 = versionedSet.pushBack < s64 >( 4 );
    string key5 = versionedSet.pushBack < s64 >( 5 );
    
    ASSERT_TRUE ( versionedSet.getSize () == 5 );
    
    versionedSet.deleteKey ( key0 );
    versionedSet.deleteKey ( key2 );
    versionedSet.deleteKey ( key3 );
    versionedSet.deleteKey ( key4 );
    versionedSet.deleteKey ( key5 );
    
    ASSERT_TRUE ( versionedSet.getSize () == 0 );
}

//----------------------------------------------------------------//
TEST ( VersionedList, test_iterator ) {

    VersionedStoreTag store;
    
    VersionedList versionedSet ( store, "test" );

    string key0 = versionedSet.pushBack < s64 >( 0 );
    string key1 = versionedSet.pushBack < s64 >( 1 );
    string key2 = versionedSet.pushBack < s64 >( 2 );
    
    // this iterator test relies on keys being provisioned in a specific order
    // and always added to the *back* of the key list.
    
    VersionedCollectionIterator < s64 > setIt ( versionedSet );
    
    ASSERT_TRUE ( setIt == true );
    ASSERT_TRUE ( *setIt == 0 );
    
    setIt.next ();
    ASSERT_TRUE ( *setIt == 1 );
    
    setIt.next ();
    ASSERT_TRUE ( *setIt == 2 );
    
    setIt.next ();
    ASSERT_TRUE ( setIt == false );
    
    setIt.prev ();
    
    ASSERT_TRUE ( setIt == true );
    ASSERT_TRUE ( *setIt == 2 );
    
    setIt.seekFront ();
    
    ASSERT_TRUE ( setIt == true );
    ASSERT_TRUE ( *setIt == 0 );
    
    setIt.seekBack ();
    
    ASSERT_TRUE ( setIt == true );
    ASSERT_TRUE ( *setIt == 2 );
}

} // namespace Test
} // namespace Padamose
