// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_HASVERSIONEDREF_H
#define PADAMOSE_HASVERSIONEDREF_H

#include <padamose/padamose-common.h>
#include <padamose/AbstractHasVersionedBranch.h>

namespace Padamose {

class AbstractVersionedBranch;
class AbstractVersionedStoreTag;

//================================================================//
// HasVersionedBranch
//================================================================//
// TODO: doxygen
class HasVersionedBranch :
    public virtual AbstractHasVersionedBranch {
protected:

    friend class AbstractHasVersionedBranch;
    friend class AbstractVersionedStoreTag;
    friend class VersionedStoreInspector;
    friend class VersionedStoreLock;

    /// The source (or parent) branch for this client. May be NULL.
    mutable shared_ptr < AbstractVersionedBranch >  mSourceBranch;
    
    /// The current (or base) version for this client.
    size_t                                          mVersion;

    //----------------------------------------------------------------//
    void setBranch ( const AbstractHasVersionedBranch& other ) {
        this->mSourceBranch     = other.getSourceBranch ();
        this->mVersion          = other.getVersion ();
    }

    //----------------------------------------------------------------//
    void setBranch ( shared_ptr < AbstractVersionedBranch > branch, size_t version ) {
        this->mSourceBranch     = branch;
        this->mVersion          = version;
    }

    //----------------------------------------------------------------//
    HasVersionedBranch& AbstractHasVersionedBranch_getRef () override {
        return *this;
    }

    //----------------------------------------------------------------//
    const HasVersionedBranch& AbstractHasVersionedBranch_getRef () const override {
        return *this;
    }

public:
    
    //----------------------------------------------------------------//
    // TODO: doxygen
    operator bool () const {
        return ( this->mSourceBranch != NULL );
    }
    
    //----------------------------------------------------------------//
    HasVersionedBranch () :
        mVersion ( 0 ) {
    }
    
    //----------------------------------------------------------------//
    ~HasVersionedBranch () {
    }
};

} // namespace Padamose
#endif
