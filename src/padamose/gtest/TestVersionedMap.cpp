// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <padamose/gtest/gtest-helpers.h>
#include <padamose/padamose.h>

namespace Padamose {
namespace Test {

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
    VersionedMapCollisionTester ( VersionedStoreTag& store, string name ) :
        VersionedMap ( store, name ) {
    }
};

//----------------------------------------------------------------//
TEST ( VersionedMap, test_set_and_get_value ) {

    VersionedStoreTag store;
    
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

    VersionedStoreTag store;
    
    VersionedMap versionedMap ( store, "test" );

    versionedMap.setValue < s64 >( KEY0, 0 );
    versionedMap.setValue < s64 >( KEY1, 1 );
    versionedMap.setValue < s64 >( KEY2, 2 );

    ASSERT_TRUE ( versionedMap.getSize () == 3 );

    versionedMap.deleteKey ( KEY1 );
    
    ASSERT_TRUE ( versionedMap.getSize () == 2 );
    
    versionedMap.setValue < s64 >( KEY3, 3 );
    versionedMap.setValue < s64 >( KEY4, 4 );
    versionedMap.setValue < s64 >( KEY5, 5 );
    
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

    VersionedStoreTag store;
    
    VersionedMap versionedMap ( store, "test" );

    versionedMap.setValue < s64 >( KEY0, 0 );
    versionedMap.setValue < s64 >( KEY1, 1 );
    versionedMap.setValue < s64 >( KEY2, 2 );
    
    // this iterator test relies on keys being provisioned in a specific order
    // and always added to the *back* of the key list.
    
    VersionedCollectionIterator < s64 > setIt ( versionedMap );
    
    ASSERT_TRUE ( setIt == true );
    ASSERT_TRUE ( *setIt == 0 );
    ASSERT_TRUE ( setIt.key () == KEY0 );
    
    setIt.next ();
    ASSERT_TRUE ( *setIt == 1 );
    ASSERT_TRUE ( setIt.key () == KEY1 );
    
    setIt.next ();
    ASSERT_TRUE ( *setIt == 2 );
    ASSERT_TRUE ( setIt.key () == KEY2 );
    
    setIt.next ();
    ASSERT_TRUE ( setIt == false );
    
    setIt.prev ();
    
    ASSERT_TRUE ( setIt == true );
    ASSERT_TRUE ( *setIt == 2 );
    ASSERT_TRUE ( setIt.key () == KEY2 );
    
    setIt.seekFront ();
    
    ASSERT_TRUE ( setIt == true );
    ASSERT_TRUE ( *setIt == 0 );
    ASSERT_TRUE ( setIt.key () == KEY0 );
    
    setIt.seekBack ();
    
    ASSERT_TRUE ( setIt == true );
    ASSERT_TRUE ( *setIt == 2 );
    ASSERT_TRUE ( setIt.key () == KEY2 );
}

//----------------------------------------------------------------//
TEST ( VersionedMap, test_key_collision_handling ) {

    VersionedStoreTag store;
    
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
    
    VersionedCollectionIterator < string >setIt ( versionedMap );
    
    ASSERT_TRUE ( setIt == true );
    ASSERT_TRUE ( *setIt == STR0 );
    
    setIt.next ();
    ASSERT_TRUE ( *setIt == STR2 );
    
    setIt.next ();
    ASSERT_TRUE ( *setIt == STR4 );
    
    setIt.next ();
    ASSERT_TRUE ( setIt == false );
}

//----------------------------------------------------------------//
TEST ( VersionedMap, test_key_collision_handling_with_common_table ) {

    static const string COLLISION_PREFIX    = "__collision.";
    static const string DECOLLIDER_PREFIX   = "__decollider.";

    VersionedStoreTag store;
    
    {
        VersionedMapCollisionTester versionedMap ( store, "test0" );
        versionedMap.setCollisionPrefixes ( COLLISION_PREFIX, DECOLLIDER_PREFIX );

        versionedMap.setValue < string >( KEY0, STR0 );
        versionedMap.setValue < string >( KEY1, STR1 );
        versionedMap.setValue < string >( KEY2, STR2 );
    }

    VersionedMapCollisionTester versionedMap ( store, "test1" );
    versionedMap.setCollisionPrefixes ( COLLISION_PREFIX, DECOLLIDER_PREFIX );

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
    
    VersionedCollectionIterator < string >setIt ( versionedMap );
    
    ASSERT_TRUE ( setIt == true );
    ASSERT_TRUE ( *setIt == STR0 );
    
    setIt.next ();
    ASSERT_TRUE ( *setIt == STR2 );
    
    setIt.next ();
    ASSERT_TRUE ( *setIt == STR4 );
    
    setIt.next ();
    ASSERT_TRUE ( setIt == false );
}

} // namespace Test
} // namespace Padamose
