// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <padamose/AbstractVersionedCollection.h>

namespace Padamose {

//================================================================//
// VersionedStoreTag
//================================================================//

const size_t AbstractVersionedCollection::INVALID_NODE_INDEX = ( size_t )-1;

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
/** \brief  Returns the name of the collection.

    \return The name of the collection.
*/
string AbstractVersionedCollection::getName () const {

    return this->mName;
}

//----------------------------------------------------------------//
// TODO: doxygen
VersionedCollectionNode AbstractVersionedCollection::getNode ( string key ) const {

    const ConstVersionedStoreTag& snapshot = this->getSnapshot ();

    VersionedCollectionNode node;
    
    if ( snapshot.hasKey ( key + ".id" )) {
    
        node.mExists    = true;
        node.mID        = snapshot.getValue < u64 >( key + ".id" );
        node.mKey       = snapshot.getValue < string >( key + ".key" );
        node.mPrev      = snapshot.getValue < u64 >( key + ".prev" );
        node.mNext      = snapshot.getValue < u64 >( key + ".next" );
    }
    else {
    
        node.mExists    = false;
        node.mID        = INVALID_NODE_INDEX;
        node.mPrev      = INVALID_NODE_INDEX;
        node.mNext      = INVALID_NODE_INDEX;
    }
    return node;
}

//----------------------------------------------------------------//
/** \brief  Returns the size of the collection's list of active keys.

    \return The size of the list of active keys.
*/
size_t AbstractVersionedCollection::getSize () const {
    return this->mState.mSize;
}

//----------------------------------------------------------------//
/** \brief  Returns the ConstVersionedStoreTag holding the collection.

    \return The ConstVersionedStoreTag holding the collection.
*/
const ConstVersionedStoreTag& AbstractVersionedCollection::getSnapshot () const {

    return AbstractVersionedCollection_getSnapshot ();
}

//----------------------------------------------------------------//
/** \brief  Sets the name of the collection and generates the cached
            lookup prefixes.
 
    \param      key     The full key string to reference.
    \return             TRUE if the key exists and is active. Otherwise FALSE.
*/
bool AbstractVersionedCollection::hasKey ( string key ) const {

    return ( this->lookupNodeID ( key ) != INVALID_NODE_INDEX );
}

//----------------------------------------------------------------//
// TODO: doxygen
void AbstractVersionedCollection::loadState () {
    
    const ConstVersionedStoreTag& snapshot = this->getSnapshot ();

    this->mState.mHead  = snapshot.getValue < u64 >( this->mName + ".head" );
    this->mState.mTail  = snapshot.getValue < u64 >( this->mName + ".tail" );
    this->mState.mSize  = snapshot.getValue < u64 >( this->mName + ".size" );
}

//----------------------------------------------------------------//
/** \brief  Sets the name of the collection and generates the cached
            lookup prefixes.
 
    \param      key     The full key string to reference.
    \return             The numeric node ID (if it exists and is active) or INVALID_NODE_INDEX.
*/
size_t AbstractVersionedCollection::lookupNodeID ( string key ) const {

    return this->getSnapshot ().getValueOrFallback < u64 >( this->mLookupPrefix + key, INVALID_NODE_INDEX );
}

//----------------------------------------------------------------//
/** \brief  Sets the name of the collection and generates the cached
            lookup prefixes.
 
    \param      name        The name of the collection.
*/
void AbstractVersionedCollection::setName ( string name ) {

    if ( this->mName.find ( ':' ) != string::npos ) throw InvalidMapNameException ();

    this->mName = name;
    this->mNodePrefix = this->mName + NODES_POSTFIX;
    this->mLookupPrefix = this->mName + LOOKUP_POSTFIX;
    this->mValuePrefix = this->mName + VALUES_POSTFIX;
}

} // namespace Padamose
