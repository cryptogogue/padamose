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

    // hash portion of key; assumed to be no collisions, so counter portion is 0.
    size_t nodeID = this->getHashPortion ( key );
    string collisionKey = this->mCollisionPrefix + nodeIDToString ( nodeID );
    bool foundCollisions = false;
    
    // check to see if there are already known collisions.
    const size_t* collisionCountRef = this->mStore.getValueOrNil < size_t >( collisionKey );
    if ( collisionCountRef ) {
    
        foundCollisions = true; // user for sanity check later.
    
        // there are already collisions; get the count
        size_t collisionCount = *collisionCountRef;
    
        // see if there is already a decollider for this key.
        string decolliderKey = this->mDecolliderPrefix + key; // decolliders are stored for the full key.
        const size_t* decolliderRef = this->mStore.getValueOrNil < size_t >( decolliderKey );
        if ( decolliderRef ) {
        
            // there is a decollider, so use that instead.
            nodeID = *decolliderRef;
        }
        else {
        
            assert ( collisionCount != COUNTER_PORTION_MAX ); // unrecoverable
        
            // no decollider, so provision a new one.
            // a decollider is just the has portion combined with a count of the colliding keys.
            nodeID |= collisionCount << COUNTER_PORTION_SHIFT;
        
            // store the decollider and increment the collision count.
            this->mStore.setValue < size_t >( decolliderKey, nodeID );
            this->mStore.setValue < size_t >( collisionKey, collisionCount++ );
        }
    }
    
    string nodeKey = this->mNodePrefix + nodeIDToString ( nodeID );
    
    const VersionedSetNode* existingNode = this->mStore.getValueOrNil < VersionedSetNode >( nodeKey );
    if ( existingNode && ( existingNode->mID != INVALID_NODE_INDEX )) {

        // check to see if there is a collision. if not, we're done.
        if ( existingNode->mKey == key ) return nodeID;

        // there is a collision!
        
        // sanity check.
        assert ( !foundCollisions );

        // there will be two decolliders now.
        this->mStore.setValue < size_t >( collisionKey, 2 );

        // decollide the original node. it'll keep its existing ID.
        this->mStore.setValue < size_t >( this->mDecolliderPrefix + existingNode->mKey, nodeID );

        // colliding node is the first non-zero decollider.
        nodeID |= ( size_t )1 << COUNTER_PORTION_SHIFT;
        this->mStore.setValue < size_t >( this->mDecolliderPrefix + key, nodeID );
        nodeKey = this->mNodePrefix + nodeIDToString ( nodeID );
    }
    
    this->insertNode ( nodeID, key, nodeKey );
    
    return nodeID;
}

//----------------------------------------------------------------//
// TODO: doxygen
void VersionedMap::deleteKey ( string key ) {

    // hash portion of key; assumed to be no collisions, so counter portion is 0.
    size_t nodeID = this->getHashPortion ( key );
    string collisionKey = this->mCollisionPrefix + nodeIDToString ( nodeID );
    
    // check to see if there are already known collisions.
    if ( this->mStore.hasKey ( collisionKey )) {
    
        // there are collisions. if the key is valid, there must be a decollider for it.
        string decolliderKey = this->mDecolliderPrefix + key; // decolliders are stored for the full key.
        const size_t* decolliderRef = this->mStore.getValueOrNil < size_t >( decolliderKey );
        
        // sanity check.
        assert ( decolliderRef );
        
        // there is a decollider, so use that instead.
        nodeID = *decolliderRef;
    }
    this->removeNode ( nodeIDToString ( nodeID ));
}

//----------------------------------------------------------------//
size_t VersionedMap::getHashPortion ( string key ) const {

    assert ( sizeof ( size_t ) == 8 );
    return this->VersionedMap_hash ( key ) & HASH_PORTION_MASK;
}

//----------------------------------------------------------------//
// TODO: doxygen
VersionedMap::VersionedMap ( VersionedStore& store, string mapName ) :
    MutableVersionedCollection ( store, mapName ) {
    
    this->affirmState ();
    
    this->mCollisionPrefix      = this->mMapName + COLLISION_POSTFIX;
    this->mDecolliderPrefix     = this->mMapName + DECOLLIDER_POSTFIX;
}

//----------------------------------------------------------------//
// TODO: doxygen
VersionedMap::~VersionedMap () {
}

//----------------------------------------------------------------//
size_t VersionedMap::VersionedMap_hash ( string key ) const {

    return std::hash < string >{}( key );
}

} // namespace Padamose
