// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef HELPERS_H
#define HELPERS_H

#include <gtest/gtest.h>

#define ASSERT_EXCEPTION_CAUGHT(expr,exceptionType)     \
    try { expr; FAIL (); }                              \
    catch ( exceptionType ) {}                          \
    catch ( ... ) { FAIL (); }

#endif
