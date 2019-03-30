// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_STRINGSTOREPERSISTENCEPROVIDER_H
#define PADAMOSE_STRINGSTOREPERSISTENCEPROVIDER_H

#include <padamose/padamose-common.h>
#include <padamose/AbstractPersistenceProvider.h>
#include <padamose/AbstractStringStore.h>

namespace Padamose {

//================================================================//
// StringStorePersistenceProvider
//================================================================//
// TODO: doxygen
class StringStorePersistenceProvider :
    public AbstractPersistenceProvider {
protected:

    friend class StringStoreVersionedBranch;

    set < string >                          mBranchIDs;
    unique_ptr < AbstractStringStore >      mStore;

    //----------------------------------------------------------------//
    void                eraseBranchID                           ( string branchID );
    string              makeBranchID                            ();

    //----------------------------------------------------------------//
    shared_ptr < AbstractVersionedBranch >  AbstractPersistenceProvider_makePersistentBranch    () override;

public:

    //----------------------------------------------------------------//
                        StringStorePersistenceProvider          ();
    virtual             ~StringStorePersistenceProvider         ();
    
    //----------------------------------------------------------------//
    template < typename TYPE >
    void setStore () {
        this->mStore = make_unique < TYPE >();
    }
};

} // namespace Padamose
#endif
