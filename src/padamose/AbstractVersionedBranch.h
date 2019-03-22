// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_ABSTRACTVERSIONEDBRANCH_H
#define PADAMOSE_ABSTRACTVERSIONEDBRANCH_H

#include <padamose/padamose-common.h>
#include <padamose/AbstractVersionedBranchClient.h>
#include <padamose/ValueStack.h>
#include <padamose/Variant.h>

namespace Padamose {

class AbstractVersionedBranchClient;
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
    set < AbstractVersionedBranchClient* >                  mClients;

    /// The number of clients holding direct references to branch internals. A nonzero direct reference count will prevent optimization of the branch.
    size_t                                                  mDirectReferenceCount;

    //----------------------------------------------------------------//
    bool            AbstractVersionedBranchClient_canJoin                   () const override;
    size_t          AbstractVersionedBranchClient_getJoinScore              () const override;
    Variant         AbstractVersionedBranchClient_getValue                  ( size_t version, string key );
    size_t          AbstractVersionedBranchClient_getVersionDependency      () const override;
    bool            AbstractVersionedBranchClient_preventJoin               () const override;
    void            AbstractVersionedBranchClient_setValue                  ( size_t version, string key, Variant& value );

    //----------------------------------------------------------------//
    virtual shared_ptr < AbstractVersionedBranch >      AbstractVersionedBranch_fork                ( size_t baseVersion ) = 0;
    virtual size_t                                      AbstractVersionedBranch_getTopVersion       () const = 0;
    virtual bool                                        AbstractVersionedBranch_hasKey              ( size_t version, string key ) const = 0;
    virtual void                                        AbstractVersionedBranch_optimize            () = 0;

public:

    //----------------------------------------------------------------//
                    AbstractVersionedBranch         ();
                    ~AbstractVersionedBranch        ();
    size_t          countDependencies               () const;
    void            eraseClient                     ( AbstractVersionedBranchClient& client );
    size_t          findImmutableTop                ( const AbstractVersionedBranchClient* ignore = NULL ) const;
    shared_ptr < AbstractVersionedBranch > fork     ( size_t baseVersion );
    size_t          getTopVersion                   () const;
    bool            hasKey                          ( size_t version, string key ) const;
    void            insertClient                    ( AbstractVersionedBranchClient& client );
    void            optimize                        ();
};

} // namespace Padamose
#endif
