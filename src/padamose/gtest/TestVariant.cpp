// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <padamose/gtest/gtest-helpers.h>
#include <padamose/padamose.h>

namespace Padamose {
namespace Test {

//----------------------------------------------------------------//
TEST ( Variant, test_null_variant ) {

    Variant variant;

    ASSERT_TRUE ( variant.isNull ());
}

} // namespace Test
} // namespace Padamose
