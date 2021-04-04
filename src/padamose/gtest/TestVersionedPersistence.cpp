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
        VersionedStoreTag tag;
        stringStore->persist ( tag, "master" );
        tag.setValue < string >( KEY0, STR0 );
        stringStore->persist ( tag, "master" );
    }
    
    stringStore->dump ();
    
    {
        VersionedStoreTag tag = stringStore->restore ( "master" );
        ASSERT_EQ ( tag.getValue < string >( KEY0 ), STR0 );
    }
}

//----------------------------------------------------------------//
TEST ( StringPersistence, test_string_persistence ) {

    shared_ptr < DebugStringStore > stringStore = make_shared < DebugStringStore >();
    
    testWithProvider ( *stringStore );
    ConstVersionedStoreTag snapshot  = stringStore->restore ( "master" );
    
    stringStore->dump ();

    printf ( "not empty:\n" );
    stringStore->dump ();
    snapshot.clear ();

    printf ( "still not empty:\n" );
    stringStore->dump ();
    printf ( "done\n" );
    
    VersionedStoreTag tag = stringStore->restore ( "master" );

    ASSERT_EQ ( tag.getVersion (), 3 );
    ASSERT_EQ ( tag.getValue < string >( KEY0, 0 ), STR0 );
    ASSERT_EQ ( tag.getValue < string >( KEY0, 1 ), STR1 );
    ASSERT_EQ ( tag.getValue < string >( KEY0, 2 ), STR2 );
    ASSERT_EQ ( tag.getValue < string >( KEY0, 3 ), STR3 );
}

//----------------------------------------------------------------//
TEST ( StringPersistence, test_persistence ) {

    shared_ptr < DebugStringStore > stringStore = make_shared < DebugStringStore >();
    
    {
        VersionedStoreTag tag;
        tag.setValue < string >( KEY0, STR0 );
        tag.pushVersion ();
        stringStore->persist ( tag, "master" );

        ASSERT_EQ ( tag.getVersion (), 1 );
        ASSERT_EQ ( tag.getValue < string >( KEY0 ), STR0 );
        
        tag.setValue < string >( KEY0, STR1 );
        tag.pushVersion ();
    }
    
    stringStore->dump ();
    
    VersionedStoreTag tag = stringStore->restore ( "master" );
    
    ASSERT_EQ ( tag.getVersion (), 1 );
    ASSERT_EQ ( tag.getValue < string >( KEY0 ), STR0 );
    
    printf ( "OK!\n" );
}

//----------------------------------------------------------------//
TEST ( StringPersistence, test_branch_consolidation ) {

    shared_ptr < DebugStringStore > provider = make_shared < DebugStringStore >();
    
    VersionedStoreTag tag;
    
    tag.setValue < string >( "KEY", "VERSION 0" );
    tag.pushVersion ();
    provider->persist ( tag, "master" );
    tag.printTree ();
    provider->dump ();

    tag.setValue < string >( "KEY", "VERSION 1" );
    tag.pushVersion ();
    provider->persist ( tag, "master" );
    tag.printTree ();
    provider->dump ();

    VersionedStoreTag tag2 ( tag );

    tag.setValue < string >( "KEY", "VERSION 2" );
    tag.pushVersion ();
    provider->persist ( tag, "master" );
    tag.printTree ();
    provider->dump ();
}

//----------------------------------------------------------------//
TEST ( StringPersistence, test_persistence_immutability ) {

    shared_ptr < DebugStringStore > provider = make_shared < DebugStringStore >();
    
    VersionedStoreTag tag;
    
    tag.setValue < string >( "KEY", "VERSION 0" );
    tag.pushVersion ();
    provider->persist ( tag, "master" );
    tag.printTree ();
    provider->dump ();

    tag.setValue < string >( "KEY", "VERSION 1" );
    tag.pushVersion ();
    tag.printTree ();
    provider->dump ();
}

} // namespace Test
} // namespace Padamose
