// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_VERSIONEDSTOREINSPECTOR_H
#define PADAMOSE_VERSIONEDSTOREINSPECTOR_H

#include <padamose/padamose-common.h>
#include <padamose/AbstractVersionedStoreInspector.h>
#include <padamose/Variant.h>
#include <padamose/VersionedStoreRef.h>

namespace Padamose {

//================================================================//
// VersionedStoreInspector
//================================================================//
// TODO: doxygen
class VersionedStoreInspector :
    public virtual AbstractVersionedStoreInspector,
    public virtual VersionedStoreRef {
protected:

    friend class VersionedStoreLock;

public:

    //----------------------------------------------------------------//
    VersionedStoreInspector () {
    }
    
    //----------------------------------------------------------------//
    VersionedStoreInspector ( VersionedStoreRef& source ) {
        this->mSourceBranch = source.mSourceBranch;
        this->mVersion = source.mVersion;
    }
    
    //----------------------------------------------------------------//
    ~VersionedStoreInspector () {
    }
};

} // namespace Padamose
#endif
