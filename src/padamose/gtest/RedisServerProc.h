// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_REDISSERVERPROC_H
#define PADAMOSE_REDISSERVERPROC_H

#include <padamose/padamose.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshorten-64-to-32"
#include <hiredis.h>
#pragma clang diagnostic pop

#include <reproc/reproc.h>
#include <signal.h>

namespace Padamose {
namespace Test {

static const char*              REDIS_HOSTNAME = "127.0.0.1";
static const int                REDIS_PORT = 9999;
static const struct timeval     REDIS_TIMEOUT = { 1, 500000 }; // 1.5 seconds

//================================================================//
// RedisServerProc
//================================================================//
class RedisServerProc {
private:

    bool            mIsRunning;
    reproc_type     mProcess;

public:

    //----------------------------------------------------------------//
    operator bool () const {
        return this->mIsRunning;
    }

    //----------------------------------------------------------------//
    RedisServerProc () :
        mIsRunning ( false ) {
    
        remove ( "./redis-test/dump.rdb" );
    
        // is redis already running on the debug port?
        redisContext* c = redisConnectWithTimeout ( REDIS_HOSTNAME, REDIS_PORT, REDIS_TIMEOUT );
        bool conflict = ( c->err == 0 );
        redisFree ( c );
        
        if ( !conflict ) {
        
            const char *argv [ 3 ] = { "redis-server", "./redis.conf", NULL };
            REPROC_ERROR reprocError = reproc_start ( &this->mProcess, 2, argv, "./redis-test" );

            if ( reprocError == REPROC_SUCCESS ) {
                while ( !this->mIsRunning ) {
                    redisContext* c = redisConnectWithTimeout ( REDIS_HOSTNAME, REDIS_PORT, REDIS_TIMEOUT );
                    this->mIsRunning = ( c->err == 0 );
                    redisFree ( c );
                }
            }
        }
    }
    
    //----------------------------------------------------------------//
    ~RedisServerProc () {
    
        REPROC_ERROR reprocError = reproc_terminate ( &this->mProcess );
        assert ( reprocError == REPROC_SUCCESS );
        
        reproc_wait ( &this->mProcess, REPROC_INFINITE, NULL );
        
        reproc_destroy ( &this->mProcess );
        remove ( "./redis-test/dump.rdb" );
    }
};

} // namespace Test
} // namespace Padamose
#endif
