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
protected:

    // indices; must match order of template arguement list passed to variant<> base class.
    enum {
        NULL_VARIANT    = 0,
        BOOL_VARIANT,
        DOUBLE_VARIANT,
        INT_VARIANT,
        SIZE_VARIANT,
        STRING_VARIANT,
    };

public:

    //----------------------------------------------------------------//
    template < typename TYPE >
    TYPE get () const {
        return this->getStrict < TYPE >();
    }

    //----------------------------------------------------------------//
    template <>
    bool get () const {
    
        switch ( this->index ()) {
            case NULL_VARIANT:
                return false;
            case BOOL_VARIANT:
                return this->getStrict < bool >();
            case DOUBLE_VARIANT:
                return this->getStrict < double >() == 0.0 ? false : true;
            case INT_VARIANT:
                return this->getStrict < int >() == 0 ? false : true;
            case SIZE_VARIANT:
                return this->getStrict < size_t >() == 0 ? false : true;
            case STRING_VARIANT:
                return this->getStrict < string >().size () == 0 ? false : true;
        }
        return false;
    }
    
    //----------------------------------------------------------------//
    template <>
    double get () const {
    
        switch ( this->index ()) {
            case NULL_VARIANT:
                return 0;
            case BOOL_VARIANT:
                return this->getStrict < bool >() ? 1.0 : 0.0;
            case DOUBLE_VARIANT:
                return this->getStrict < double >();
            case INT_VARIANT:
                return ( double )this->getStrict < int >();
            case SIZE_VARIANT:
                return ( double )this->getStrict < size_t >();
            case STRING_VARIANT:
                return stod ( this->getStrict < string >());
        }
        return false;
    }
    
    //----------------------------------------------------------------//
    template <>
    int get () const {
    
        switch ( this->index ()) {
            case NULL_VARIANT:
                return 0;
            case BOOL_VARIANT:
                return this->getStrict < bool >() ? 1 : 0;
            case DOUBLE_VARIANT:
                return ( int )this->getStrict < double >();
            case INT_VARIANT:
                return this->getStrict < int >();
            case SIZE_VARIANT:
                return ( int )this->getStrict < size_t >();
            case STRING_VARIANT:
                return stoi ( this->getStrict < string >());
        }
        return false;
    }

    //----------------------------------------------------------------//
    template <>
    size_t get () const {
    
        switch ( this->index ()) {
            case NULL_VARIANT:
                return 0;
            case BOOL_VARIANT:
                return this->getStrict < bool >() ? 1 : 0;
            case DOUBLE_VARIANT:
                return ( size_t )this->getStrict < double >();
            case INT_VARIANT:
                return ( size_t )this->getStrict < int >();
            case SIZE_VARIANT:
                return this->getStrict < size_t >();
            case STRING_VARIANT:
                return stoi ( this->getStrict < string >());
        }
        return false;
    }

    //----------------------------------------------------------------//
    template <>
    string get () const {
    
        ostringstream stream;
    
        switch ( this->index ()) {
            case NULL_VARIANT:
                return "";
            case BOOL_VARIANT:
                return this->getStrict < bool >() ? "true" : "false";
            case DOUBLE_VARIANT:
                stream << this->getStrict < double >();
                return stream.str ();
            case INT_VARIANT:
                stream << this->getStrict < int >();
                return stream.str ();
            case SIZE_VARIANT:
                stream << this->getStrict < size_t >();
                return stream.str ();
            case STRING_VARIANT:
                return this->getStrict < string >();
        }
        return "";
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

} // namespace Padamose
#endif
