// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <padamose/gtest/gtest-helpers.h>
#include <padamose/padamose.h>
#include <padamose/padamose-hiredis.h>

namespace Padamose {
namespace Test {

//----------------------------------------------------------------//
TEST ( Hiredis, hello_redis ) {

    RedisServerProc redisServerProc ( "./redis-test", "./redis.conf", "127.0.0.1", 9999 );
    ASSERT_TRUE ( redisServerProc.getStatus () == RedisServerProc::RUNNING_AS_CHILD );

    redisContext* c = NULL;
    redisReply* reply;

    c = redisConnectWithTimeout ( redisServerProc.getHostname ().c_str (), redisServerProc.getPort (), { 1, 500000 });
    ASSERT_TRUE ( c && ( c->err == 0 ));

    reply = ( redisReply* )redisCommand ( c, "PING" );
    ASSERT_TRUE ( strcmp ( reply->str, "PONG" ) == 0 );
    freeReplyObject ( reply );

    reply = ( redisReply* )redisCommand ( c, "SET %s %s", "foo", "hello world" );
    ASSERT_TRUE ( strcmp ( reply->str, "OK" ) == 0 );
    freeReplyObject ( reply );

    reply = ( redisReply* )redisCommand ( c, "SET %b %b", "bar", 3, "hello", 5 );
    ASSERT_TRUE ( strcmp ( reply->str, "OK" ) == 0 );
    freeReplyObject ( reply );

    reply = ( redisReply* )redisCommand ( c, "GET foo" );
    ASSERT_TRUE ( strcmp ( reply->str, "hello world" ) == 0 );
    freeReplyObject ( reply );
    
    reply = ( redisReply* )redisCommand ( c, "GET bar" );
    ASSERT_TRUE ( strcmp ( reply->str, "hello" ) == 0 );
    freeReplyObject ( reply );

    reply = ( redisReply* )redisCommand ( c, "INCR counter" );
    ASSERT_TRUE ( reply->integer == 1 );
    freeReplyObject(reply);

    reply = ( redisReply* )redisCommand ( c, "INCR counter" );
    ASSERT_TRUE ( reply->integer == 2 );
    freeReplyObject ( reply );

    /* Create a list of numbers, from 0 to 9 */
    reply = ( redisReply* )redisCommand ( c, "DEL mylist" );
    freeReplyObject ( reply );
    for ( size_t i = 0; i < 10; i++ ) {
        char buf [ 64 ];

        snprintf ( buf, 64, "%u", ( unsigned int )i );
        reply = ( redisReply* )redisCommand ( c, "LPUSH mylist element-%s", buf );
        freeReplyObject ( reply );
    }

    /* Let's check what we have inside the list */
    reply = ( redisReply* )redisCommand ( c, "LRANGE mylist 0 -1" );
    if ( reply->type == REDIS_REPLY_ARRAY ) {
        for ( size_t i = 0; i < reply->elements; i++ ) {
            ostringstream stream;
            stream << "element-" << ( 9 - i );
            ASSERT_TRUE ( strcmp ( reply->element [ i ]->str, stream.str ().c_str ()) == 0 );
        }
    }
    freeReplyObject ( reply );

    /* Disconnects and frees the context */
    redisFree ( c );
}

} // namespace Test
} // namespace Padamose
