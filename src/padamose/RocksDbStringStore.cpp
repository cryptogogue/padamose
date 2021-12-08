// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <padamose/RocksDbStringStore.h>

namespace Padamose {

    using namespace ROCKSDB_NAMESPACE;
//================================================================//
// RocksDbStringStore
//================================================================//

//----------------------------------------------------------------//
    RocksDbStringStore::RocksDbStringStore() :
            AbstractStringStore(""), mDB(nullptr) {
    }

//----------------------------------------------------------------//
    RocksDbStringStore::~RocksDbStringStore() {

        for (auto it: transactions) {
            delete it;
        }

        delete mDB;
    }

//================================================================//
// overrides
//================================================================//

//----------------------------------------------------------------//
    void RocksDbStringStore::AbstractPersistenceProvider_beginTransaction() {
        assert ( this->mDB );
        rocksdb::Transaction *txn = mDB->BeginTransaction(rocksdb::WriteOptions(), rocksdb::TransactionOptions());
        transactions.push_back(txn);
    }

//----------------------------------------------------------------//
    void RocksDbStringStore::AbstractPersistenceProvider_commitTransaction() {
        auto txn = transactions.back();
        if (txn) {
            txn->Commit();
            delete txn;
        }
        transactions.pop_back();
    }

//----------------------------------------------------------------//
    void RocksDbStringStore::AbstractStringStore_eraseString(string key) {
        auto s = getLatestTransaction()->Delete(key);
        assert(s.ok());
    }

//----------------------------------------------------------------//
    string RocksDbStringStore::AbstractStringStore_getString(string key) const {
        assert ( this->mDB );
        std::string value;
        rocksdb::Status s = mDB->Get(rocksdb::ReadOptions(), key, &value);
        if (s.ok()) {
            return value;
        }

        return {};
    }

//----------------------------------------------------------------//
    bool RocksDbStringStore::AbstractStringStore_hasString(string key) const {
        assert ( this->mDB );
        std::string value;
        auto s = mDB->Get(rocksdb::ReadOptions(), key, &value);
        return !s.IsNotFound();
    }

//----------------------------------------------------------------//
    void RocksDbStringStore::AbstractStringStore_setString(string key, string value) {
        auto s = getLatestTransaction()->Put(key, value);
        assert(s.ok());
    }

    void
    RocksDbStringStore::open(const string &filename, const string &configPath) {
        assert(!filename.empty());
        if (!mDB) {
            mOptions.create_if_missing = true;
            if(!configPath.empty()) {
                //TODO: load config here based on config path
            }
            Status status = TransactionDB::Open(mOptions, mTxnDbOptions, filename, &mDB);
            assert(status.ok());
            this->loadFromStore();
        }
    }

    rocksdb::Transaction *RocksDbStringStore::getLatestTransaction() const {
        assert(transactions.size() > 0);
        return transactions.back();
    }

} // namespace Padamose
