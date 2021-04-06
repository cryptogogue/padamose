// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_VERSIONEDSTOREREF_H
#define PADAMOSE_VERSIONEDSTOREREF_H

#include <padamose/padamose-common.h>
#include <padamose/AbstractHasVersionedStoreRef.h>

namespace Padamose {

class AbstractVersionedBranch;
class AbstractVersionedStoreInspector;
class AbstractVersionedStoreTag;

//================================================================//
// VersionedStoreRef
//================================================================//
// TODO: doxygen
class VersionedStoreRef :
    public virtual AbstractHasVersionedStoreRef {
protected:

    friend class AbstractHasVersionedStoreRef;
    friend class AbstractVersionedStoreInspector;
    friend class AbstractVersionedStoreTag;
    friend class VersionedStoreInspector;
    friend class VersionedStoreLock;

    /// The source (or parent) branch for this client. May be NULL.
    mutable shared_ptr < AbstractVersionedBranch >  mSourceBranch;
    
    /// The current (or base) version for this client.
    size_t                                          mVersion;

    //----------------------------------------------------------------//
    void setBranch ( const VersionedStoreRef& other ) {
        this->mSourceBranch = other.mSourceBranch;
        this->mVersion = other.mVersion;
    }

    //----------------------------------------------------------------//
    void setBranch ( shared_ptr < AbstractVersionedBranch > branch, size_t version ) {
        this->mSourceBranch = branch;
        this->mVersion = version;
    }

    //----------------------------------------------------------------//
    VersionedStoreRef& AbstractHasVersionedStoreRef_getRef () override {
        return *this;
    }

    //----------------------------------------------------------------//
    const VersionedStoreRef& AbstractHasVersionedStoreRef_getRef () const override {
        return *this;
    }

public:
    
    //----------------------------------------------------------------//
    // TODO: doxygen
    operator bool () const {
        return ( this->mSourceBranch != NULL );
    }
    
    //----------------------------------------------------------------//
    VersionedStoreRef () :
        mVersion ( 0 ) {
    }
    
    //----------------------------------------------------------------//
    ~VersionedStoreRef () {
    }
};

} // namespace Padamose
#endif
