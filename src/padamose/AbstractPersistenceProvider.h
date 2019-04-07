// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_ABSTRACTPERSISTENCEPROVIDER_H
#define PADAMOSE_ABSTRACTPERSISTENCEPROVIDER_H

#include <padamose/padamose-common.h>
#include <padamose/VersionedStoreSnapshot.h>

namespace Padamose {

class AbstractPersistentVersionedBranch;
class AbstractVersionedBranch;

//================================================================//
// AbstractPersistenceProvider
//================================================================//
// TODO: doxygen
class AbstractPersistenceProvider :
    public enable_shared_from_this < AbstractPersistenceProvider > {
protected:

    friend class AbstractPersistentVersionedBranch;
    friend class VersionedStoreSnapshot;

    map < string, VersionedStoreSnapshot >  mTags;
    bool                                    mIsFrozen;

    //----------------------------------------------------------------//
    void                                freeze          ();
    const VersionedStoreSnapshot&       getTag          ( string branchName ) const;
    
    //----------------------------------------------------------------//
    virtual shared_ptr < AbstractPersistentVersionedBranch >    AbstractPersistenceProvider_makePersistentBranch    () = 0;
    virtual void                                                AbstractPersistenceProvider_tagDidChange            ( string name, const VersionedStoreSnapshot* snapshot ) = 0;
    
public:

    //----------------------------------------------------------------//
                                                        AbstractPersistenceProvider             ();
    virtual                                             ~AbstractPersistenceProvider            ();
    bool                                                isFrozen                                () const;
    shared_ptr < AbstractPersistentVersionedBranch >    makePersistentBranch                    ();
    void                                                tagBranch                               ( AbstractVersionedBranch& branch, string branchName, size_t version );
};

} // namespace Padamose
#endif
