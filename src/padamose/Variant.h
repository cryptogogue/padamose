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
    public variant < NullVariant, bool, double, int, size_t, string > {
public:

    // indices; must match order of template arguement list passed to variant<> base class.
    enum {
        NULL_VARIANT    = 0,
        BOOL_VARIANT,
        DOUBLE_VARIANT,
        INT_VARIANT,
        SIZE_VARIANT,
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
    Variant& operator = ( int rhs ) {
        *( variant* )this = rhs;
        return *this;
    }
    
    //----------------------------------------------------------------//
    Variant& operator = ( size_t rhs ) {
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
    Variant ( int value ) :
        variant ( value ) {
    }
    
    //----------------------------------------------------------------//
    Variant ( size_t value ) :
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
template <> int     Variant::get () const;
template <> size_t  Variant::get () const;
template <> string  Variant::get () const;

} // namespace Padamose
#endif
