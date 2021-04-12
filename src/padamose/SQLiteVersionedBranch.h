// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_SQLITEVERSIONEDBRANCH_H
#define PADAMOSE_SQLITEVERSIONEDBRANCH_H

#include <padamose/padamose-common.h>
#include <padamose/AbstractPersistentVersionedBranch.h>
#include <padamose/SQLitePersistenceProvider.h>

namespace Padamose {

//================================================================//
// SQLiteVersionedBranch
//================================================================//
// TODO: doxygen
class SQLiteVersionedBranch :
    public AbstractPersistentVersionedBranch {
private:

    friend class SQLitePersistenceProvider;

    static const size_t INVALID_LAYER_INDEX             = ( size_t )-1;
    static const size_t INVALID_VERSION                 = ( size_t )-1;

    u64                                         mBranchID;
    u64                                         mTopVersion;
    shared_ptr < SQLitePersistenceProvider >    mProvider;

    //----------------------------------------------------------------//
    SQLite&             getDB                                       () const;
    Variant             getValueVariantForVersion                   ( string key, size_t version ) const;
    void                loadFromStore                               ();
    void                setTopVersion                               ( u64 topVersion );

    //----------------------------------------------------------------//
    ConstProviderPtr    AbstractPersistentVersionedBranch_getProvider           () const override;
    void                AbstractVersionedBranch_begin                           () override;
    void                AbstractVersionedBranch_commit                          () override;
    BranchPtr           AbstractVersionedBranch_fork                            ( size_t baseVersion ) override;
    size_t              AbstractVersionedBranch_getTopVersion                   () const override;
    size_t              AbstractVersionedBranch_getValueNextVersion             ( string key, size_t version ) const override;
    size_t              AbstractVersionedBranch_getValuePrevVersion             ( string key, size_t version ) const override;
    Variant             AbstractVersionedBranch_getValueVariant                 ( size_t version, string key ) const override;
    bool                AbstractVersionedBranch_getValueVersionExtents          ( string key, size_t upperBound, size_t& first, size_t& last ) const override;
    bool                AbstractVersionedBranch_hasKey                          ( string key, size_t upperBound ) const override;
    bool                AbstractVersionedBranch_isPersistent                    () const override;
    void                AbstractVersionedBranch_joinBranch                      ( AbstractVersionedBranch& other ) override;
    void                AbstractVersionedBranch_persist                         ( shared_ptr < AbstractPersistentVersionedBranch > persist ) override;
    void                AbstractVersionedBranch_print                           ( string prefix ) const override;
    void                AbstractVersionedBranch_setValueVariant                 ( size_t version, string key, const Variant& value ) override;
    void                AbstractVersionedBranch_truncate                        ( size_t topVersion ) override;
    void                AbstractVersionedBranchClient_sourceBranchDidChange     () override;

public:

    //----------------------------------------------------------------//
                        SQLiteVersionedBranch                  ( shared_ptr < SQLitePersistenceProvider > provider, u64 branchID );
                        SQLiteVersionedBranch                  ( shared_ptr < SQLitePersistenceProvider > provider, AbstractVersionedBranch& from );
                        ~SQLiteVersionedBranch                 ();
};

} // namespace Padamose
#endif
