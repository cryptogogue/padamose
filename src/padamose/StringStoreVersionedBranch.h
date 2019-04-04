// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_STRINGSTOREVERSIONEDBRANCH_H
#define PADAMOSE_STRINGSTOREVERSIONEDBRANCH_H

#include <padamose/padamose-common.h>
#include <padamose/AbstractVersionedBranch.h>

namespace Padamose {

class AbstractVersionedBranchClient;
class StringStorePersistenceProvider;
class VersionedStoreSnapshot;

//================================================================//
// StringStoreVersionedBranch
//================================================================//
// TODO: doxygen
class StringStoreVersionedBranch :
    public AbstractVersionedBranch {
private:

    friend class AbstractStringStorePersistenceProvider;

    static const size_t INVALID_LAYER_INDEX             = ( size_t )-1;
    static const size_t INVALID_VERSION                 = ( size_t )-1;

    string                                      mBranchID;
    AbstractStringStorePersistenceProvider*     mStringStore;

    //----------------------------------------------------------------//
    string                          formatKeyForLayerSizeByVersion          ( size_t version ) const;
    string                          formatKeyForTopVersion                  () const;
    string                          formatKeyForValueByVersion              ( string key, size_t version ) const;
    string                          formatKeyForValueNameByIndexInLayer     ( size_t version, size_t indexInLayer ) const;
    string                          formatKeyForValueStackIndexByVersion    ( string key, size_t version ) const;
    string                          formatKeyForValueStackSize              ( string key ) const;
    string                          formatKeyForValueStackType              ( string key ) const;
    string                          formatKeyForValueVersionByStackIndex    ( string key, size_t stackIndex ) const;
    AbstractStringStore&            getStore                                ();
    const AbstractStringStore&      getStoreConst                           () const;
    Variant                         getValueVariantForVersion               ( string key, size_t version ) const;

    //----------------------------------------------------------------//
    shared_ptr < AbstractVersionedBranch >      AbstractVersionedBranch_fork                            ( size_t baseVersion ) override;
    size_t                                      AbstractVersionedBranch_getTopVersion                   () const override;
    size_t                                      AbstractVersionedBranch_getValueNextVersion             ( string key, size_t version ) const override;
    size_t                                      AbstractVersionedBranch_getValuePrevVersion             ( string key, size_t version ) const override;
    Variant                                     AbstractVersionedBranch_getValueVariant                 ( size_t version, string key ) const override;
    bool                                        AbstractVersionedBranch_getValueVersionExtents          ( string key, size_t upperBound, size_t& first, size_t& last ) const override;
    bool                                        AbstractVersionedBranch_hasKey                          ( string key, size_t upperBound ) const override;
    void                                        AbstractVersionedBranch_persistSelf                     ( AbstractPersistenceProvider& provider ) override;
    void                                        AbstractVersionedBranch_setValueVariant                 ( size_t version, string key, const Variant& value ) override;
    void                                        AbstractVersionedBranch_truncate                        ( size_t topVersion ) override;
    bool                                        AbstractVersionedBranchClient_canJoin                   () const override;
    size_t                                      AbstractVersionedBranchClient_getJoinScore              () const override;
    size_t                                      AbstractVersionedBranchClient_getVersionDependency      () const override;
    void                                        AbstractVersionedBranchClient_joinBranch                ( AbstractVersionedBranch& other ) override;
    bool                                        AbstractVersionedBranchClient_preventJoin               () const override;

public:

    //----------------------------------------------------------------//
                        StringStoreVersionedBranch          ();
                        ~StringStoreVersionedBranch         ();
};

} // namespace Padamose
#endif
