// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <sys/stat.h>
#include <padamose/gtest/util.h>

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
bool exists ( cc8* filename ) {

    struct stat buffer;
    return ( stat ( filename, &buffer ) == 0) ;
}

//----------------------------------------------------------------//
void testWithProvider ( AbstractPersistenceProvider& provider ) {

    {
        VersionedStoreTag tag;

        tag.setValue < string >( KEY0, STR0 );
        tag.pushVersion (); // version 1
        ASSERT_EQ ( tag.getValue < string >( KEY0 ), STR0 );
        
        tag.setValue < string >( KEY0, STR1 );
        tag.pushVersion (); // version 2
        ASSERT_EQ ( tag.getValue < string >( KEY0 ), STR1 );
        
        provider.persist ( tag, "master" );
        ASSERT_EQ ( tag.getValue < string >( KEY0 ), STR1 );
        
        tag.setValue < string >( KEY0, STR2 );
        tag.pushVersion (); // version 3
        ASSERT_EQ ( tag.getValue < string >( KEY0 ), STR2 );
        
        provider.persist ( tag, "master" );
        ASSERT_EQ ( tag.getValue < string >( KEY0 ), STR2 );
        
        tag.setValue < string >( KEY0, STR3 );
        ASSERT_EQ ( tag.getVersion (), 3 );
        ASSERT_EQ ( tag.getValue < string >( KEY0 ), STR3 );
        
        provider.persist ( tag, "master" );
    }

    VersionedStoreTag tag = provider.restore ( "master" );

    ASSERT_EQ ( tag.getVersion (), 3 );
    ASSERT_EQ ( tag.getValue < string >( KEY0, 0 ), STR0 );
    ASSERT_EQ ( tag.getValue < string >( KEY0, 1 ), STR1 );
    ASSERT_EQ ( tag.getValue < string >( KEY0, 2 ), STR2 );
    ASSERT_EQ ( tag.getValue < string >( KEY0, 3 ), STR3 );
    
    // iterate by version
    VersionedStoreIterator tagIt ( tag );
    ASSERT_EQ ( tagIt.getValue < string >( KEY0 ), STR3 );
    
    tagIt.prev ();
    ASSERT_EQ ( tagIt.getValue < string >( KEY0 ), STR2 );
    
    tagIt.prev ();
    ASSERT_EQ ( tagIt.getValue < string >( KEY0 ), STR1 );
    
    tagIt.prev ();
    ASSERT_EQ ( tagIt.getValue < string >( KEY0 ), STR0 );
    
    tagIt.next ();
    ASSERT_EQ ( tagIt.getValue < string >( KEY0 ), STR1 );
    
    tagIt.next ();
    ASSERT_EQ ( tagIt.getValue < string >( KEY0 ), STR2 );
    
    tagIt.next ();
    ASSERT_EQ ( tagIt.getValue < string >( KEY0 ), STR3 );
    
    // iterate by value
    VersionedValueIterator < string > valueIt ( tag, KEY0 );
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

} // namespace Test
} // namespace Padamose
