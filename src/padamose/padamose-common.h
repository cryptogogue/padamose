// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_COMMON_H
#define PADAMOSE_COMMON_H

#include <array>
#include <assert.h>
#include <cmath>
#include <exception>
#include <fstream>
#include <functional>
#include <inttypes.h>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <random>
#include <set>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <typeinfo>
#include <variant>
#include <vector>

#include <lognosis/Logger.h>

using namespace std;

#include <padamose/Exceptions.h>

#define PDM_FILTER_ROOT             " @PDM           "
#define PDM_FILTER_TREE             " @PDM_TREE      "
#define PDM_FILTER_SQLITE           " @SQLITE        "
#define PDM_FILTER_SQLSTORE         " @PDM_SQLSTORE  "
#define PDM_FILTER_STRSTORE         " @PDM_STRSTORE  "

#if !defined(UNUSED)
    #define UNUSED(x) ((void)(x))
#endif

//----------------------------------------------------------------//
typedef const char              cc8;

typedef uint8_t                 u8;
typedef uint16_t                u16;
typedef uint32_t                u32;
typedef uint64_t                u64;

typedef int8_t                  s8;
typedef int16_t                 s16;
typedef int32_t                 s32;
typedef int64_t                 s64;

#endif
