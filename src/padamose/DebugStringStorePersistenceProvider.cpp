// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <padamose/DebugStringStorePersistenceProvider.h>

namespace Padamose {

//================================================================//
// DebugStringStorePersistenceProvider
//================================================================//

//----------------------------------------------------------------//
DebugStringStorePersistenceProvider::DebugStringStorePersistenceProvider () {
}

//----------------------------------------------------------------//
DebugStringStorePersistenceProvider::~DebugStringStorePersistenceProvider () {

    this->mTags.clear ();
}

//----------------------------------------------------------------//
void DebugStringStorePersistenceProvider::dump () const {

    map < string, string >::const_iterator stringIt = this->mStrings.cbegin ();
    for ( ; stringIt != this->mStrings.end (); ++stringIt ) {
        printf ( "%s:\t%s\n", stringIt->first.c_str (), stringIt->second.c_str ());
    }
}

//================================================================//
// overrides
//================================================================//

//----------------------------------------------------------------//
void DebugStringStorePersistenceProvider::AbstractStringStore_eraseString ( string key ) {

    this->mStrings.erase ( key );
}

//----------------------------------------------------------------//
string DebugStringStorePersistenceProvider::AbstractStringStore_getString ( string key ) const {

    map < string, string >::const_iterator stringIt = this->mStrings.find ( key );
    if ( stringIt != this->mStrings.cend ()) {
        return stringIt->second;
    }
    return string ();
}

//----------------------------------------------------------------//
bool DebugStringStorePersistenceProvider::AbstractStringStore_hasString ( string key ) const {

    return ( this->mStrings.find ( key ) != this->mStrings.cend ());
}

//----------------------------------------------------------------//
void DebugStringStorePersistenceProvider::AbstractStringStore_setString ( string key, string value ) {

    this->mStrings [ key ] = value;
}

} // namespace Padamose
