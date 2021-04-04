// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_CONSTVERSIONEDSTORETAG_H
#define PADAMOSE_CONSTVERSIONEDSTORETAG_H

#include <padamose/padamose-common.h>
#include <padamose/AbstractVersionedBranch.h>
#include <padamose/AbstractVersionedBranchClient.h>

#include <padamose/EphemeralVersionedBranch.h>

namespace Padamose {

//================================================================//
// ConstVersionedStoreTag
//================================================================//
/** \brief Common base implementation for versioned store iterators.
    Contains only read-only methods and getters.
 
    This is the base of VersionedStoreTag, VersionedStoreIterator and VersionedValueIterator. It
    was originally part of VersionedStoreTag, but was broken out into a separate base class
    to avoid exposing mutators through iterator implementations, which are read-only.
*/
class ConstVersionedStoreTag :
    public AbstractVersionedBranchClient {
protected:

    friend class AbstractPersistenceProvider;
    friend class AbstractVersionedCollection;
    friend class BaseVersionedCollectionIterator;
    friend class AbstractVersionedValueIterator;
    friend class MutableVersionedCollection;
    friend class VersionedBranch;
    friend class VersionedMap;
    friend class VersionedList;
    friend class VersionedStoreIterator;
    
    template < typename > friend class VersionedValue;
    template < typename > friend class VersionedValueIterator;

    string          mDebugName;
    
    //----------------------------------------------------------------//
    bool            AbstractVersionedBranchClient_canJoin                   () const override;
    size_t          AbstractVersionedBranchClient_getJoinScore              () const override;
    size_t          AbstractVersionedBranchClient_getVersionDependency      () const override;
    void            AbstractVersionedBranchClient_joinBranch                ( AbstractVersionedBranch& branch ) override;
    bool            AbstractVersionedBranchClient_preventJoin               () const override;
    void            AbstractVersionedBranchClient_sourceBranchDidChange     () override;
    
public:

    //----------------------------------------------------------------//
    void            clear                           ();
                    ConstVersionedStoreTag          ();
                    ConstVersionedStoreTag          ( const AbstractVersionedBranchClient& other );
                    ConstVersionedStoreTag          ( shared_ptr < AbstractPersistenceProvider > provider, string branchName );
    virtual         ~ConstVersionedStoreTag         ();
    Variant         getValueVariant                 ( string key ) const;
    Variant         getValueVariant                 ( string key, size_t version ) const;
    size_t          getVersion                      () const;
    bool            hasKey                          ( string key ) const;
    bool            hasValue                        ( string key ) const;
    bool            hasValue                        ( string key, size_t version ) const;
    void            setDebugName                    ( string debugName );
    
    //----------------------------------------------------------------//
    /** \brief  Return a copy of the value for a key. Throws a KeyNotFoundException exception
                if the key does not exist.
     
        \param  key                     The key.
        \return                         A copy of the value.
        \throws KeyNotFoundException    No value was be found for the given key.
    */
    template < typename TYPE >
    TYPE getValue ( string key ) const {
        return this->getValue < TYPE >( key, this->mVersion );
    }
    
    //----------------------------------------------------------------//
    /** \brief  Return a copy of the value for a key at a given version. Throws a
                KeyNotFoundException exception if the key does not exist.
     
                Returns the value for the most recent version equal to or less than
                the given version.
     
        \param  key                     The key.
        \param  version                 The version.
        \return                         A copy of the value.
        \throws KeyNotFoundException    No value was be found for the given key.
    */
    template < typename TYPE >
    TYPE getValue ( string key, size_t version ) const {
        Variant value = this->getValueVariant ( key, version );
        if ( value.isNull ()) throw KeyNotFoundException ( key );
        if ( !value.isType < TYPE >()) throw TypeMismatchOnGetException ();
        return value.getStrict < TYPE >();
    }

    //----------------------------------------------------------------//
    /** \brief  Return a copy of the value for a key. If the value doesn't
                exist, return the fallback.
     
        \param  key                     The key.
        \param  fallback                The fallback.
        \return                         The value or the fallback.
    */
    template < typename TYPE >
    TYPE getValueOrFallback ( string key, const TYPE& fallback ) const {
        return this->getValueOrFallback < TYPE >( key, this->mVersion, fallback );
    }

    //----------------------------------------------------------------//
    /** \brief  Return a copy of the value for a key at a given version. If
                the value doesn't exist, return the fallback.
     
                Returns the value for the most recent version equal to or less than
                the given version.
     
        \param  key                     The key.
        \param  version                 The version.
        \param  fallback                The fallback.
        \return                         The value or the fallback.
    */
    template < typename TYPE >
    TYPE getValueOrFallback ( string key, size_t version, const TYPE& fallback ) const {
        Variant value = this->getValueVariant ( key, version );
        if ( value.isNull ()) return fallback;
        return value.getStrict < TYPE >();
    }
    
    //----------------------------------------------------------------//
    // TODO: doxygen
    template < typename TYPE >
    bool hasValueWithType ( string key ) const {
        return this->hasValueWithType < TYPE >( key, this->mVersion );
    }
    
    //----------------------------------------------------------------//
    // TODO: doxygen
    template < typename TYPE >
    bool hasValueWithType ( string key, size_t version ) const {
        // TODO: provide a more efficient implementation
        Variant value = this->getValueVariant ( key, version );
        return holds_alternative < TYPE >( value );
    }
};

} // namespace Padamose
#endif
