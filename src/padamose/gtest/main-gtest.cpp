// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <gtest/gtest.h>
#include <padamose/padamose-common.h>

//----------------------------------------------------------------//
int main ( int argc, char **argv ) {

    Lognosis::setFilter ( PDM_FILTER_ROOT, Lognosis::MAX );
    Lognosis::init ( argc, argv );

    ::testing::InitGoogleTest ( &argc, argv );
    int result = RUN_ALL_TESTS ();
    return result;
}
