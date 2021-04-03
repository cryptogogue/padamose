// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <padamose/gtest/gtest-helpers.h>
#include <padamose/gtest/util.h>
#include <padamose/padamose.h>

namespace Padamose {
namespace Test {

static const string KEY0    = "test0";
static const string KEY1    = "test1";
static const string STR0    = "abc";
static const string STR1    = "def";
static const string STR2    = "ghi";
static const string STR3    = "jkl";
static const string STR4    = "mno";

//----------------------------------------------------------------//
TEST ( RedisPersistence, test_redis_persistence ) {

    RedisServerProc redisServerProc ( "./redis-test", "./redis.conf", "127.0.0.1", 9999 );
    ASSERT_TRUE ( redisServerProc.getStatus () == RedisServerProc::RUNNING_AS_CHILD );

    shared_ptr < RedisStringStore > stringStore = redisServerProc.makeStringStore ();
    shared_ptr < AbstractStringStore > provider = make_shared < AbstractStringStore >( stringStore );
    
    testWithProvider ( provider );
    
    // load provider from store
    provider = make_shared < AbstractStringStore >( stringStore );
    
    VersionedStoreTag store ( provider, "master" );

    ASSERT_EQ ( store.getVersion (), 3 );
    ASSERT_EQ ( store.getValue < string >( KEY0, 0 ), STR0 );
    ASSERT_EQ ( store.getValue < string >( KEY0, 1 ), STR1 );
    ASSERT_EQ ( store.getValue < string >( KEY0, 2 ), STR2 );
    ASSERT_EQ ( store.getValue < string >( KEY0, 3 ), STR3 );
}

} // namespace Test
} // namespace Padamose
