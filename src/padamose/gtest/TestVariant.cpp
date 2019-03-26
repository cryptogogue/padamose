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
    ASSERT_TRUE ( v.get < bool >() == true );
    ASSERT_TRUE ( v.get < double >() == 1.0 );
    ASSERT_TRUE ( v.get < int >() == 1 );
    ASSERT_TRUE ( v.get < size_t >() == 1 );
    ASSERT_TRUE ( v.get < string >() == "true" );
    
    v = false;
    ASSERT_TRUE ( v.get < bool >() == false );
    ASSERT_TRUE ( v.get < double >() == 0.0 );
    ASSERT_TRUE ( v.get < int >() == 0 );
    ASSERT_TRUE ( v.get < size_t >() == 0 );
    ASSERT_TRUE ( v.get < string >() == "false" );
    
    v = 123.0;
    ASSERT_TRUE ( v.get < bool >() == true );
    ASSERT_TRUE ( v.get < double >() == 123.0 );
    ASSERT_TRUE ( v.get < int >() == 123 );
    ASSERT_TRUE ( v.get < size_t >() == 123 );
    ASSERT_TRUE ( v.get < string >() == "123" );
    
    v = 0.0;
    ASSERT_TRUE ( v.get < bool >() == false );
    ASSERT_TRUE ( v.get < double >() == 0.0 );
    ASSERT_TRUE ( v.get < int >() == 0 );
    ASSERT_TRUE ( v.get < size_t >() == 0 );
    ASSERT_TRUE ( v.get < string >() == "0" );
    
    v = 456;
    ASSERT_TRUE ( v.get < bool >() == true );
    ASSERT_TRUE ( v.get < double >() == 456.0 );
    ASSERT_TRUE ( v.get < int >() == 456 );
    ASSERT_TRUE ( v.get < size_t >() == 456 );
    ASSERT_TRUE ( v.get < string >() == "456" );
    
    v = 0;
    ASSERT_TRUE ( v.get < bool >() == false );
    ASSERT_TRUE ( v.get < double >() == 0.0 );
    ASSERT_TRUE ( v.get < int >() == 0 );
    ASSERT_TRUE ( v.get < size_t >() == 0 );
    ASSERT_TRUE ( v.get < string >() == "0" );
    
    v = "123";
    ASSERT_TRUE ( v.get < bool >() == true );
    ASSERT_TRUE ( v.get < double >() == 123.0 );
    ASSERT_TRUE ( v.get < int >() == 123 );
    ASSERT_TRUE ( v.get < size_t >() == 123 );
    ASSERT_TRUE ( v.get < string >() == "123" );
    
    v = string ( "123" );
    ASSERT_TRUE ( v.get < bool >() == true );
    ASSERT_TRUE ( v.get < double >() == 123.0 );
    ASSERT_TRUE ( v.get < int >() == 123 );
    ASSERT_TRUE ( v.get < size_t >() == 123 );
    ASSERT_TRUE ( v.get < string >() == "123" );
}

} // namespace Test
} // namespace Padamose
