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
    
    size_t prevID = this->mState.mTail;
    
    VersionedSetNode node;
    node.mID = nodeID;
    node.mKey = key;
    node.mPrev = prevID;
    node.mNext = INVALID_NODE_INDEX;
    this->mStore.setValue < VersionedSetNode >( nodeKey, node );

    this->mState.mTail = nodeID;
    this->mState.mSize++;

    // if there is a next node, update that node's prev.
    if ( prevID == INVALID_NODE_INDEX ) {
        this->mState.mHead = nodeID;
    }
    else {
        string prevNodeKey = this->mNodePrefix + to_string ( prevID );
        VersionedSetNode prevNode = this->mStore.getValue < VersionedSetNode >( prevNodeKey );
        prevNode.mNext = nodeID;
        this->mStore.setValue < VersionedSetNode >( prevNodeKey, prevNode );
    }
    
    this->mStore.setValue < VersionedSetState >( this->mMapName, this->mState );

    return nodeID;
}

//----------------------------------------------------------------//
// TODO: doxygen
void VersionedMap::deleteKey ( string key ) {

    size_t nodeID = VersionedMap::getNodeID ( key );
    string nodeKey = this->mNodePrefix + to_string ( nodeID );
    
    const VersionedSetNode* existingNode = this->mStore.getValueOrNil < VersionedSetNode >( nodeKey );
    if (( !existingNode ) || ( existingNode->mID == INVALID_NODE_INDEX )) return;
    
    // TODO: worth handing this properly? can certainly expand hash resolution, add a nonce, or other strategies...
    if ( existingNode->mKey != key ) throw VersionedMapKeyCollisionException ();
    
    if ( existingNode->mPrev != INVALID_NODE_INDEX ) {
        string prevNodeKey = this->mNodePrefix + to_string ( existingNode->mPrev );
        VersionedSetNode prevNode = this->mStore.getValue < VersionedSetNode >( prevNodeKey );
        prevNode.mNext = existingNode->mNext;
        this->mStore.setValue < VersionedSetNode >( prevNodeKey, prevNode );
    }
    else {
        this->mState.mHead = existingNode->mNext;
    }
    
    if ( existingNode->mNext != INVALID_NODE_INDEX ) {
        string nextNodeKey = this->mNodePrefix + to_string ( existingNode->mNext );
        VersionedSetNode nextNode = this->mStore.getValue < VersionedSetNode >( nextNodeKey );
        nextNode.mPrev = existingNode->mPrev;
        this->mStore.setValue < VersionedSetNode >( nextNodeKey, nextNode );
    }
    else {
        this->mState.mTail = existingNode->mPrev;
    }

    VersionedSetNode node;
    node.mPrev = INVALID_NODE_INDEX;
    node.mNext = INVALID_NODE_INDEX;
    node.mID = INVALID_NODE_INDEX;
    
    this->mStore.setValue < VersionedSetNode >( nodeKey, node );
    
    this->mState.mSize--;
    this->mStore.setValue < VersionedSetState >( this->mMapName, this->mState );
}

//----------------------------------------------------------------//
size_t VersionedMap::getNodeID ( string key ) {

    return std::hash < string >{}( key );
}

//----------------------------------------------------------------//
// TODO: doxygen
VersionedMap::VersionedMap ( VersionedStore& store, string mapName ) :
    mStore ( store ) {

    this->setName ( mapName );

    if ( this->mStore.hasKey ( this->mMapName )) {
        this->mState = this->mStore.getValue < VersionedSetState >( this->mMapName );
    }
    else {
        if ( this->mMapName.find ( ':' ) != string::npos ) throw InvalidMapNameException ();
        
        this->mState.mHead = INVALID_NODE_INDEX;
        this->mState.mTail = INVALID_NODE_INDEX;
        this->mState.mSize = 0;
        this->mState.mFreeStack = INVALID_NODE_INDEX;
        this->mState.mTotalNodes = 0;
        
        this->mStore.setValue < VersionedSetState >( this->mMapName, this->mState );
    }
    
    this->mNodePrefix = this->mMapName + SET_NODES_POSTFIX;
    this->mValuePrefix = this->mMapName + SET_VALUES_POSTFIX;
}

//----------------------------------------------------------------//
// TODO: doxygen
VersionedMap::~VersionedMap () {
}

//================================================================//
// overrides
//================================================================//

//----------------------------------------------------------------//
const VersionedStoreSnapshot& VersionedMap::AbstractVersionedSet_getSnapshot () const {

    return this->mStore;
}

} // namespace Padamose
