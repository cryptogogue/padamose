// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <padamose/VersionedMap.h>

namespace Padamose {

//================================================================//
// VersionedStore
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
size_t VersionedMap::affirmKey ( string key ) {

    size_t nodeID = VersionedMap::getNodeID ( key );
    string nodeKey = this->mNodePrefix + to_string ( nodeID );
    
    const VersionedSetNode* existingNode = this->mStore.getValueOrNil < VersionedSetNode >( nodeKey );
    if (( existingNode ) && ( existingNode->mID != INVALID_NODE_INDEX )) {
        // TODO: worth handing this properly? can certainly expand hash resolution, add a nonce, or other strategies...
        if ( existingNode->mKey != key ) throw VersionedMapKeyCollisionException ();
        return nodeID;
    }
    
    this->insertNode ( nodeID, key, nodeKey );

    return nodeID;
}

//----------------------------------------------------------------//
// TODO: doxygen
void VersionedMap::deleteKey ( string key ) {

    size_t nodeID = VersionedMap::getNodeID ( key );
    this->removeNode ( to_string ( nodeID ));
}

//----------------------------------------------------------------//
size_t VersionedMap::getNodeID ( string key ) {

    return std::hash < string >{}( key );
}

//----------------------------------------------------------------//
// TODO: doxygen
VersionedMap::VersionedMap ( VersionedStore& store, string mapName ) :
    MutableVersionedCollection ( store, mapName ) {
    
    this->affirmState ();
}

//----------------------------------------------------------------//
// TODO: doxygen
VersionedMap::~VersionedMap () {
}

} // namespace Padamose
