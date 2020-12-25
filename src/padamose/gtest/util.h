// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_GTEST_UTIL_H
#define PADAMOSE_GTEST_UTIL_H

#include <padamose/gtest/gtest-helpers.h>
#include <padamose/padamose.h>

namespace Padamose {
namespace Test {

//----------------------------------------------------------------//
bool    exists              ( cc8* filename );
void    testWithProvider    ( shared_ptr < AbstractPersistenceProvider > provider );

} // namespace Test
} // namespace Padamose
#endif
