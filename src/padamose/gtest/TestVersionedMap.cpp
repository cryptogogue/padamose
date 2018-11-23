// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <padamose/gtest/gtest-helpers.h>
#include <padamose/VersionedMap.h>
#include <padamose/VersionedCollectionIterator.h>

using namespace Padamose;

static const string KEY0    = "key0";
static const string KEY1    = "key1";
static const string KEY2    = "key2";
static const string KEY3    = "key3";
static const string KEY4    = "key4";
static const string KEY5    = "key5";

static const string STR0    = "abc";
static const string STR1    = "def";
static const string STR2    = "ghi";
static const string STR3    = "jkl";
static const string STR4    = "mno";
static const string STR5    = "pqr";
static const string STR6    = "stu";

//================================================================//
// VersionedMapCollisionTester
//================================================================//
class VersionedMapCollisionTester :
    public VersionedMap {
private:

    //----------------------------------------------------------------//
    size_t VersionedMap_hash ( string key ) const {
        return 0;
    }

public:

    //----------------------------------------------------------------//
    VersionedMapCollisionTester ( VersionedStore& store, string name ) :
        VersionedMap ( store, name ) {
    }
};

//----------------------------------------------------------------//
TEST ( VersionedMap, test_set_and_get_value ) {

    VersionedStore store;
    
    VersionedMap versionedMap ( store, "test" );

    versionedMap.setValue < string >( KEY0, STR0 );
    versionedMap.setValue < string >( KEY1, STR1 );
    versionedMap.setValue < string >( KEY2, STR2 );

    ASSERT_TRUE ( versionedMap.getValue < string >( KEY0 ) == STR0 );
    ASSERT_TRUE ( versionedMap.getValue < string >( KEY1 ) == STR1 );
    ASSERT_TRUE ( versionedMap.getValue < string >( KEY2 ) == STR2 );

    versionedMap.setValue < string >( KEY1, STR3 );
    ASSERT_TRUE ( versionedMap.getValue < string >( KEY1 ) == STR3 );
}

//----------------------------------------------------------------//
TEST ( VersionedMap, test_element_count_is_updated_correctly ) {

    VersionedStore store;
    
    VersionedMap versionedMap ( store, "test" );

    versionedMap.setValue < int >( KEY0, 0 );
    versionedMap.setValue < int >( KEY1, 1 );
    versionedMap.setValue < int >( KEY2, 2 );

    ASSERT_TRUE ( versionedMap.getSize () == 3 );

    versionedMap.deleteKey ( KEY1 );
    
    ASSERT_TRUE ( versionedMap.getSize () == 2 );
    
    versionedMap.setValue < int >( KEY3, 3 );
    versionedMap.setValue < int >( KEY4, 4 );
    versionedMap.setValue < int >( KEY5, 5 );
    
    ASSERT_TRUE ( versionedMap.getSize () == 5 );
    
    versionedMap.deleteKey ( KEY0 );
    versionedMap.deleteKey ( KEY2 );
    versionedMap.deleteKey ( KEY3 );
    versionedMap.deleteKey ( KEY4 );
    versionedMap.deleteKey ( KEY5 );
    
    ASSERT_TRUE ( versionedMap.getSize () == 0 );
}

//----------------------------------------------------------------//
TEST ( VersionedMap, test_iterator ) {

    VersionedStore store;
    
    VersionedMap versionedMap ( store, "test" );

    versionedMap.setValue < int >( KEY0, 0 );
    versionedMap.setValue < int >( KEY1, 1 );
    versionedMap.setValue < int >( KEY2, 2 );
    
    // this iterator test relies on keys being provisioned in a specific order
    // and always added to the *back* of the key list.
    
    VersionedCollectionIterator setIt ( versionedMap );
    
    ASSERT_TRUE ( setIt == true );
    ASSERT_TRUE ( setIt.value < int >() == 0 );
    ASSERT_TRUE ( setIt.key () == KEY0 );
    
    setIt.next ();
    ASSERT_TRUE ( setIt.value < int >() == 1 );
    ASSERT_TRUE ( setIt.key () == KEY1 );
    
    setIt.next ();
    ASSERT_TRUE ( setIt.value < int >() == 2 );
    ASSERT_TRUE ( setIt.key () == KEY2 );
    
    setIt.next ();
    ASSERT_TRUE ( setIt == false );
    
    setIt.prev ();
    
    ASSERT_TRUE ( setIt == true );
    ASSERT_TRUE ( setIt.value < int >() == 2 );
    ASSERT_TRUE ( setIt.key () == KEY2 );
    
    setIt.seekFront ();
    
    ASSERT_TRUE ( setIt == true );
    ASSERT_TRUE ( setIt.value < int >() == 0 );
    ASSERT_TRUE ( setIt.key () == KEY0 );
    
    setIt.seekBack ();
    
    ASSERT_TRUE ( setIt == true );
    ASSERT_TRUE ( setIt.value < int >() == 2 );
    ASSERT_TRUE ( setIt.key () == KEY2 );
}

//----------------------------------------------------------------//
TEST ( VersionedMap, test_key_collision_handling ) {

    VersionedStore store;
    
    VersionedMapCollisionTester versionedMap ( store, "test" );

    versionedMap.setValue < string >( KEY0, STR0 );
    versionedMap.setValue < string >( KEY1, STR1 );
    versionedMap.setValue < string >( KEY2, STR2 );

    ASSERT_TRUE ( versionedMap.getValue < string >( KEY0 ) == STR0 );
    ASSERT_TRUE ( versionedMap.getValue < string >( KEY1 ) == STR1 );
    ASSERT_TRUE ( versionedMap.getValue < string >( KEY2 ) == STR2 );

    versionedMap.setValue < string >( KEY1, STR3 );
    ASSERT_TRUE ( versionedMap.getValue < string >( KEY1 ) == STR3 );
    
    versionedMap.deleteKey ( KEY1 );
    
    ASSERT_TRUE ( versionedMap.getSize () == 2 );
    
    versionedMap.setValue < string >( KEY1, STR4 );
    
    ASSERT_TRUE ( versionedMap.getValue < string >( KEY1 ) == STR4 );
    
    VersionedCollectionIterator setIt ( versionedMap );
    
    ASSERT_TRUE ( setIt == true );
    ASSERT_TRUE ( setIt.value < string >() == STR0 );
    
    setIt.next ();
    ASSERT_TRUE ( setIt.value < string >() == STR2 );
    
    setIt.next ();
    ASSERT_TRUE ( setIt.value < string >() == STR4 );
    
    setIt.next ();
    ASSERT_TRUE ( setIt == false );
}
