// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_ABSTRACTPERSISTENCEPROVIDER_H
#define PADAMOSE_ABSTRACTPERSISTENCEPROVIDER_H

#include <padamose/padamose-common.h>
#include <padamose/ConstVersionedStoreTag.h>

namespace Padamose {

class AbstractPersistentVersionedBranch;
class AbstractVersionedBranch;

//================================================================//
// AbstractPersistenceProvider
//================================================================//
// TODO: doxygen
class AbstractPersistenceProvider {
protected:

    friend class AbstractPersistentVersionedBranch;
    friend class VersionedStoreTag;
    friend class ConstVersionedStoreTag;

    map < string, ConstVersionedStoreTag >  mTags;
    bool                                    mIsFrozen;

    //----------------------------------------------------------------//
    void                                begin           ();
    void                                commit          ();
    void                                freeze          ();
    const ConstVersionedStoreTag&       getTag          ( string branchName ) const;
    bool                                hasTag          ( string branchName ) const;
    
    //----------------------------------------------------------------//
    virtual void                                                AbstractPersistenceProvider_begin                   () = 0;
    virtual void                                                AbstractPersistenceProvider_commit                  () = 0;
    virtual void                                                AbstractPersistenceProvider_flush                   () = 0;
    virtual shared_ptr < AbstractPersistentVersionedBranch >    AbstractPersistenceProvider_makePersistentBranch    () = 0;
    virtual void                                                AbstractPersistenceProvider_tagDidChange            ( string name, const ConstVersionedStoreTag* snapshot ) = 0;
    
public:

    //----------------------------------------------------------------//
                                                        AbstractPersistenceProvider             ();
    virtual                                             ~AbstractPersistenceProvider            ();
    void                                                flush                                   ();
    bool                                                isFrozen                                () const;
    shared_ptr < AbstractPersistentVersionedBranch >    makePersistentBranch                    ();
    void                                                tagBranch                               ( AbstractVersionedBranch& branch, string branchName, size_t version );
};

} // namespace Padamose
#endif
