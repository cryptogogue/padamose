// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <padamose/gtest/gtest-helpers.h>
#include <padamose/padamose.h>

namespace Padamose {
namespace Test {

//----------------------------------------------------------------//
TEST ( Variant, test_null_variant ) {

    Variant v;

    ASSERT_TRUE ( v.isNull ());
}

//----------------------------------------------------------------//
TEST ( Variant, test_coercion ) {

    Variant v;

    ASSERT_TRUE ( v.isNull ());
    
    v = true;
    ASSERT_TRUE (( bool )v == true );
    ASSERT_TRUE (( double )v == 1.0 );
    ASSERT_TRUE (( int )v == 1 );
    ASSERT_TRUE (( string )v == "true" );
    
    v = false;
    ASSERT_TRUE (( bool )v == false );
    ASSERT_TRUE (( double )v == 0.0 );
    ASSERT_TRUE (( int )v == 0 );
    ASSERT_TRUE (( string )v == "false" );
    
    v = 123.0;
    ASSERT_TRUE (( bool )v == true );
    ASSERT_TRUE (( double )v == 123.0 );
    ASSERT_TRUE (( int )v == 123 );
    ASSERT_TRUE (( string )v == "123" );
    
    v = 0.0;
    ASSERT_TRUE (( bool )v == false );
    ASSERT_TRUE (( double )v == 0.0 );
    ASSERT_TRUE (( int )v == 0 );
    ASSERT_TRUE (( string )v == "0" );
    
    v = 456;
    ASSERT_TRUE (( bool )v == true );
    ASSERT_TRUE (( double )v == 456.0 );
    ASSERT_TRUE (( int )v == 456 );
    ASSERT_TRUE (( string )v == "456" );
    
    v = 0;
    ASSERT_TRUE (( bool )v == false );
    ASSERT_TRUE (( double )v == 0.0 );
    ASSERT_TRUE (( int )v == 0 );
    ASSERT_TRUE (( string )v == "0" );
    
    v = "123";
    ASSERT_TRUE (( bool )v == true );
    ASSERT_TRUE (( double )v == 123.0 );
    ASSERT_TRUE (( int )v == 123 );
    ASSERT_TRUE (( string )v == "123" );
    
    v = string ( "123" );
    ASSERT_TRUE (( bool )v == true );
    ASSERT_TRUE (( double )v == 123.0 );
    ASSERT_TRUE (( int )v == 123 );
    ASSERT_TRUE (( string )v == "123" );
}

} // namespace Test
} // namespace Padamose
