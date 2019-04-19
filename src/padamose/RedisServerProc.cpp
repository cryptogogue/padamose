// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <padamose/RedisServerProc.h>
#include <padamose/RedisStringStore.h>

#include <padamose/padamose-hiredis.h>
#include <reproc/reproc.h>
#include <signal.h>

namespace Padamose {

//================================================================//
// RedisServerProc
//================================================================//

//----------------------------------------------------------------//
string RedisServerProc::getHostname () const {
    return this->mHostname;
}

//----------------------------------------------------------------//
int RedisServerProc::getPort () const {
    return this->mPort;
}

//----------------------------------------------------------------//
int RedisServerProc::getStatus () const {
    return this->mStatus;
}

//----------------------------------------------------------------//
shared_ptr < RedisStringStore > RedisServerProc::makeStringStore ( const struct timeval timeout ) const {

    assert ( this->mStatus != NOT_RUNNING ); // TODO: throw exception
    return make_shared < RedisStringStore >( this->mHostname, this->mPort, timeout );
}

//----------------------------------------------------------------//
RedisServerProc::RedisServerProc ( const char* workingDir, const char* pathToRedisConf, const char* hostname, int port, const struct timeval timeout ) :
    mProcess ( 0 ),
    mStatus ( NOT_RUNNING ) {
    
    this->mHostname = hostname;
    this->mPort = port;
    
    // is redis already running on the debug port?
    redisContext* c = redisConnectWithTimeout ( hostname, port, timeout );
    this->mStatus = ( c->err == 0 ) ? ALREADY_RUNNING : NOT_RUNNING;
    redisFree ( c );
    
    if ( this->mStatus == NOT_RUNNING ) {
        
        reproc_type process;
    
        const char *argv [ 3 ] = { "redis-server", pathToRedisConf, NULL };
        REPROC_ERROR reprocError = reproc_start ( &process, 2, argv, workingDir );

        if ( reprocError == REPROC_SUCCESS ) {
            while ( this->mStatus != RUNNING_AS_CHILD ) {
                redisContext* c = redisConnectWithTimeout ( hostname, port, timeout );
                this->mStatus = ( c->err == 0 ) ? RUNNING_AS_CHILD : NOT_RUNNING;
                redisFree ( c );
            }
        }
        
        this->mProcess = ( void* )new reproc_type ();
        *( reproc_type* )this->mProcess = process;
    }
}

//----------------------------------------------------------------//
RedisServerProc::~RedisServerProc () {

    if ( this->mStatus == RUNNING_AS_CHILD ) {

        assert ( this->mProcess );
        reproc_type* process = ( reproc_type* )this->mProcess;

        REPROC_ERROR reprocError = reproc_terminate ( process );
        assert ( reprocError == REPROC_SUCCESS );
        
        reproc_wait ( process, REPROC_INFINITE, NULL );
        reproc_destroy ( process );
        delete process;
    }
}

} // namespace Padamose
