// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_ROCKSDBSTRINGSTORE_H
#define PADAMOSE_ROCKSDBSTRINGSTORE_H

#include <padamose/padamose-common.h>
#include <padamose/AbstractStringStore.h>
#include <rocksdb/db.h>
#include <rocksdb/utilities/transaction_db.h>
#include "rocksdb/utilities/transaction.h"

namespace Padamose {

//================================================================//
// RocksDbStringStore
//================================================================//
// TODO: doxygen
class RocksDbStringStore :
    public AbstractStringStore {
protected:

    rocksdb::TransactionDB* mDB{};
    rocksdb::Options mOptions{};
    rocksdb::TransactionDBOptions mTxnDbOptions{};
    vector< rocksdb::Transaction* > transactions{};

    //----------------------------------------------------------------//
    void            AbstractPersistenceProvider_beginTransaction        () override;
    void            AbstractPersistenceProvider_commitTransaction       () override;
    void            AbstractStringStore_eraseString                     ( string key ) override;
    string          AbstractStringStore_getString                       ( string key ) const override;
    bool            AbstractStringStore_hasString                       ( string key ) const override;
    void            AbstractStringStore_setString                       ( string key, string value ) override;

public:

    //----------------------------------------------------------------//
    RocksDbStringStore();
    void open(const string& filename, const string &configPath);
    rocksdb::Transaction * getLatestTransaction() const;
    virtual         ~RocksDbStringStore       ();
    //----------------------------------------------------------------//
    static shared_ptr < RocksDbStringStore > make (const string &filename, const string &configPath) {
        shared_ptr < RocksDbStringStore > store = make_shared < RocksDbStringStore >();
        store->open ( filename, configPath);
        return store;
    }
};

} // namespace Padamose
#endif
