// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <padamose/DebugStringStore.h>

namespace Padamose {

//================================================================//
// DebugStringStore
//================================================================//

//----------------------------------------------------------------//
DebugStringStore::DebugStringStore () {
}

//----------------------------------------------------------------//
DebugStringStore::~DebugStringStore () {
}

//================================================================//
// overrides
//================================================================//

//----------------------------------------------------------------//
string DebugStringStore::AbstractStringStore_getString ( string key ) const {

    map < string, string >::const_iterator stringIt = this->mStrings.find ( key );
    if ( stringIt != this->mStrings.cend ()) {
        return stringIt->second;
    }
    return string ();
}

//----------------------------------------------------------------//
bool DebugStringStore::AbstractStringStore_hasString ( string key ) const {

    return ( this->mStrings.find ( key ) != this->mStrings.cend ());
}

//----------------------------------------------------------------//
void DebugStringStore::AbstractStringStore_setString ( string key, string value ) {

    this->mStrings [ key ] = value;
}

} // namespace Padamose
