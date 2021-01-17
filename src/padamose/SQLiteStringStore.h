// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_SQLITESTRINGSTORE_H
#define PADAMOSE_SQLITESTRINGSTORE_H

#include <padamose/padamose-common.h>
#include <padamose/AbstractStringStore.h>
#include <padamose/SQLite.h>

namespace Padamose {

//================================================================//
// SQLiteStringStore
//================================================================//
// TODO: doxygen
class SQLiteStringStore :
    public AbstractStringStore {
protected:

    mutable SQLite  mDB;

    //----------------------------------------------------------------//
    void            open                    ( string filename, string prefix = "" );
                    
    //----------------------------------------------------------------//
    void            AbstractStringStore_eraseString     ( string key ) override;
    string          AbstractStringStore_getString       ( string key ) const override;
    bool            AbstractStringStore_hasString       ( string key ) const override;
    void            AbstractStringStore_setString       ( string key, string value ) override;

public:

    //----------------------------------------------------------------//
    operator bool () const {
        return ( bool )this->mDB;
    }

    //----------------------------------------------------------------//
    void            clear                   ();
                    SQLiteStringStore       ();
    virtual         ~SQLiteStringStore      ();
    
    //----------------------------------------------------------------//
    static shared_ptr < SQLiteStringStore > make ( string filename, string prefix = "" ) {
    
        shared_ptr < SQLiteStringStore > store = make_shared < SQLiteStringStore >();
        store->open ( filename, prefix );
        return store;
    }
};

} // namespace Padamose
#endif
