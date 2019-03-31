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

    string                                      mBranchID;
    AbstractStringStorePersistenceProvider*     mStringStore;

    //----------------------------------------------------------------//
    string                          formatKeyForLayerIndexByMemberName      ( size_t version, string name ) const;
    string                          formatKeyForLayerMemberNameByIndex      ( size_t version, size_t index ) const;
    string                          formatKeyForLayerSize                   ( size_t version ) const;
    string                          formatKeyForLayerStackSize              () const;
    string                          formatKeyForValueByIndex                ( string key, size_t index ) const;
    string                          formatKeyForValueIndexByVersion         ( string key, size_t version ) const;
    string                          formatKeyForValueStackSize              ( string key ) const;
    string                          formatKeyForValueStackType              ( string key ) const;
    string                          formatKeyForValueVersionByIndex         ( string key, size_t index ) const;
    AbstractStringStore&            getStore                                ();
    const AbstractStringStore&      getStoreConst                           () const;

    //----------------------------------------------------------------//
    shared_ptr < AbstractVersionedBranch >      AbstractVersionedBranch_fork                            ( size_t baseVersion ) override;
    size_t                                      AbstractVersionedBranch_getTopVersion                   () const override;
    size_t                                      AbstractVersionedBranch_getValueNextVersion             ( string key, size_t version ) const override;
    size_t                                      AbstractVersionedBranch_getValuePrevVersion             ( string key, size_t version ) const override;
    Variant                                     AbstractVersionedBranch_getValueVariant                 ( size_t version, string key ) const override;
    bool                                        AbstractVersionedBranch_getValueVersionExtents          ( string key, size_t upperBound, size_t& first, size_t& last ) const override;
    bool                                        AbstractVersionedBranch_hasKey                          ( string key, size_t upperBound ) const override;
    void                                        AbstractVersionedBranch_optimize                        () override;
    void                                        AbstractVersionedBranch_persistSelf                     ( AbstractPersistenceProvider& provider ) override;
    void                                        AbstractVersionedBranch_setValueVariant                 ( size_t version, string key, const Variant& value ) override;
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
