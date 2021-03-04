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
        case INT64_VARIANT:
            return this->getStrict < s64 >() == 0 ? false : true;
        case UINT64_VARIANT:
            return this->getStrict < u64 >() == 0 ? false : true;
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
        case INT64_VARIANT:
            return ( double )this->getStrict < s64 >();
        case UINT64_VARIANT:
            return ( double )this->getStrict < u64 >();
        case STRING_VARIANT:
            return stod ( this->getStrict < string >());
    }
    return false;
}

//----------------------------------------------------------------//
template <>
s64 Variant::get () const {

    switch ( this->getType ()) {
        case NULL_VARIANT:
            return 0;
        case BOOL_VARIANT:
            return this->getStrict < bool >() ? 1 : 0;
        case DOUBLE_VARIANT:
            return ( int )this->getStrict < double >();
        case INT64_VARIANT:
            return this->getStrict < s64 >();
        case UINT64_VARIANT:
            return ( int )this->getStrict < u64 >();
        case STRING_VARIANT:
            return stoll ( this->getStrict < string >());
    }
    return false;
}

//----------------------------------------------------------------//
template <>
u64 Variant::get () const {

    switch ( this->getType ()) {
        case NULL_VARIANT:
            return 0;
        case BOOL_VARIANT:
            return this->getStrict < bool >() ? 1 : 0;
        case DOUBLE_VARIANT:
            return ( size_t )this->getStrict < double >();
        case INT64_VARIANT:
            return ( size_t )this->getStrict < s64 >();
        case UINT64_VARIANT:
            return this->getStrict < u64 >();
        case STRING_VARIANT:
            return stoull ( this->getStrict < string >());
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
        case INT64_VARIANT:
            stream << this->getStrict < s64 >();
            return stream.str ();
        case UINT64_VARIANT:
            stream << this->getStrict < u64 >();
            return stream.str ();
        case STRING_VARIANT:
            return this->getStrict < string >();
    }
    return "";
}

} // namespace Padamose
