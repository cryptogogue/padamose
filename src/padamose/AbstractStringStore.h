// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_ABSTRACTSTRINGSTORE_H
#define PADAMOSE_ABSTRACTSTRINGSTORE_H

#include <padamose/padamose-common.h>
#include <padamose/AbstractPersistenceProvider.h>

namespace Padamose {

class StringStoreVersionedBranch;

//================================================================//
// AbstractStringStore
//================================================================//
// TODO: doxygen
class AbstractStringStore :
    public AbstractPersistenceProvider {
protected:

    friend class StringStoreVersionedBranch;

    static const size_t INVALID_VERSION                 = ( size_t )-1;

    std::map < string, weak_ptr < StringStoreVersionedBranch >>     mBranchesByID;
    std::map < const AbstractVersionedBranch*, string >             mIDsByBranch;
    
    string                                          mPrefix;

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
                                                    AbstractStringStore                     ();

    //----------------------------------------------------------------//
    void                                                AbstractPersistenceProvider_flush                   () override;
    shared_ptr < AbstractPersistentVersionedBranch >    AbstractPersistenceProvider_makePersistentBranch    () override;
    void                                                AbstractPersistenceProvider_tagDidChange            ( string name, const VersionedStoreSnapshot* snapshot ) override;

    //----------------------------------------------------------------//
    virtual void        AbstractStringStore_eraseString         ( string key ) = 0;
    virtual string      AbstractStringStore_getString           ( string key ) const = 0;
    virtual bool        AbstractStringStore_hasString           ( string key ) const = 0;
    virtual void        AbstractStringStore_setString           ( string key, string value ) = 0;

    //----------------------------------------------------------------//
    template < typename TYPE >
    TYPE get ( string key, const TYPE value ) const {
        if ( this->hasString ( key )) {
            Variant var ( this->getString ( key ));
            return var.get < TYPE >();
        }
        return value;
    }
    
    //----------------------------------------------------------------//
    template < typename TYPE >
    void set ( string key, const TYPE value ) {
        Variant var ( value );
        this->setString ( key, var.get < string >());
    }

public:

    //----------------------------------------------------------------//
    void                eraseString                 ( string key );
    string              getPrefix                   () const;
    string              getString                   ( string key ) const;
    bool                hasString                   ( string key ) const;
    void                setString                   ( string key, string value );
                        AbstractStringStore         ( string prefix = "" );
    virtual             ~AbstractStringStore        ();
};

} // namespace Padamose
#endif
