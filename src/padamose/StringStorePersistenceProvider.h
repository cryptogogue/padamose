// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_STRINGSTOREPERSISTENCEPROVIDER_H
#define PADAMOSE_STRINGSTOREPERSISTENCEPROVIDER_H

#include <padamose/padamose-common.h>
#include <padamose/AbstractPersistenceProvider.h>
#include <padamose/AbstractStringStore.h>

namespace Padamose {

class StringStoreVersionedBranch;

//================================================================//
// StringStorePersistenceProvider
//================================================================//
// TODO: doxygen
class StringStorePersistenceProvider :
    public AbstractPersistenceProvider {
protected:

    friend class StringStoreVersionedBranch;

    static const size_t INVALID_VERSION                 = ( size_t )-1;

    std::map < string, weak_ptr < StringStoreVersionedBranch >>     mBranchesByID;
    std::map < const AbstractVersionedBranch*, string >             mIDsByBranch;
    
    shared_ptr < AbstractStringStore >                  mStore;
    string                                              mPrefix;

    //----------------------------------------------------------------//
    shared_ptr < StringStoreVersionedBranch >       affirmBranch                            ( string branchID );
    void                                            eraseBranch                             ( StringStoreVersionedBranch& branch );
    string                                          formatKeyForBranchIDByTagName           ( string tagName ) const;
    string                                          formatKeyForTagListSize                 () const;
    string                                          formatKeyForTagNameByTagListIndex       ( size_t index ) const;
    string                                          formatKeyForVersionByTagName            ( string tagName ) const;
    string                                          getIDForBranch                          ( const AbstractVersionedBranch& branch ) const;
    void                                            insertBranch                            ( shared_ptr < StringStoreVersionedBranch > branch );
    void                                            loadFromStore                           ();
    string                                          makeBranchID                            () const;
                                                    StringStorePersistenceProvider          ();

    //----------------------------------------------------------------//
    shared_ptr < AbstractPersistentVersionedBranch >    AbstractPersistenceProvider_makePersistentBranch    () override;
    void                                                AbstractPersistenceProvider_tagDidChange            ( string name, const VersionedStoreSnapshot* snapshot ) override;

public:

//    //----------------------------------------------------------------//
//    template < typename TYPE, typename ... ARGS >
//    shared_ptr < TYPE > setStore ( ARGS ... args ) {
//        shared_ptr < TYPE > store = make_shared < TYPE >( args ... );
//        this->mStore = store;
//        return store;
//    }

    //----------------------------------------------------------------//
    string              getPrefix                               () const;
                        StringStorePersistenceProvider          ( shared_ptr < AbstractStringStore > store, string prefix = "" );
    virtual             ~StringStorePersistenceProvider         ();
};

} // namespace Padamose
#endif
