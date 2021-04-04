// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_VERSIONEDSTORELOCK_H
#define PADAMOSE_VERSIONEDSTORELOCK_H

#include <padamose/VersionedStoreInspector.h>

namespace Padamose {

class AbstractPersistenceProvider;

//================================================================//
// VersionedStoreLock
//================================================================//
// TODO: doxygen
class VersionedStoreLock :
    public virtual VersionedStoreInspector {
protected:

    //----------------------------------------------------------------//
    void            VersionedStoreRef_setBranch     ( shared_ptr < AbstractVersionedBranch > branch, size_t version ) override;
    
public:

    //----------------------------------------------------------------//
                    VersionedStoreLock              ();
                    VersionedStoreLock              ( const VersionedStoreRef& other );
    virtual         ~VersionedStoreLock             ();
    
    //----------------------------------------------------------------//
    VersionedStoreLock& operator = ( const VersionedStoreRef& other ) {
        this->setBranch ( other );
        return *this;
    }
};

} // namespace Padamose
#endif
