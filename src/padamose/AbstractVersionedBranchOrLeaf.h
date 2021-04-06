// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_ABSTRACTVERSIONEDBRANCHORLEAF_H
#define PADAMOSE_ABSTRACTVERSIONEDBRANCHORLEAF_H

#include <padamose/padamose-common.h>
#include <padamose/AbstractHasVersionedStoreRef.h>
#include <padamose/VersionedStoreRef.h>

namespace Padamose {

class AbstractPersistenceProvider;
class AbstractVersionedBranch;

//================================================================//
// AbstractVersionedBranchOrLeaf
//================================================================//
/** \brief Base class for AbstractVersionedBranch clients. Branch client types
    include branches (parent-to-child) as well as snapshots and iterators.
 
    Implementing this functionality as a base class saves some redundant
    code in AbstractVersionedBranch at the expense of some extra methods to
    implement in branch implementations and VersionedStoreTag (which
    is the base for snapshots and iterators).
 
    In practice, it is easier and cleaner to implement a handful of
    single-line virtual methods than to complicate the client bookkeeping.
*/
class AbstractVersionedBranchOrLeaf :
    public virtual VersionedStoreRef,
    public enable_shared_from_this < AbstractVersionedBranch > {
protected:

    friend class AbstractVersionedBranch;
    friend class EphemeralVersionedBranch;
    friend class VersionedStoreLock;
    
    string              mDebugName;
    
    //----------------------------------------------------------------//
    BranchPtr           asBranch                                ();
    size_t              getVersionDependency                    () const;

    //----------------------------------------------------------------//
    virtual BranchPtr   AbstractVersionedBranchOrLeaf_asBranch                  ();
    virtual size_t      AbstractVersionedBranchOrLeaf_getVersionDependency      () const;
    virtual void        AbstractVersionedBranchOrLeaf_print                     ( string prefix ) const;
    virtual void        AbstractVersionedBranchOrLeaf_sourceBranchDidChange     ();

public:

    //----------------------------------------------------------------//
                        AbstractVersionedBranchOrLeaf           ();
    virtual             ~AbstractVersionedBranchOrLeaf          ();
    void                clear                                   ();
    size_t              countBranches                           () const;
    void                printTree                               () const;
    void                setDebugName                            ( string debugName );
    void                setParent                               ( shared_ptr < AbstractVersionedBranch > branch, size_t version );
};

} // namespace Padamose
#endif
