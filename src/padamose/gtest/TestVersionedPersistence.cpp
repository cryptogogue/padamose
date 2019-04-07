// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <padamose/gtest/gtest-helpers.h>
#include <padamose/padamose.h>

namespace Padamose {
namespace Test {

static const string KEY0    = "test0";
static const string KEY1    = "test1";
static const string STR0    = "abc";
static const string STR1    = "def";
static const string STR2    = "ghi";
static const string STR3    = "jkl";
static const string STR4    = "mno";

//----------------------------------------------------------------//
void testWithProvider ( shared_ptr < AbstractPersistenceProvider > provider ) {

    {
        VersionedStore store;

        store.setValue < string >( KEY0, STR0 );
        store.pushVersion (); // version 1
        store.setValue < string >( KEY0, STR1 );
        store.pushVersion (); // version 2
        store.setValue < string >( KEY0, STR2 );
        store.pushVersion (); // version 3
        store.setValue < string >( KEY0, STR3 );

        ASSERT_EQ ( store.getVersion (), 3 );
        ASSERT_EQ ( store.getValue < string >( KEY0 ), STR3 );
        
        store.persist ( provider, "master" );
    }

    VersionedStore store ( provider, "master" );

    ASSERT_EQ ( store.getVersion (), 3 );
    ASSERT_EQ ( store.getValue < string >( KEY0, 0 ), STR0 );
    ASSERT_EQ ( store.getValue < string >( KEY0, 1 ), STR1 );
    ASSERT_EQ ( store.getValue < string >( KEY0, 2 ), STR2 );
    ASSERT_EQ ( store.getValue < string >( KEY0, 3 ), STR3 );
    
    // iterate by version
    VersionedStoreIterator storeIt ( store );
    ASSERT_EQ ( storeIt.getValue < string >( KEY0 ), STR3 );
    
    storeIt.prev ();
    ASSERT_EQ ( storeIt.getValue < string >( KEY0 ), STR2 );
    
    storeIt.prev ();
    ASSERT_EQ ( storeIt.getValue < string >( KEY0 ), STR1 );
    
    storeIt.prev ();
    ASSERT_EQ ( storeIt.getValue < string >( KEY0 ), STR0 );
    
    storeIt.next ();
    ASSERT_EQ ( storeIt.getValue < string >( KEY0 ), STR1 );
    
    storeIt.next ();
    ASSERT_EQ ( storeIt.getValue < string >( KEY0 ), STR2 );
    
    storeIt.next ();
    ASSERT_EQ ( storeIt.getValue < string >( KEY0 ), STR3 );
    
    // iterate by value
    VersionedValueIterator < string > valueIt ( store, KEY0 );
    ASSERT_EQ ( valueIt.value (), STR3 );
    
    valueIt.prev ();
    ASSERT_EQ ( valueIt.value (), STR2 );
    
    valueIt.prev ();
    ASSERT_EQ ( valueIt.value (), STR1 );
    
    valueIt.prev ();
    ASSERT_EQ ( valueIt.value (), STR0 );
    
    valueIt.next ();
    ASSERT_EQ ( valueIt.value (), STR1 );
    
    valueIt.next ();
    ASSERT_EQ ( valueIt.value (), STR2 );
    
    valueIt.next ();
    ASSERT_EQ ( valueIt.value (), STR3 );
}

//----------------------------------------------------------------//
TEST ( VersionedStore, test_simple_persistence ) {

    shared_ptr < DebugStringStore > stringStore = make_shared < DebugStringStore >();
    shared_ptr < StringStorePersistenceProvider > provider = make_shared < StringStorePersistenceProvider >( stringStore );
    
    testWithProvider ( provider );
    VersionedStoreSnapshot snapshot ( provider, "master" );
    
    stringStore->dump ();
    provider = NULL;

    printf ( "not empty:\n" );
    stringStore->dump ();
    snapshot.clear ();

    printf ( "still not empty:\n" );
    stringStore->dump ();
    printf ( "done\n" );
    
    // load provider from store
    provider = make_shared < StringStorePersistenceProvider >( stringStore );
    
    VersionedStore store ( provider, "master" );

    ASSERT_EQ ( store.getVersion (), 3 );
    ASSERT_EQ ( store.getValue < string >( KEY0, 0 ), STR0 );
    ASSERT_EQ ( store.getValue < string >( KEY0, 1 ), STR1 );
    ASSERT_EQ ( store.getValue < string >( KEY0, 2 ), STR2 );
    ASSERT_EQ ( store.getValue < string >( KEY0, 3 ), STR3 );
}

} // namespace Test
} // namespace Padamose
