// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <padamose/RedisStringStore.h>
#include <padamose/padamose-hiredis.h>

namespace Padamose {

//================================================================//
// RedisStringStore
//================================================================//

//----------------------------------------------------------------//
bool RedisStringStore::isConnecred () {

    return ( this->mContext != NULL );
}

//----------------------------------------------------------------//
RedisStringStore::RedisStringStore ( string host, int port, const struct timeval timeout ) :
    mContext ( NULL ) {

    redisContext* c = redisConnectWithTimeout ( host.c_str (), port, timeout );
    if ( c->err == 0 ) {
        this->mContext = c;
    }
}

//----------------------------------------------------------------//
RedisStringStore::~RedisStringStore () {

    redisContext* c = ( redisContext* )this->mContext;
    if ( c ) {
        redisFree ( c );
    }
}

//================================================================//
// overrides
//================================================================//

//----------------------------------------------------------------//
void RedisStringStore::AbstractStringStore_eraseString ( string key ) {

    redisContext* c = ( redisContext* )this->mContext;
    assert ( c );
    
    redisReply* reply = ( redisReply* )redisCommand ( c, "DEL %s", key.c_str ());
    freeReplyObject ( reply );
}

//----------------------------------------------------------------//
string RedisStringStore::AbstractStringStore_getString ( string key ) const {

    redisContext* c = ( redisContext* )this->mContext;
    assert ( c );
    
    string result;
    redisReply* reply = ( redisReply* )redisCommand ( c, "GET %s", key.c_str ());
    if ( reply->type == REDIS_REPLY_STRING ) {
        result = reply->str;
    }
    freeReplyObject ( reply );
    
    return result;
}

//----------------------------------------------------------------//
bool RedisStringStore::AbstractStringStore_hasString ( string key ) const {

    redisContext* c = ( redisContext* )this->mContext;
    assert ( c );

    bool result = false;
    redisReply* reply = ( redisReply* )redisCommand ( c, "EXISTS %s", key.c_str ());
    if ( reply->type == REDIS_REPLY_INTEGER ) {
        result = ( reply->integer > 0 );
    }
    freeReplyObject ( reply );

    return result;
}

//----------------------------------------------------------------//
void RedisStringStore::AbstractStringStore_setString ( string key, string value ) {

    redisContext* c = ( redisContext* )this->mContext;
    assert ( c );

    redisReply* reply = ( redisReply* )redisCommand ( c, "SET %s %s", key.c_str (), value.c_str ());
    assert ( strcmp ( reply->str, "OK" ) == 0 );
    freeReplyObject ( reply );
}

} // namespace Padamose
