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
    implement in branch implementations and ConstVersionedStoreTag (which
    is the base for snapshots and iterators).
 
    In practice, it is easier and cleaner to implement a handful of
    single-line virtual methods than to complicate the client bookkeeping.
*/
class AbstractVersionedBranchClient :
    public enable_shared_from_this < AbstractVersionedBranch > {
public:

    typedef shared_ptr < AbstractVersionedBranch >          BranchPtr;
    typedef shared_ptr < const AbstractVersionedBranch >    ConstBranchPtr;

protected:

    friend class AbstractVersionedBranch;
    friend class EphemeralVersionedBranch;
    friend class VersionedStoreLock;

    /// The source (or parent) branch for this client. May be NULL.
    shared_ptr < AbstractVersionedBranch >      mSourceBranch;
    
    /// The current (or base) version for this client.
    size_t                                      mVersion;

    //----------------------------------------------------------------//
    BranchPtr           asBranch                                ();
    size_t              getVersionDependency                    () const;

    //----------------------------------------------------------------//
    virtual BranchPtr   AbstractVersionedBranchClient_asBranch                  ();
    virtual size_t      AbstractVersionedBranchClient_getVersionDependency      () const;
    virtual void        AbstractVersionedBranchClient_print                     ( string prefix ) const;
    virtual void        AbstractVersionedBranchClient_sourceBranchDidChange     ();

public:

    //----------------------------------------------------------------//
                        AbstractVersionedBranchClient           ();
    virtual             ~AbstractVersionedBranchClient          ();
    size_t              countBranches                           () const;
    BranchPtr           getSourceBranch                         ();
    ConstBranchPtr      getSourceBranch                         () const;
    size_t              getVersion                              () const;
    void                printTree                               () const;
    void                setBranch                               ( shared_ptr < AbstractVersionedBranch > branch );
    void                setBranch                               ( shared_ptr < AbstractVersionedBranch > branch, size_t version );
    void                takeSnapshot                            ( const AbstractVersionedBranchClient& other );
    
    //----------------------------------------------------------------//
    /** \brief  Implements assignment by calling takeSnapshot().
     
        \param  other   The version to snapshot.
    */
    AbstractVersionedBranchClient& operator = ( AbstractVersionedBranchClient& other ) {
        this->takeSnapshot ( other );
        return *this;
    }
};

} // namespace Padamose
#endif
