// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_EXCEPTIONS_H
#define PADAMOSE_EXCEPTIONS_H

#include <padamose/padamose-common.h>

namespace Padamose {

//================================================================//
// CannotAssignNullException
//================================================================//
class CannotAssignNullException :
    public logic_error {
public:

    //----------------------------------------------------------------//
    CannotAssignNullException () :
        logic_error ( "Cannot assign NULL as a versioned value." ) {
    }
};

//================================================================//
// InvalidMapNameException
//================================================================//
class InvalidMapNameException :
    public logic_error {
public:

    //----------------------------------------------------------------//
    InvalidMapNameException () :
        logic_error ( "Map name contains illegal characters." ) {
    }
};

//================================================================//
// KeyNotFoundException
//================================================================//
class KeyNotFoundException :
    public logic_error {
public:

    //----------------------------------------------------------------//
    KeyNotFoundException ( string key ) :
        logic_error ( "Key could not be found: " + key ) {
    }
};

//================================================================//
// OverwriteException
//================================================================//
class OverwriteException :
    public logic_error {
public:

    //----------------------------------------------------------------//
    OverwriteException () :
        logic_error ( "Existing value would be overwritten." ) {
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
// TypeMismatchOnGetException
//================================================================//
class TypeMismatchOnGetException :
    public logic_error {
public:

    //----------------------------------------------------------------//
    TypeMismatchOnGetException () :
        logic_error ( "Type mismatch on get; key exists, but was declared as another type." ) {
    }
};

//================================================================//
// VersionedCollectionNotFoundException
//================================================================//
class VersionedCollectionNotFoundException :
    public logic_error {
public:

    //----------------------------------------------------------------//
    VersionedCollectionNotFoundException () :
        logic_error ( "Versioned set not found for given name." ) {
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
