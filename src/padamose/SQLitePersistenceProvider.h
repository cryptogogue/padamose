// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_SQLITEPERSISTENCEPROVIDER_H
#define PADAMOSE_SQLITEPERSISTENCEPROVIDER_H

#include <padamose/padamose-common.h>
#include <padamose/AbstractPersistenceProvider.h>
#include <padamose/SQLite.h>

namespace Padamose {

class SQLiteVersionedBranch;

//================================================================//
// SQLitePersistenceProvider
//================================================================//
// TODO: doxygen
class SQLitePersistenceProvider :
    public AbstractPersistenceProvider,
    public enable_shared_from_this < SQLitePersistenceProvider > {
protected:

    friend class SQLiteVersionedBranch;

    static const size_t INVALID_VERSION                 = ( size_t )-1;

    mutable SQLite  mDB;

    std::map < u64, weak_ptr < SQLiteVersionedBranch >>     mBranchesByID;
    std::map < const AbstractVersionedBranch*, u64 >        mIDsByBranch;
    
    //----------------------------------------------------------------//
    shared_ptr < SQLiteVersionedBranch >            affirmBranch                            ( u64 branchID );
    void                                            eraseBranch                             ( SQLiteVersionedBranch& branch );
    u64                                             getIDForBranch                          ( const AbstractVersionedBranch& branch ) const;
    void                                            insertBranch                            ( shared_ptr < SQLiteVersionedBranch > branch );
    void                                            loadFromStore                           ();

    //----------------------------------------------------------------//
    void                                                AbstractPersistenceProvider_begin                   () override;
    void                                                AbstractPersistenceProvider_commit                  () override;
    shared_ptr < AbstractPersistentVersionedBranch >    AbstractPersistenceProvider_makePersistentBranch    ( AbstractVersionedBranch& from ) override;
    void                                                AbstractPersistenceProvider_tagDidChange            ( string name, const VersionedStoreTag* snapshot ) override;

    //----------------------------------------------------------------//
    template < typename TYPE >
    TYPE get ( string key, const TYPE value ) const {
        return value;
    }
    
    //----------------------------------------------------------------//
    template < typename TYPE >
    void set ( string key, const TYPE value ) {
    }

public:

    //----------------------------------------------------------------//
    operator bool () const {
        return ( bool )this->mDB;
    }

    //----------------------------------------------------------------//
    void                open                            ( string filename, int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE );
                        SQLitePersistenceProvider       ();
    virtual             ~SQLitePersistenceProvider      ();

    //----------------------------------------------------------------//
    static shared_ptr < SQLitePersistenceProvider > make ( string filename, int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE ) {
    
        shared_ptr < SQLitePersistenceProvider > store = make_shared < SQLitePersistenceProvider >();
        store->open ( filename, flags );
        return store;
    }
};

} // namespace Padamose
#endif
