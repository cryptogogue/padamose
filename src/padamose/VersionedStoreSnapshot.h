// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_VERSIONEDSTORESNAPSHOT_H
#define PADAMOSE_VERSIONEDSTORESNAPSHOT_H

#include <padamose/padamose-common.h>
#include <padamose/AbstractVersionedBranch.h>
#include <padamose/AbstractVersionedBranchClient.h>

#include <padamose/EphemeralVersionedBranch.h>

namespace Padamose {

//================================================================//
// VersionedStoreSnapshot
//================================================================//
/** \brief Common base implementation for versioned store iterators.
    Contains only read-only methods and getters.
 
    This is the base of VersionedStore, VersionedStoreIterator and VersionedValueIterator. It
    was originally part of VersionedStore, but was broken out into a separate base class
    to avoid exposing mutators through iterator implementations, which are read-only.
*/
class VersionedStoreSnapshot :
    public AbstractVersionedBranchClient {
protected:

    friend class AbstractVersionedCollection;
    friend class BaseVersionedCollectionIterator;
    friend class AbstractVersionedValueIterator;
    friend class MutableVersionedCollection;
    friend class VersionedBranch;
    friend class VersionedMap;
    friend class VersionedSet;
    friend class VersionedStoreIterator;
    
    template < typename > friend class VersionedValue;
    template < typename > friend class VersionedValueIterator;

    #ifdef _DEBUG
        /// Available in debug builds to add an easily readable name to snapshots.
        string                              mDebugName;
    #endif

    //----------------------------------------------------------------//
    void            affirmBranch                    ();
    
    //----------------------------------------------------------------//
    /** \brief Recursively searches the branch to find the value for the key. The most recent version
        equal to or earlier will be returned.

        A pointer to the value or NULL is returned.

        \param      key         The key.
        \return                 A pointer to the value for the key or NULL.
    */
    template < typename TYPE >
    const TYPE* getValueOrNil ( string key ) const {
        return this->getValueOrNil < TYPE >( key, this->mVersion );
    }
    
    //----------------------------------------------------------------//
    /** \brief Recursively searches the branch to find the value for the key. The most recent version
        equal to or earlier will be returned.

        A pointer to the value or NULL is returned.

        \param      version     Search for this version of the most recent lesser version of the value;
        \param      key         The key.
        \return                 A pointer to the value for the key or NULL.
    */
    template < typename TYPE >
    const TYPE* getValueOrNil ( string key, size_t version ) const {
        // TODO: obviously, this is temp
        const EphemeralVersionedBranch* ephemeralBranch = dynamic_cast < const EphemeralVersionedBranch* >( this->mSourceBranch.get ());
        return ephemeralBranch ? ephemeralBranch->getValueOrNil < TYPE >( version <= this->mVersion ? version : this->mVersion, key ) : NULL;
    }
    
    //----------------------------------------------------------------//
    bool            AbstractVersionedBranchClient_canJoin                   () const override;
    size_t          AbstractVersionedBranchClient_getJoinScore              () const override;
    size_t          AbstractVersionedBranchClient_getVersionDependency      () const override;
    void            AbstractVersionedBranchClient_joinBranch                ( AbstractVersionedBranch& branch ) override;
    bool            AbstractVersionedBranchClient_preventJoin               () const override;
    
public:

    //----------------------------------------------------------------//
    void            clear                           ();
    size_t          getVersion                      () const;
    bool            hasKey                          ( string key ) const;
    void            setDebugName                    ( string debugName );
    void            takeSnapshot                    ( const VersionedStoreSnapshot& other );
                    VersionedStoreSnapshot          ();
                    VersionedStoreSnapshot          ( const VersionedStoreSnapshot& other );
    virtual         ~VersionedStoreSnapshot         ();
    
    //----------------------------------------------------------------//
    /** \brief  Implements assignment by calling takeSnapshot().
     
        \param  other   The version to snapshot.
    */
    VersionedStoreSnapshot& operator = ( VersionedStoreSnapshot& other ) {
        this->takeSnapshot ( other );
        return *this;
    }
    
    //----------------------------------------------------------------//
    /** \brief  Return a copy of the value for a key. Throws a KeyNotFoundException exception
                if the key does not exist.
     
        \param  key                     The key.
        \return                         A copy of the value.
        \throws KeyNotFoundException    No value was be found for the given key.
    */
    template < typename TYPE >
    TYPE getValue ( string key ) const {
        const TYPE* value = this->getValueOrNil < TYPE >( key, this->mVersion );
        if ( !value ) throw KeyNotFoundException ();
        return *value;
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
        const TYPE* value = this->getValueOrNil < TYPE >( key, this->mVersion );
        if ( !value ) return fallback;
        return *value;
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
        const TYPE* value = this->getValueOrNil < TYPE >( key, version );
        if ( !value ) throw KeyNotFoundException ();
        return *value;
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
        const TYPE* value = this->getValueOrNil < TYPE >( key, version );
        if ( !value ) return fallback;
        return *value;
    }

    //----------------------------------------------------------------//
    /** \brief  Check to see if the value can be found.
     
        \param  key     The key.
        \return         True if the value exists. False if it doesn't.
    */
    template < typename TYPE >
    bool hasValue ( string key ) const {
        const TYPE* value = this->getValueOrNil < TYPE >( key, this->mVersion );
        return ( value != NULL );
    }
    
    //----------------------------------------------------------------//
    /** \brief  Check to see if the value can be found for the given version.
     
                Check the value for the most recent version equal to or less than
                the given version.
     
        \param  key     The key.
        \return         True if the value exists. False if it doesn't.
    */
    template < typename TYPE >
    bool hasValue ( string key, size_t version ) const {
        const TYPE* value = this->getValueOrNil < TYPE >( key, version );
        return ( value != NULL );
    }
};

} // namespace Padamose
#endif
