// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <padamose/gtest/gtest-helpers.h>
#include <padamose/gtest/util.h>
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
TEST ( StringPersistence, test_new_branch ) {

    shared_ptr < DebugStringStore > stringStore = make_shared < DebugStringStore >();
    shared_ptr < StringStorePersistenceProvider > provider = make_shared < StringStorePersistenceProvider >( stringStore );
    
    {
        VersionedStore store;
        store.persist ( provider, "master" );
        store.setValue < string >( KEY0, STR0 );
        store.persist ( provider, "master" );
    }
    
    stringStore->dump ();
    
    {
        VersionedStore store;
        store.takeSnapshot ( provider, "master" );
        ASSERT_EQ ( store.getValue < string >( KEY0 ), STR0 );
    }
}

//----------------------------------------------------------------//
TEST ( StringPersistence, test_string_persistence ) {

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
