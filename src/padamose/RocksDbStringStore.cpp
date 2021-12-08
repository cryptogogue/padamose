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
        assert (this->mDB);
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

        rocksdb::Status status;

        if (hasTransaction()) {
            status = getLatestTransaction()->Delete(key);
        } else {
            status = mDB->Delete(rocksdb::WriteOptions(), key);
        }

        assert(status.ok());
    }

//----------------------------------------------------------------//
    string RocksDbStringStore::AbstractStringStore_getString(string key) const {
        assert (this->mDB);
        std::string value;
        rocksdb::Status status;
        if (hasTransaction()) {
            status = getLatestTransaction()->Get(rocksdb::ReadOptions(), key, &value);
        } else {
            status = mDB->Get(rocksdb::ReadOptions(), key, &value);
        }

        if (status.ok()) {
            return value;
        }

        return {};
    }

//----------------------------------------------------------------//
    bool RocksDbStringStore::AbstractStringStore_hasString(string key) const {
        assert (this->mDB);
        std::string value;
        rocksdb::Status status;
        if (hasTransaction()) {
            status = getLatestTransaction()->Get(rocksdb::ReadOptions(), key, &value);
        } else {
            status = mDB->Get(rocksdb::ReadOptions(), key, &value);
        }
        return !status.IsNotFound();
    }

//----------------------------------------------------------------//
    void RocksDbStringStore::AbstractStringStore_setString(string key, string value) {
        rocksdb::Status status;
        if (hasTransaction()) {
            status = getLatestTransaction()->Put(key, value);
            assert(status.ok());
        } else {
            assert(this->mDB);
            status = mDB->Put(rocksdb::WriteOptions(), key, value);
            assert(status.ok());
        }
    }

    void
    RocksDbStringStore::open(const string &filename, const string &configPath) {
        assert(!filename.empty());
        if (!mDB) {
            mOptions.create_if_missing = true;
            if (!configPath.empty()) {
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

    bool RocksDbStringStore::hasTransaction() const {
        return !transactions.empty();
    }

} // namespace Padamose
