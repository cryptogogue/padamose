// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_ABSTRACTVERSIONEDSTOREINSPECTOR_H
#define PADAMOSE_ABSTRACTVERSIONEDSTOREINSPECTOR_H

#include <padamose/padamose-common.h>
#include <padamose/Variant.h>
#include <padamose/AbstractHasVersionedStoreRef.h>

namespace Padamose {

//================================================================//
// AbstractVersionedStoreInspector
//================================================================//
/// TODO: doxygen
class AbstractVersionedStoreInspector :
    public virtual AbstractHasVersionedStoreRef {
protected:

    friend class VersionedStoreRef;

public:

    //----------------------------------------------------------------//
                                AbstractVersionedStoreInspector         ();
    virtual                     ~AbstractVersionedStoreInspector        ();
    Variant                     getValueVariant                         ( string key ) const;
    Variant                     getValueVariant                         ( string key, size_t version ) const;
    bool                        hasKey                                  ( string key ) const;
    bool                        hasValue                                ( string key ) const;
    bool                        hasValue                                ( string key, size_t version ) const;
    
    //----------------------------------------------------------------//
    /** \brief  Return a copy of the value for a key. Throws a KeyNotFoundException exception
                if the key does not exist.
     
        \param  key                     The key.
        \return                         A copy of the value.
        \throws KeyNotFoundException    No value was be found for the given key.
    */
    template < typename TYPE >
    TYPE getValue ( string key ) const {
        return this->getValue < TYPE >( key, this->getRef ().mVersion );
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
        return this->getValueOrFallback < TYPE >( key, this->getRef ().mVersion, fallback );
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
        return this->hasValueWithType < TYPE >( key, this->getRef ().mVersion );
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
