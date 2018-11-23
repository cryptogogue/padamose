// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <padamose/AbstractVersionedCollection.h>

namespace Padamose {

//================================================================//
// VersionedStore
//================================================================//

//----------------------------------------------------------------//
AbstractVersionedCollection::AbstractVersionedCollection () {
}

//----------------------------------------------------------------//
AbstractVersionedCollection::~AbstractVersionedCollection () {
}

//----------------------------------------------------------------//
/** \brief  String encoding for node IDs. This is an ad hoc 64-bit
            encoding; it is not base64 encoding.

    \param      nodeID      A numeric node ID.
    
    \return                 The encoded node ID.
*/
string AbstractVersionedCollection::encodeNodeID ( size_t nodeID ) {

    const char* dict = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ+-";

    // quick and dirty encoding
    
    std::string out;
    out.reserve ( 32 );
    
    if ( nodeID ) {
        for ( ; nodeID; nodeID = nodeID >> 6 ) {
            out.push_back ( dict [ nodeID & 0x3F ]);
        }
    }
    else {
        out.push_back ( dict [ 0 ]);
    }
    return out;
}

//----------------------------------------------------------------//
/** \brief  Returns the size of the collection's list of active keys.

    \return The size of the list of active keys.
*/
size_t AbstractVersionedCollection::getSize () const {
    return this->mState.mSize;
}

//----------------------------------------------------------------//
/** \brief  Returns the VersionedStoreSnapshot holding the collection.

    \return The VersionedStoreSnapshot holding the collection.
*/
const VersionedStoreSnapshot& AbstractVersionedCollection::getSnapshot () const {

    return AbstractVersionedCollection_getSnapshot ();
}

//----------------------------------------------------------------//
/** \brief  Returns the name of the collection.

    \return The name of the collection.
*/
string AbstractVersionedCollection::getName () const {

    return this->mName;
}

//----------------------------------------------------------------//
/** \brief  Sets the name of the collection and generates the cached
            lookup prefixes.
 
    \param      name        The name of the collection.
*/
void AbstractVersionedCollection::setName ( string name ) {

    if ( this->mName.find ( ':' ) != string::npos ) throw InvalidMapNameException ();

    this->mName = name;
    this->mNodePrefix = this->mName + SET_NODES_POSTFIX;
    this->mValuePrefix = this->mName + SET_VALUES_POSTFIX;
}

} // namespace Padamose
