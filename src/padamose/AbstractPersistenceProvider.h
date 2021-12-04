// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_ABSTRACTPERSISTENCEPROVIDER_H
#define PADAMOSE_ABSTRACTPERSISTENCEPROVIDER_H

#include <padamose/padamose-common.h>
#include <padamose/VersionedStoreTag.h>

namespace Padamose {

class AbstractPersistenceProvider;
class AbstractPersistentVersionedBranch;
class AbstractVersionedBranch;

//================================================================//
// PersistenceTag
//================================================================//
class PersistenceTag :
    public VersionedStoreTag {
protected:

    friend class AbstractPersistenceProvider;

    string                          mTagName;
    AbstractPersistenceProvider*    mOwner;

    //----------------------------------------------------------------//
    void            AbstractVersionedBranchClient_sourceBranchDidChange     () override;

public:

    //----------------------------------------------------------------//
    string          getName                 () const;
                    PersistenceTag          ();
                    ~PersistenceTag         ();
};

//================================================================//
// AbstractPersistenceProvider
//================================================================//
// TODO: doxygen
class AbstractPersistenceProvider {
protected:

    friend class AbstractPersistentVersionedBranch;
    friend class PersistenceTag;
    friend class VersionedStoreTag;
    friend class VersionedStoreTag;

    map < string, PersistenceTag >      mTags;
    bool                                mIsFrozen;

    //----------------------------------------------------------------//
    void                                freeze          ();
    const VersionedStoreTag&            getTag          ( string branchName ) const;
    bool                                hasTag          ( string branchName ) const;
    
    //----------------------------------------------------------------//
    virtual void                                                AbstractPersistenceProvider_beginTransaction        () = 0;
    virtual void                                                AbstractPersistenceProvider_commitTransaction       () = 0;
    virtual shared_ptr < AbstractPersistentVersionedBranch >    AbstractPersistenceProvider_makePersistentBranch    ( AbstractVersionedBranch& from ) = 0;
    virtual void                                                AbstractPersistenceProvider_removeTag               ( const PersistenceTag& tag ) = 0;
    virtual void                                                AbstractPersistenceProvider_tagDidChange            ( const PersistenceTag& tag ) = 0;
    
public:

    //----------------------------------------------------------------//
                                                        AbstractPersistenceProvider             ();
    virtual                                             ~AbstractPersistenceProvider            ();
    void                                                beginTransaction                        ();
    void                                                commitTransaction                       ();
    void                                                flush                                   ();
    bool                                                isFrozen                                () const;
    shared_ptr < AbstractPersistentVersionedBranch >    makePersistentBranch                    ( AbstractVersionedBranch& from );
    void                                                persist                                 ( VersionedStoreTag& tag, string tagName );
    void                                                remove                                  ( string tagName );
    VersionedStoreTag                                   restore                                 ( string tagName );
};

} // namespace Padamose
#endif
