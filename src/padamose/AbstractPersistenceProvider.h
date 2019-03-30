// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_ABSTRACTPERSISTENCEPROVIDER_H
#define PADAMOSE_ABSTRACTPERSISTENCEPROVIDER_H

#include <padamose/padamose-common.h>

namespace Padamose {

class AbstractVersionedBranch;

//================================================================//
// BranchTag
//================================================================//
// TODO: doxygen
class BranchTag {
public:

    shared_ptr < AbstractVersionedBranch >      mBranch;
    size_t                                      mVersion;
};

//================================================================//
// AbstractPersistenceProvider
//================================================================//
// TODO: doxygen
class AbstractPersistenceProvider {
protected:

    map < string, BranchTag > mTags;

    //----------------------------------------------------------------//
    virtual shared_ptr < AbstractVersionedBranch >  AbstractPersistenceProvider_makePersistentBranch    () = 0;
    
public:

    //----------------------------------------------------------------//
                                                    AbstractPersistenceProvider             ();
    virtual                                         ~AbstractPersistenceProvider            ();
    BranchTag&                                      getTag                                  ( string branchName );
    shared_ptr < AbstractVersionedBranch >          makePersistentBranch                    ();
    void                                            tagBranch                               ( AbstractVersionedBranch& branch, string branchName, size_t version );
};

} // namespace Padamose
#endif
