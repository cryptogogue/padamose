// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_VERSIONEDSTORELOCK_H
#define PADAMOSE_VERSIONEDSTORELOCK_H

#include <padamose/AbstractVersionedBranchClient.h>

namespace Padamose {

class AbstractPersistenceProvider;

//================================================================//
// VersionedStoreLock
//================================================================//
// TODO: doxygen
class VersionedStoreLock :
    public AbstractVersionedBranchClient {
protected:

    //----------------------------------------------------------------//
    void            lock                            ( const AbstractVersionedBranchClient& other );
    
    //----------------------------------------------------------------//
    bool            AbstractVersionedBranchClient_canJoin                   () const override;
    size_t          AbstractVersionedBranchClient_getJoinScore              () const override;
    size_t          AbstractVersionedBranchClient_getVersionDependency      () const override;
    void            AbstractVersionedBranchClient_joinBranch                ( AbstractVersionedBranch& branch ) override;
    bool            AbstractVersionedBranchClient_preventJoin               () const override;
    void            AbstractVersionedBranchClient_print                     ( string prefix ) const override;
    void            AbstractVersionedBranchClient_sourceBranchDidChange     () override;
    
public:

    //----------------------------------------------------------------//
    void            unlock                          ();
                    VersionedStoreLock              ();
                    VersionedStoreLock              ( const AbstractVersionedBranchClient& other );
    virtual         ~VersionedStoreLock             ();
    
    //----------------------------------------------------------------//
    VersionedStoreLock& operator = ( const AbstractVersionedBranchClient& other ) {
        this->lock ( other );
        return *this;
    }
};

} // namespace Padamose
#endif
