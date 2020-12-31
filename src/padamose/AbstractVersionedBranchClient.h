// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_ABSTRACTVERSIONEDBRANCHCLIENT_H
#define PADAMOSE_ABSTRACTVERSIONEDBRANCHCLIENT_H

#include <padamose/padamose-common.h>

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
    implement in branch implementations and VersionedStoreSnapshot (which
    is the base for snapshots and iterators).
 
    In practice, it is easier and cleaner to implement a handful of
    single-line virtual methods than to complicate the client bookkeeping.
*/
class AbstractVersionedBranchClient {
protected:

    friend class AbstractVersionedBranch;
    friend class EphemeralVersionedBranch;

    /// The source (or parent) branch for this client. May be NULL.
    shared_ptr < AbstractVersionedBranch >      mSourceBranch;
    
    /// The current (or base) version for this client.
    size_t                                      mVersion;

    //----------------------------------------------------------------//
    bool                canJoin                                                 () const;
    size_t              getJoinScore                                            () const;
    size_t              getVersionDependency                                    () const;
    void                joinBranch                                              ( AbstractVersionedBranch& branch );
    bool                preventJoin                                             () const;

    //----------------------------------------------------------------//
    virtual bool        AbstractVersionedBranchClient_canJoin                   () const = 0;
    virtual size_t      AbstractVersionedBranchClient_getJoinScore              () const = 0;
    virtual size_t      AbstractVersionedBranchClient_getVersionDependency      () const = 0;
    virtual void        AbstractVersionedBranchClient_joinBranch                ( AbstractVersionedBranch& other ) = 0;
    virtual bool        AbstractVersionedBranchClient_preventJoin               () const = 0;
    virtual void        AbstractVersionedBranchClient_sourceBranchDidChange     () = 0;

public:

    //----------------------------------------------------------------//
                                        AbstractVersionedBranchClient           ();
    virtual                             ~AbstractVersionedBranchClient          ();
    size_t                              countBranches                           () const;
    const AbstractVersionedBranch*      getSourceBranch                         () const;
    void                                setBranch                               ( shared_ptr < AbstractVersionedBranch > branch );
    void                                setBranch                               ( shared_ptr < AbstractVersionedBranch > branch, size_t version );
};

} // namespace Padamose
#endif
