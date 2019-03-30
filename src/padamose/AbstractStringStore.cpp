// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <padamose/AbstractStringStore.h>

namespace Padamose {

//================================================================//
// AbstractStringStore
//================================================================//

//----------------------------------------------------------------//
AbstractStringStore::AbstractStringStore () {
}

//----------------------------------------------------------------//
AbstractStringStore::~AbstractStringStore () {
}

//----------------------------------------------------------------//
string AbstractStringStore::getString ( string key ) const {

    return this->AbstractStringStore_getString ( key );
}

//----------------------------------------------------------------//
bool AbstractStringStore::hasString ( string key ) const {

    return this->AbstractStringStore_hasString ( key );
}

//----------------------------------------------------------------//
void AbstractStringStore::setString ( string key, string value ) {

    this->AbstractStringStore_setString ( key, value );
}

} // namespace Padamose
