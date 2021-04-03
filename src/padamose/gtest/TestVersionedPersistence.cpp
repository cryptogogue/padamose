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
    
    {
        VersionedStore store;
        store.persist ( stringStore, "master" );
        store.setValue < string >( KEY0, STR0 );
        store.persist ( stringStore, "master" );
    }
    
    stringStore->dump ();
    
    {
        VersionedStore store;
        store.takeSnapshot ( stringStore, "master" );
        ASSERT_EQ ( store.getValue < string >( KEY0 ), STR0 );
    }
}

//----------------------------------------------------------------//
TEST ( StringPersistence, test_string_persistence ) {

    shared_ptr < DebugStringStore > stringStore = make_shared < DebugStringStore >();
    
    testWithProvider ( stringStore );
    VersionedStoreSnapshot snapshot ( stringStore, "master" );
    
    stringStore->dump ();

    printf ( "not empty:\n" );
    stringStore->dump ();
    snapshot.clear ();

    printf ( "still not empty:\n" );
    stringStore->dump ();
    printf ( "done\n" );
    
    VersionedStore store ( stringStore, "master" );

    ASSERT_EQ ( store.getVersion (), 3 );
    ASSERT_EQ ( store.getValue < string >( KEY0, 0 ), STR0 );
    ASSERT_EQ ( store.getValue < string >( KEY0, 1 ), STR1 );
    ASSERT_EQ ( store.getValue < string >( KEY0, 2 ), STR2 );
    ASSERT_EQ ( store.getValue < string >( KEY0, 3 ), STR3 );
}

//----------------------------------------------------------------//
TEST ( StringPersistence, test_persistence ) {

    shared_ptr < DebugStringStore > stringStore = make_shared < DebugStringStore >();
    
    {
        VersionedStore store;
        store.setValue < string >( KEY0, STR0 );
        store.pushVersion ();
        store.persist ( stringStore, "master" );

        ASSERT_EQ ( store.getVersion (), 1 );
        ASSERT_EQ ( store.getValue < string >( KEY0 ), STR0 );
        
        store.setValue < string >( KEY0, STR1 );
        store.pushVersion ();
    }
    
    stringStore->dump ();
    
    VersionedStore store ( stringStore, "master" );
    
    ASSERT_EQ ( store.getVersion (), 1 );
    ASSERT_EQ ( store.getValue < string >( KEY0 ), STR0 );
    
    printf ( "OK!\n" );
}

//----------------------------------------------------------------//
TEST ( StringPersistence, test_branch_consolidation ) {

    shared_ptr < DebugStringStore > provider = make_shared < DebugStringStore >();
    
    VersionedStore store;
    
    store.setValue < string >( "KEY", "VERSION 0" );
    store.pushVersion ();
//    store.persist ( provider, "master" );
    store.printTree ();
//    provider->dump ();
//
    store.setValue < string >( "KEY", "VERSION 1" );
    store.pushVersion ();
//    store.persist ( provider, "master" );
    store.printTree ();
//    provider->dump ();

    VersionedStore store2 ( store );

    store.setValue < string >( "KEY", "VERSION 2" );
    store.pushVersion ();
//    store.persist ( provider, "master" );
    store.printTree ();
//    provider->dump ();
    
//    store2 = store;
//    store.persist ( provider, "master" );
//    provider->dump ();
//
//    store2.takeSnapshot ( store );
//    store2.revert ( 1 );
//    provider->dump ();
}

} // namespace Test
} // namespace Padamose
