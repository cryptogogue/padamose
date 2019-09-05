// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_HIREDIS_H
#define PADAMOSE_HIREDIS_H

#if __clang__
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wshorten-64-to-32"
    #include <hiredis.h>
    #pragma clang diagnostic pop
#endif

#if __GNUC__
//    #pragma GCC diagnostic push
//    #pragma GCC diagnostic ignored "-Wshorten-64-to-32"
    #include <hiredis.h>
//    #pragma GCC diagnostic pop
#endif

#endif
