// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_ABSTRACTPERSISTENCEPROVIDER_H
#define PADAMOSE_ABSTRACTPERSISTENCEPROVIDER_H

#include <padamose/padamose-common.h>
#include <padamose/VersionedStoreTag.h>

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
    friend class VersionedStoreTag;

    map < string, VersionedStoreTag >       mTags;
    bool                                    mIsFrozen;

    //----------------------------------------------------------------//
    void                            freeze          ();
    const VersionedStoreTag&        getTag          ( string branchName ) const;
    bool                            hasTag          ( string branchName ) const;
    
    //----------------------------------------------------------------//
    virtual void                                                AbstractPersistenceProvider_begin                   () = 0;
    virtual void                                                AbstractPersistenceProvider_commit                  () = 0;
    virtual shared_ptr < AbstractPersistentVersionedBranch >    AbstractPersistenceProvider_makePersistentBranch    ( AbstractVersionedBranch& from ) = 0;
    virtual void                                                AbstractPersistenceProvider_tagDidChange            ( string name, const VersionedStoreTag* snapshot ) = 0;
    
public:

    //----------------------------------------------------------------//
                                                        AbstractPersistenceProvider             ();
    virtual                                             ~AbstractPersistenceProvider            ();
    void                                                begin                                   ();
    void                                                commit                                  ();
    void                                                flush                                   ();
    bool                                                isFrozen                                () const;
    shared_ptr < AbstractPersistentVersionedBranch >    makePersistentBranch                    ( AbstractVersionedBranch& from );
    void                                                persist                                 ( VersionedStoreTag& tag, string tagName );
    VersionedStoreTag                                   restore                                 ( string tagName );
};

} // namespace Padamose
#endif
