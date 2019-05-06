// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_ABSTRACTVERSIONEDBRANCH_H
#define PADAMOSE_ABSTRACTVERSIONEDBRANCH_H

#include <padamose/padamose-common.h>
#include <padamose/AbstractVersionedBranchClient.h>
#include <padamose/Variant.h>

namespace Padamose {

class AbstractPersistentVersionedBranch;
class AbstractVersionedBranchClient;
class AbstractVersionedBranchTag;
class VersionedStoreSnapshot;

//================================================================//
// AbstractVersionedBranch
//================================================================//
/** \brief AbstractVersionedBranch is an internal data structure used to store
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
class AbstractVersionedBranch :
    public enable_shared_from_this < AbstractVersionedBranch >,
    public AbstractVersionedBranchClient {
protected:

    friend class AbstractVersionedBranchClient;
    friend class AbstractVersionedValueIterator;
    friend class VersionedStore;
    friend class VersionedStoreSnapshot;
    friend class VersionedStoreIterator;
    template < typename > friend class VersionedValue;
    template < typename > friend class VersionedValueIterator;

    /// Set containing active clients. This is needed to calculate dependencies and to know when to optimize.
    set < AbstractVersionedBranchClient* >      mClients;

    /// The number of clients holding direct references to branch internals. A nonzero direct reference count will prevent optimization of the branch.
    size_t                                      mDirectReferenceCount;

    //----------------------------------------------------------------//
    void            transferClients             ( AbstractVersionedBranch& other );
    void            truncate                    ( size_t topVersion );

    //----------------------------------------------------------------//
    bool            AbstractVersionedBranchClient_canJoin                   () const override;
    size_t          AbstractVersionedBranchClient_getJoinScore              () const override;
    size_t          AbstractVersionedBranchClient_getVersionDependency      () const override;
    bool            AbstractVersionedBranchClient_preventJoin               () const override;

    //----------------------------------------------------------------//
    virtual shared_ptr < AbstractVersionedBranch >      AbstractVersionedBranch_fork                    ( size_t baseVersion ) = 0;
    virtual size_t                                      AbstractVersionedBranch_getTopVersion           () const = 0;
    virtual size_t                                      AbstractVersionedBranch_getValueNextVersion     ( string key, size_t version ) const = 0;
    virtual size_t                                      AbstractVersionedBranch_getValuePrevVersion     ( string key, size_t version ) const = 0;
    virtual Variant                                     AbstractVersionedBranch_getValueVariant         ( size_t version, string key ) const = 0;
    virtual bool                                        AbstractVersionedBranch_getValueVersionExtents  ( string key, size_t upperBound, size_t& first, size_t& last ) const = 0;
    virtual bool                                        AbstractVersionedBranch_hasKey                  ( string key, size_t upperBound ) const = 0;
    virtual bool                                        AbstractVersionedBranch_isPersistent            () const = 0;
    virtual void                                        AbstractVersionedBranch_persist                 ( shared_ptr < AbstractPersistentVersionedBranch > persist ) = 0;
    virtual void                                        AbstractVersionedBranch_setValueVariant         ( size_t version, string key, const Variant& value ) = 0;
    virtual void                                        AbstractVersionedBranch_truncate                ( size_t topVersion ) = 0;

public:

    //----------------------------------------------------------------//
                                                AbstractVersionedBranch         ();
                                                ~AbstractVersionedBranch        ();
    size_t                                      countDependencies               () const;
    void                                        eraseClient                     ( AbstractVersionedBranchClient& client );
    size_t                                      findImmutableTop                ( const AbstractVersionedBranchClient* ignore = NULL ) const;
    shared_ptr < AbstractVersionedBranch >      fork                            ( size_t baseVersion );
    size_t                                      getDirectReferenceCount         () const;
    size_t                                      getTopVersion                   () const;
    size_t                                      getValueNextVersion             ( string key, size_t version ) const;
    size_t                                      getValuePrevVersion             ( string key, size_t version ) const;
    Variant                                     getValueVariant                 ( size_t version, string key ) const;
    bool                                        getValueVersionExtents          ( string key, size_t upperBound, size_t& first, size_t& last ) const;
    bool                                        hasKey                          ( size_t version, string key ) const;
    void                                        insertClient                    ( AbstractVersionedBranchClient& client );
    bool                                        isPersistent                    () const;
    void                                        optimize                        ();
    void                                        persistSelf                     ( shared_ptr < AbstractPersistenceProvider > provider );
    void                                        setValueVariant                 ( size_t version, string key, const Variant& value );
};

} // namespace Padamose
#endif
