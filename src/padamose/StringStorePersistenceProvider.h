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

    std::set < string >                 mBranchIDs;
    shared_ptr < AbstractStringStore >  mStore;

    //----------------------------------------------------------------//
    void                eraseBranchID                           ( string branchID );
    string              makeBranchID                            ();
                        StringStorePersistenceProvider          ();

    //----------------------------------------------------------------//
    shared_ptr < AbstractPersistentVersionedBranch >    AbstractPersistenceProvider_makePersistentBranch    () override;

public:

//    //----------------------------------------------------------------//
//    template < typename TYPE, typename ... ARGS >
//    shared_ptr < TYPE > setStore ( ARGS ... args ) {
//        shared_ptr < TYPE > store = make_shared < TYPE >( args ... );
//        this->mStore = store;
//        return store;
//    }

    //----------------------------------------------------------------//
                        StringStorePersistenceProvider          ( shared_ptr < AbstractStringStore > store );
    virtual             ~StringStorePersistenceProvider         ();
};

} // namespace Padamose
#endif
