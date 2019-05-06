// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_EPHEMERALVERSIONEDBRANCH_H
#define PADAMOSE_EPHEMERALVERSIONEDBRANCH_H

#include <padamose/padamose-common.h>
#include <padamose/AbstractVersionedBranch.h>
#include <padamose/AbstractVersionedBranchClient.h>
#include <padamose/EphemeralValueStack.h>

namespace Padamose {

class AbstractVersionedBranchClient;
class VersionedStoreSnapshot;

//================================================================//
// EphemeralVersionedBranch
//================================================================//
/** \brief EphemeralVersionedBranch is an internal data structure used to store
    a contiguous block of versioned values.
 
    Each branch is a sparse record of changes to the database over a
    span of versions. It contains a map of ValueStack instances referenced
    by key. In addition, a sparse stack of version layers (implemented as a map of
    string sets) is used to quickly identify the keys of values modified in that layer.
 
    The branch tracks a base version. Its "top" version is the highest version
    index in the layer stack. When a layer is removed, the keys of the values that were
    set in that layer are retrieved from the layer stack and used to erase the
    corresponding values from each value stack.
 
    Any branch may have a parent branch. When searching for values, the tree will
    be searched recurively until a value is found.
*/
class EphemeralVersionedBranch :
    public AbstractVersionedBranch {
private:

//    friend class AbstractVersionedBranchClient;
//    friend class AbstractVersionedValueIterator;
//    friend class VersionedStore;
//    friend class VersionedStoreSnapshot;
//    friend class VersionedStoreIterator;
//    template < typename > friend class VersionedValue;
//    template < typename > friend class VersionedValueIterator;

    typedef set < string > Layer;
    
    /// Sparse array mapping versions onto layers. Each layer holds the set of keys corresponding to values that were set or modified in the version.
    map < size_t, Layer >                                   mLayers;
    
    /// Map of value stacks, indexed by key.
    map < string, unique_ptr < EphemeralValueStack >>       mValueStacksByKey;

    //----------------------------------------------------------------//
    void                            copyValues                  ( AbstractVersionedBranch& other );
    const EphemeralValueStack*      findValueStack              ( string key ) const;

    //----------------------------------------------------------------//
    shared_ptr < AbstractVersionedBranch >      AbstractVersionedBranch_fork                            ( size_t baseVersion ) override;
    size_t                                      AbstractVersionedBranch_getTopVersion                   () const override;
    size_t                                      AbstractVersionedBranch_getValueNextVersion             ( string key, size_t version ) const override;
    size_t                                      AbstractVersionedBranch_getValuePrevVersion             ( string key, size_t version ) const override;
    Variant                                     AbstractVersionedBranch_getValueVariant                 ( size_t version, string key ) const override;
    bool                                        AbstractVersionedBranch_getValueVersionExtents          ( string key, size_t upperBound, size_t& first, size_t& last ) const override;
    bool                                        AbstractVersionedBranch_hasKey                          ( string key, size_t upperBound ) const override;
    bool                                        AbstractVersionedBranch_isPersistent                    () const override;
    void                                        AbstractVersionedBranch_persist                         ( shared_ptr < AbstractPersistentVersionedBranch > persist ) override;
    void                                        AbstractVersionedBranch_setValueVariant                 ( size_t version, string key, const Variant& value ) override;
    void                                        AbstractVersionedBranch_truncate                        ( size_t topVersion ) override;
    bool                                        AbstractVersionedBranchClient_canJoin                   () const override;
    size_t                                      AbstractVersionedBranchClient_getJoinScore              () const override;
    size_t                                      AbstractVersionedBranchClient_getVersionDependency      () const override;
    void                                        AbstractVersionedBranchClient_joinBranch                ( AbstractVersionedBranch& other ) override;
    bool                                        AbstractVersionedBranchClient_preventJoin               () const override;
    void                                        AbstractVersionedBranchClient_sourceBranchDidChange     () override;

public:

    //----------------------------------------------------------------//
                    EphemeralVersionedBranch        ();
                    ~EphemeralVersionedBranch       ();
};

} // namespace Padamose
#endif
