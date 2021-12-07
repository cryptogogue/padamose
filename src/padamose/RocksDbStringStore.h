// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_DEBUGSTRINGSTORE_H
#define PADAMOSE_DEBUGSTRINGSTORE_H

#include <padamose/padamose-common.h>
#include <padamose/AbstractStringStore.h>
#include <rocksdb/db.h>
#include <rocksdb/utilities/transaction_db.h>

namespace Padamose {

//================================================================//
// DebugStringStore
//================================================================//
// TODO: doxygen
class RocksDbStringStore :
    public AbstractStringStore {
protected:

    rocksdb::TransactionDB* db;
    rocksdb::Options mOptions;
    rocksdb::TransactionDBOptions mTxnDbOptions;

    //----------------------------------------------------------------//
    void            AbstractPersistenceProvider_beginTransaction        () override;
    void            AbstractPersistenceProvider_commitTransaction       () override;
    void            AbstractStringStore_eraseString                     ( string key ) override;
    string          AbstractStringStore_getString                       ( string key ) const override;
    bool            AbstractStringStore_hasString                       ( string key ) const override;
    void            AbstractStringStore_setString                       ( string key, string value ) override;

public:

    //----------------------------------------------------------------//
    RocksDbStringStore        ( string prefix = "" );
    void open(const string& filename, rocksdb::Options options = {}, const rocksdb::TransactionDBOptions& txnDbOptions = {});
    virtual         ~RocksDbStringStore       ();
};

} // namespace Padamose
#endif
