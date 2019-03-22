// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_VARIANT_H
#define PADAMOSE_VARIANT_H

#include <padamose/padamose-common.h>
#include <padamose/AbstractValueStack.h>

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
    public variant < NullVariant, bool, double, int, string > {
protected:

    // indices; must match order of template arguement list passed to variant<> base class.
    enum {
        NULL_VARIANT    = 0,
        BOOL_VARIANT,
        DOUBLE_VARIANT,
        INT_VARIANT,
        STRING_VARIANT,
    };

public:

    //----------------------------------------------------------------//
    operator bool () const {
    
        switch ( this->index ()) {
            case NULL_VARIANT:
                return false;
            case BOOL_VARIANT:
                return this->get < bool >();
            case DOUBLE_VARIANT:
                return this->get < double >() == 0.0 ? false : true;
            case INT_VARIANT:
                return this->get < int >() == 0 ? false : true;
            case STRING_VARIANT:
                return this->get < string >().size () == 0 ? false : true;
        }
        return false;
    }
    
    //----------------------------------------------------------------//
    operator double () const {
    
        switch ( this->index ()) {
            case NULL_VARIANT:
                return 0;
            case BOOL_VARIANT:
                return this->get < bool >() ? 1.0 : 0.0;
            case DOUBLE_VARIANT:
                return this->get < double >();
            case INT_VARIANT:
                return ( double )this->get < int >();
            case STRING_VARIANT:
                return 0; // TODO: parse
        }
        return false;
    }
    
    //----------------------------------------------------------------//
    operator int () const {
    
        switch ( this->index ()) {
            case NULL_VARIANT:
                return 0;
            case BOOL_VARIANT:
                return this->get < bool >() ? 1 : 0;
            case DOUBLE_VARIANT:
                return ( int )this->get < double >();
            case INT_VARIANT:
                return this->get < int >();
            case STRING_VARIANT:
                return 0; // TODO: parse
        }
        return false;
    }

    //----------------------------------------------------------------//
    operator string () const {
    
        switch ( this->index ()) {
            case NULL_VARIANT:
                return "";
            case BOOL_VARIANT:
                return this->get < bool >() ? "true" : "false";
            case DOUBLE_VARIANT:
                return ( int )this->get < double >();
            case INT_VARIANT:
                return this->get < int >();
            case STRING_VARIANT:
                return this->get < string >(); // TODO: parse
        }
        return false;
    }

    //----------------------------------------------------------------//
    template < typename TYPE >
    TYPE get () const {
        const TYPE* value = this->get_if < TYPE >();
        assert ( value );
        return *value;
    }

    //----------------------------------------------------------------//
    bool isNull () {
        return holds_alternative < NullVariant >( *this );
    }

    //----------------------------------------------------------------//
    Variant () :
        variant ( NullVariant ()) {
    }
};

} // namespace Padamose
#endif
