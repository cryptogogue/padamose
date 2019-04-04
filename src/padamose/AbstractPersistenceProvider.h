// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_ABSTRACTPERSISTENCEPROVIDER_H
#define PADAMOSE_ABSTRACTPERSISTENCEPROVIDER_H

#include <padamose/padamose-common.h>
#include <padamose/VersionedStoreSnapshot.h>

namespace Padamose {

class AbstractVersionedBranch;

//================================================================//
// AbstractPersistenceProvider
//================================================================//
// TODO: doxygen
class AbstractPersistenceProvider {
protected:

    map < string, VersionedStoreSnapshot > mTags;

    //----------------------------------------------------------------//
    virtual shared_ptr < AbstractVersionedBranch >  AbstractPersistenceProvider_makePersistentBranch    () = 0;
    
public:

    //----------------------------------------------------------------//
                                                    AbstractPersistenceProvider             ();
    virtual                                         ~AbstractPersistenceProvider            ();
    const VersionedStoreSnapshot&                   getTag                                  ( string branchName );
    shared_ptr < AbstractVersionedBranch >          makePersistentBranch                    ();
    void                                            tagBranch                               ( AbstractVersionedBranch& branch, string branchName, size_t version );
};

} // namespace Padamose
#endif
