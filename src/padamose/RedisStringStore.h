// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_REDISSTRINGSTORE_H
#define PADAMOSE_REDISSTRINGSTORE_H

#include <padamose/padamose-common.h>
#include <padamose/AbstractStringStore.h>

namespace Padamose {

//================================================================//
// RedisStringStore
//================================================================//
// TODO: doxygen
class RedisStringStore :
    public AbstractStringStore {
protected:

    void*           mContext;

    //----------------------------------------------------------------//
    void            AbstractStringStore_eraseString     ( string key ) override;
    string          AbstractStringStore_getString       ( string key ) const override;
    bool            AbstractStringStore_hasString       ( string key ) const override;
    void            AbstractStringStore_setString       ( string key, string value ) override;

public:

    //----------------------------------------------------------------//
    bool            isConnected             ();
                    RedisStringStore        ( string host, int port, string prefix = "", const struct timeval timeout = { 1, 500000 });
    virtual         ~RedisStringStore       ();
};

} // namespace Padamose
#endif
