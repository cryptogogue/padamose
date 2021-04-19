// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <padamose/gtest/gtest-helpers.h>
#include <padamose/padamose.h>

namespace Padamose {
namespace Test {

//----------------------------------------------------------------//
void testVersionedStoreIterator ( VersionedStoreTag& store, string key, const vector < string > expected ) {

    VersionedStoreIterator storeIt ( store );

    size_t idx = expected.size ();

    for ( ; storeIt; storeIt.prev ()) {
        ASSERT_TRUE ( storeIt.getValue < string >( key ) == expected [ --idx ]);
    }
    
    ASSERT_TRUE ( idx == 0 );
    
    for ( storeIt.next (); storeIt; storeIt.next ()) {
        ASSERT_TRUE ( storeIt.getValue < string >( key ) == expected [ idx++ ]);
    }
    
    ASSERT_TRUE ( idx == expected.size ());
}

//----------------------------------------------------------------//
void testVersionedValueIterator ( VersionedStoreTag& store, string key, const vector < string > expected ) {

    VersionedValueIterator < string > valueIt ( store, key );

    size_t idx = expected.size ();

    for ( ; valueIt; valueIt.prev ()) {
        ASSERT_TRUE ( *valueIt == expected [ --idx ]);
        ASSERT_TRUE ( valueIt.value () == expected [ idx ]);
    }
    
    ASSERT_TRUE ( idx == 0 );
    
    for ( valueIt.next (); valueIt; valueIt.next ()) {
        ASSERT_TRUE ( *valueIt == expected [ idx ]);
        ASSERT_TRUE ( valueIt.value () == expected [ idx++ ]);
    }
    
    ASSERT_TRUE ( idx == expected.size ());
}

//----------------------------------------------------------------//
TEST ( VersionedStoreIterators, test_iterators ) {

    const string KEY = "test";

    VersionedStoreTag store0;
    VersionedStoreTag store1;

    store0.setValue < string >( KEY, "a" );
    store0.pushVersion ();
    
    store0.setValue < string >( KEY, "b" );
    store0.pushVersion ();
    
    store0.setValue < string >( KEY, "c" );
    store0.pushVersion ();

    store1 = store0;
    
    store0.setValue < string >( KEY, "aa" );
    store0.pushVersion ();

    store0.setValue < string >( KEY, "ab" );
    store0.pushVersion ();

    store0.setValue < string >( KEY, "ac" );

    store1.setValue < string >( KEY, "ba" );
    store1.pushVersion ();

    store1.setValue < string >( KEY, "bb" );
    store1.pushVersion ();

    store1.setValue < string >( KEY, "bc" );
    
    vector < string > expected0 = {
        "a",
        "b",
        "c",
        "aa",
        "ab",
        "ac",
    };
    
    vector < string > expected1 = {
        "a",
        "b",
        "c",
        "ba",
        "bb",
        "bc",
    };
    
    testVersionedStoreIterator ( store0, KEY, expected0 );
    testVersionedStoreIterator ( store1, KEY, expected1 );
    
    testVersionedValueIterator ( store0, KEY, expected0 );
    testVersionedValueIterator ( store1, KEY, expected1 );
}

//----------------------------------------------------------------//
TEST ( VersionedStoreIterators, test_version_iterator_with_branches ) {

    const string KEY = "test";

    VersionedStoreTag store0;

    VersionedStoreTag tag0;
    VersionedStoreTag tag1;
    VersionedStoreTag tag2;

    store0.setValue < string >( KEY, "a" );
    
    store0.pushVersion ();
    store0.setValue < string >( KEY, "b" );
    
    store0.pushVersion ();
    store0.setValue < string >( KEY, "c" );
    
    tag0 = store0;
    
    store0.pushVersion ();
    store0.setValue < string >( KEY, "aa" );
    
    store0.pushVersion ();
    store0.setValue < string >( KEY, "bb" );
    
    store0.pushVersion ();
    store0.setValue < string >( KEY, "cc" );

    tag1 = store0;

    store0.pushVersion ();
    store0.setValue < string >( KEY, "aaa" );
    
    store0.pushVersion ();
    store0.setValue < string >( KEY, "bbb" );
    
    store0.pushVersion ();
    store0.setValue < string >( KEY, "ccc" );
    
    tag2 = store0;
    
    tag2.printTree ();
    
    vector < string > expected0 = {
        "a",
        "b",
        "c",
    };
    
    vector < string > expected1 = {
        "a",
        "b",
        "c",
        "aa",
        "bb",
        "cc",
    };
    
    vector < string > expected2 = {
        "a",
        "b",
        "c",
        "aa",
        "bb",
        "cc",
        "aaa",
        "bbb",
        "ccc",
    };
    
    testVersionedStoreIterator ( tag0, KEY, expected0 );
    testVersionedStoreIterator ( tag1, KEY, expected1 );
    testVersionedStoreIterator ( tag2, KEY, expected2 );
    
    VersionedStoreIterator storeIt ( store0 );
    
    storeIt.seek ( 0 );
    ASSERT_TRUE ( storeIt.getValue < string >( KEY ) == "a" );
    
    storeIt.seek ( 10 );
    ASSERT_TRUE ( storeIt.getValue < string >( KEY ) == "ccc" );
    
    storeIt.seek ( 8 );
    ASSERT_TRUE ( storeIt.getValue < string >( KEY ) == "ccc" );
    
    storeIt.seek ( 7 );
    ASSERT_TRUE ( storeIt.getValue < string >( KEY ) == "bbb" );
}

} // namespace Test
} // namespace Padamose
