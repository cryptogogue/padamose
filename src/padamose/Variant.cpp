// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <padamose/Variant.h>

namespace Padamose {


//================================================================//
// Variant
//================================================================//

//----------------------------------------------------------------//
template <>
bool Variant::get () const {

    switch ( this->getType ()) {
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
double Variant::get () const {

    switch ( this->getType ()) {
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
int Variant::get () const {

    switch ( this->getType ()) {
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
size_t Variant::get () const {

    switch ( this->getType ()) {
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
string Variant::get () const {

    ostringstream stream;

    switch ( this->getType ()) {
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

} // namespace Padamose
