// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_VERSIONEDSTORELOCK_H
#define PADAMOSE_VERSIONEDSTORELOCK_H

#include <padamose/HasVersionedBranch.h>

namespace Padamose {

class AbstractPersistenceProvider;

//================================================================//
// VersionedStoreLock
//================================================================//
// TODO: doxygen
class VersionedStoreLock :
    public virtual HasVersionedBranch {
    
    //----------------------------------------------------------------//
    void                        lock                            ( shared_ptr < AbstractVersionedBranch > branch, size_t version );
    
public:

    //----------------------------------------------------------------//
    void                        lock                            ( const HasVersionedBranch& other );
    void                        unlock                          ();
                                VersionedStoreLock              ();
                                VersionedStoreLock              ( const HasVersionedBranch& other );
    virtual                     ~VersionedStoreLock             ();
    
    //----------------------------------------------------------------//
    VersionedStoreLock& operator = ( HasVersionedBranch& other ) {
        this->lock ( other.getSourceBranch (), other.getVersion ());
        return *this;
    }
};

} // namespace Padamose
#endif
