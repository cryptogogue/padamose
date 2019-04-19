// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_REDISSERVERPROC_H
#define PADAMOSE_REDISSERVERPROC_H

#include <padamose/padamose-common.h>

namespace Padamose {

class RedisStringStore;

//================================================================//
// RedisServerProc
//================================================================//
class RedisServerProc {
private:

    string          mHostname;
    int             mPort;

    void*           mProcess;
    int             mStatus;

public:

    enum {
        NOT_RUNNING,
        ALREADY_RUNNING,
        RUNNING_AS_CHILD,
    };

    //----------------------------------------------------------------//
    string                              getHostname         () const;
    int                                 getPort             () const;
    int                                 getStatus           () const;
    shared_ptr < RedisStringStore >     makeStringStore     ( const struct timeval timeout = { 1, 500000 }) const;
                                        RedisServerProc     ( const char* workingDir, const char* pathToRedisConf, const char* hostname, int port, const struct timeval timeout = { 1, 500000 });
                                        ~RedisServerProc    ();
};

} // namespace Padamose
#endif
