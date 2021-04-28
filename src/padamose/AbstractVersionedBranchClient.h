// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_ABSTRACTVERSIONEDBRANCHCLIENT_H
#define PADAMOSE_ABSTRACTVERSIONEDBRANCHCLIENT_H

#include <padamose/padamose-common.h>
#include <padamose/AbstractHasVersionedBranch.h>
#include <padamose/BranchInspector.h>
#include <padamose/HasVersionedBranch.h>

namespace Padamose {

class AbstractPersistenceProvider;
class AbstractVersionedBranch;

//================================================================//
// AbstractVersionedBranchClient
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
class AbstractVersionedBranchClient :
    public virtual HasVersionedBranch,
    public enable_shared_from_this < AbstractVersionedBranch > {
protected:

    friend class AbstractVersionedBranch;
    friend class EphemeralVersionedBranch;
    friend class VersionedStoreLock;
    
    string              mDebugName;
    
    //----------------------------------------------------------------//
    size_t              getVersionDependency                    () const;

    //----------------------------------------------------------------//
    virtual size_t      AbstractVersionedBranchClient_getVersionDependency      () const;
    virtual void        AbstractVersionedBranchClient_print                     ( string lgnFilter, string prefix ) const;
    virtual void        AbstractVersionedBranchClient_sourceBranchDidChange     ();

public:

    //----------------------------------------------------------------//
                        AbstractVersionedBranchClient           ();
    virtual             ~AbstractVersionedBranchClient          ();
    void                clear                                   ();
    size_t              countBranches                           () const;
    BranchInspector     getBranchInspector                      ( size_t depth );
    void                printTree                               ( string lgnFilter = PDM_FILTER_TREE ) const;
    void                setDebugName                            ( string debugName );
    void                setParent                               ( shared_ptr < AbstractVersionedBranch > branch, size_t version );
};

} // namespace Padamose
#endif
