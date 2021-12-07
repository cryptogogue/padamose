// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <padamose/RocksDbStringStore.h>

namespace Padamose {

    using namespace ROCKSDB_NAMESPACE;
//================================================================//
// DebugStringStore
//================================================================//

//----------------------------------------------------------------//
    RocksDbStringStore::RocksDbStringStore(string prefix) :
            AbstractStringStore(prefix) {
    }

//----------------------------------------------------------------//
    RocksDbStringStore::~RocksDbStringStore() {
        delete db;
    }

//================================================================//
// overrides
//================================================================//

//----------------------------------------------------------------//
    void RocksDbStringStore::AbstractPersistenceProvider_beginTransaction() {
    }

//----------------------------------------------------------------//
    void RocksDbStringStore::AbstractPersistenceProvider_commitTransaction() {
    }

//----------------------------------------------------------------//
    void RocksDbStringStore::AbstractStringStore_eraseString(string key) {
        rocksdb::Status s = db->Delete(rocksdb::WriteOptions(), key);
    }

//----------------------------------------------------------------//
    string RocksDbStringStore::AbstractStringStore_getString(string key) const {

        std::string value;
        rocksdb::Status s = db->Get(rocksdb::ReadOptions(), key, &value);
        if (s.ok()) {
            return value;
        }

        return {};
    }

//----------------------------------------------------------------//
    bool RocksDbStringStore::AbstractStringStore_hasString(string key) const {
        std::string value;
        rocksdb::Status s = db->Get(rocksdb::ReadOptions(), key, &value);
        return !s.IsNotFound();
    }

//----------------------------------------------------------------//
    void RocksDbStringStore::AbstractStringStore_setString(string key, string value) {
        rocksdb::Status s = db->Put(rocksdb::WriteOptions(), key, value);
        assert(!s.ok());
    }

    void
    RocksDbStringStore::open(const string &filename, rocksdb::Options options,
                             const rocksdb::TransactionDBOptions &txnDbOptions) {
        if (!db) {
            options.create_if_missing = true;
            mOptions = options;
            mTxnDbOptions = txnDbOptions;
            Status status =
                    TransactionDB::Open(mOptions, mTxnDbOptions, filename, &db);
            assert(status.ok());
            this->loadFromStore();
        }
    }

} // namespace Padamose
