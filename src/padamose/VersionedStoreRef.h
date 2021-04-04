// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_VERSIONEDSTOREREF_H
#define PADAMOSE_VERSIONEDSTOREREF_H

#include <padamose/padamose-common.h>

namespace Padamose {

class AbstractVersionedBranch;

//================================================================//
// VersionedStoreRef
//================================================================//
// TODO: doxygen
class VersionedStoreRef {
public:

    typedef shared_ptr < AbstractVersionedBranch >          BranchPtr;
    typedef shared_ptr < const AbstractVersionedBranch >    ConstBranchPtr;
    
protected:

    /// The source (or parent) branch for this client. May be NULL.
    shared_ptr < AbstractVersionedBranch >      mSourceBranch;
    
    /// The current (or base) version for this client.
    size_t                                      mVersion;

    //----------------------------------------------------------------//
    virtual void    VersionedStoreRef_setBranch     ( shared_ptr < AbstractVersionedBranch > branch, size_t version );

public:
    
    //----------------------------------------------------------------//
    // TODO: doxygen
    operator bool () const {
        return ( this->mSourceBranch != NULL );
    }
    
    //----------------------------------------------------------------//
    void                clear                       ();
    BranchPtr           getSourceBranch             ();
    ConstBranchPtr      getSourceBranch             () const;
    size_t              getVersion                  () const;
    void                setBranch                   ( const VersionedStoreRef& other );
    void                setBranch                   ( shared_ptr < AbstractVersionedBranch > branch );
    void                setBranch                   ( shared_ptr < AbstractVersionedBranch > branch, size_t version );
                        VersionedStoreRef           ();
    virtual             ~VersionedStoreRef          ();
};

} // namespace Padamose
#endif
