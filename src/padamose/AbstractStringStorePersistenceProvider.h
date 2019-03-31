// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_ABSTRACTSTRINGSTOREPERSISTENCEPROVIDER_H
#define PADAMOSE_ABSTRACTSTRINGSTOREPERSISTENCEPROVIDER_H

#include <padamose/padamose-common.h>
#include <padamose/AbstractPersistenceProvider.h>
#include <padamose/AbstractStringStore.h>

namespace Padamose {

//================================================================//
// AbstractStringStorePersistenceProvider
//================================================================//
// TODO: doxygen
class AbstractStringStorePersistenceProvider :
    public AbstractPersistenceProvider,
    public AbstractStringStore {
protected:

    friend class StringStoreVersionedBranch;

    std::set < string > mBranchIDs;

    //----------------------------------------------------------------//
    void                eraseBranchID                           ( string branchID );
    string              makeBranchID                            ();

    //----------------------------------------------------------------//
    shared_ptr < AbstractVersionedBranch >  AbstractPersistenceProvider_makePersistentBranch    () override;

public:

    //----------------------------------------------------------------//
                        AbstractStringStorePersistenceProvider          ();
    virtual             ~AbstractStringStorePersistenceProvider         ();
};

} // namespace Padamose
#endif
