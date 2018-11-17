// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_EXCEPTION_H
#define PADAMOSE_EXCEPTION_H

#include <padamose/padamose-common.h>

namespace Padamose {

//================================================================//
// KeyNotFoundException
//================================================================//
class KeyNotFoundException :
    public logic_error {
public:

    //----------------------------------------------------------------//
    KeyNotFoundException () :
        logic_error ( "Key could not be found." ) {
    }
};

//================================================================//
// TypeMismatchOnAssignException
//================================================================//
class TypeMismatchOnAssignException :
    public logic_error {
public:

    //----------------------------------------------------------------//
    TypeMismatchOnAssignException () :
        logic_error ( "Type mismatch on assign to key; key value was already declared as another type." ) {
    }
};

//================================================================//
// VersionOutOfBoundsException
//================================================================//
class VersionOutOfBoundsException :
    public logic_error {
public:

    //----------------------------------------------------------------//
    VersionOutOfBoundsException () :
        logic_error ( "Attempt to reference out-of-bounds version." ) {
    }
};

} // namespace Padamose
#endif
