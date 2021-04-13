// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef HELPERS_H
#define HELPERS_H

#include <gtest/gtest.h>

#define ASSERT_EXCEPTION_CAUGHT(expr,exceptionType)     \
    try { expr; FAIL (); }                              \
    catch ( exceptionType ) {}                          \
    catch ( ... ) { FAIL (); }

static const char* KEY0     = "test0";
static const char* KEY1     = "test1";
static const char* STR0     = "abc";
static const char* STR1     = "def";
static const char* STR2     = "ghi";
static const char* STR3     = "jkl";
static const char* STR4     = "mno";

#endif
