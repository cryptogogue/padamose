// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <sys/stat.h>
#include <padamose/gtest/util.h>

namespace Padamose {
namespace Test {

//----------------------------------------------------------------//
bool exists ( cc8* filename ) {

    struct stat buffer;
    return ( stat ( filename, &buffer ) == 0) ;
}

} // namespace Test
} // namespace Padamose
