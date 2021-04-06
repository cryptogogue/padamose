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
    
    //----------------------------------------------------------------//
    void                        lock                            ( shared_ptr < AbstractVersionedBranch > branch, size_t version );
    
public:

    //----------------------------------------------------------------//
    void                        lock                            ( VersionedStoreRef& other );
//    VersionedStoreInspector     seek                            ( size_t version );
    void                        unlock                          ();
                                VersionedStoreLock              ();
                                VersionedStoreLock              ( VersionedStoreRef& other );
    virtual                     ~VersionedStoreLock             ();
    
    //----------------------------------------------------------------//
    VersionedStoreLock& operator = ( VersionedStoreRef& other ) {
        this->lock ( other.getSourceBranch (), other.getVersion ());
        return *this;
    }
};

} // namespace Padamose
#endif
