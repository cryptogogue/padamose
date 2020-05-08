// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_VARIANT_H
#define PADAMOSE_VARIANT_H

#include <padamose/padamose-common.h>

namespace Padamose {

//================================================================//
// NullVariant
//================================================================//
class NullVariant {
};

//================================================================//
// Variant
//================================================================//
// TODO: doxygen
class Variant :
    public variant < NullVariant, bool, double, s64, u64, string > {
public:

    // indices; must match order of template arguement list passed to variant<> base class.
    enum {
        NULL_VARIANT    = 0,
        BOOL_VARIANT,
        DOUBLE_VARIANT,
        INT64_VARIANT,
        UINT64_VARIANT,
        STRING_VARIANT,
    };

    //----------------------------------------------------------------//
    template < typename TYPE >
    TYPE get () const {
        return this->getStrict < TYPE >();
    }
    
    //----------------------------------------------------------------//
    size_t getType () const {
        return this->index ();
    }
    
    //----------------------------------------------------------------//
    Variant& operator = ( const variant& rhs ) {
        *( variant* )this = rhs;
        return *this;
    }

    //----------------------------------------------------------------//
    Variant& operator = ( bool rhs ) {
        *( variant* )this = rhs;
        return *this;
    }
    
    //----------------------------------------------------------------//
    Variant& operator = ( double rhs ) {
        *( variant* )this = rhs;
        return *this;
    }
    
    //----------------------------------------------------------------//
    Variant& operator = ( s64 rhs ) {
        *( variant* )this = rhs;
        return *this;
    }
    
    //----------------------------------------------------------------//
    Variant& operator = ( u64 rhs ) {
        *( variant* )this = rhs;
        return *this;
    }

    //----------------------------------------------------------------//
    Variant& operator = ( const char* rhs ) {
        *( variant* )this = string ( rhs );
        return *this;
    }

    //----------------------------------------------------------------//
    Variant& operator = ( string rhs ) {
        *( variant* )this = rhs;
        return *this;
    }

    //----------------------------------------------------------------//
    template < typename TYPE >
    TYPE getStrict () const {
        const TYPE* value = std::get_if < TYPE >( this );
        assert ( value );
        return *value;
    }

    //----------------------------------------------------------------//
    bool isNull () {
        return holds_alternative < NullVariant >( *this );
    }
    
    //----------------------------------------------------------------//
    template < typename TYPE >
    bool isType () {
        return holds_alternative < TYPE >( *this );
    }

    //----------------------------------------------------------------//
    Variant () :
        variant ( NullVariant ()) {
    }
    
    //----------------------------------------------------------------//
    Variant ( const variant& value ) :
        variant ( value ) {
    }
    
    //----------------------------------------------------------------//
    Variant ( bool value ) :
        variant ( value ) {
    }
    
    //----------------------------------------------------------------//
    Variant ( double value ) :
        variant ( value ) {
    }
    
    //----------------------------------------------------------------//
    Variant ( s64 value ) :
        variant ( value ) {
    }
    
    //----------------------------------------------------------------//
    Variant ( u64 value ) :
        variant ( value ) {
    }
    
    //----------------------------------------------------------------//
    Variant ( const char* value ) :
        variant ( string ( value )) {
    }
    
    //----------------------------------------------------------------//
    Variant ( string value ) :
        variant ( value ) {
    }
};

//----------------------------------------------------------------//
template <> bool    Variant::get () const;
template <> double  Variant::get () const;
template <> s64     Variant::get () const;
template <> u64     Variant::get () const;
template <> string  Variant::get () const;

} // namespace Padamose
#endif
