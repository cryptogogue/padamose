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
public:

    //----------------------------------------------------------------//
    void                        lock                            ( const HasVersionedBranch& other );
    void                        unlock                          ();
                                VersionedStoreLock              ();
                                VersionedStoreLock              ( const HasVersionedBranch& other );
    virtual                     ~VersionedStoreLock             ();
    
    //----------------------------------------------------------------//
    VersionedStoreLock& operator = ( HasVersionedBranch& other ) {
        this->lock ( other );
        return *this;
    }
};

} // namespace Padamose
#endif
