// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <padamose/MutableVersionedCollection.h>

namespace Padamose {

//================================================================//
// MutableVersionedCollection
//================================================================//

//----------------------------------------------------------------//
bool MutableVersionedCollection::affirmState () {

    if ( this->mStore.hasKey ( this->mMapName )) {
        this->mState = this->mStore.getValue < VersionedCollectionState >( this->mMapName );
        return true;
    }

    this->mState.mHead = INVALID_NODE_INDEX;
    this->mState.mTail = INVALID_NODE_INDEX;
    this->mState.mSize = 0;
    
    this->mStore.setValue < VersionedCollectionState >( this->mMapName, this->mState );

    return false;
}

//----------------------------------------------------------------//
void MutableVersionedCollection::insertNode ( size_t nodeID, string key, string nodeKey ) {

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
    
    this->mStore.setValue < VersionedCollectionState >( this->mMapName, this->mState );
}

//----------------------------------------------------------------//
// TODO: doxygen
MutableVersionedCollection::MutableVersionedCollection ( VersionedStore& store, string mapName ) :
    mStore ( store ) {

    this->setName ( mapName );
}

//----------------------------------------------------------------//
// TODO: doxygen
MutableVersionedCollection::~MutableVersionedCollection () {
}

//----------------------------------------------------------------//
size_t MutableVersionedCollection::removeNode ( string key, size_t prevID ) {

    string nodeKey = this->mNodePrefix + key;
    
    const VersionedSetNode* existingNode = this->mStore.getValueOrNil < VersionedSetNode >( nodeKey );
    if (( !existingNode ) || ( existingNode->mID == INVALID_NODE_INDEX )) throw KeyNotFoundException ();
    
    size_t nodeID = existingNode->mID;
    
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
    node.mPrev = prevID;
    node.mNext = INVALID_NODE_INDEX;
    node.mID = INVALID_NODE_INDEX;
    
    this->mStore.setValue < VersionedSetNode >( nodeKey, node );
    
    this->mState.mSize--;
    this->mStore.setValue < VersionedCollectionState >( this->mMapName, this->mState );
    
    return nodeID;
}

//================================================================//
// overrides
//================================================================//

//----------------------------------------------------------------//
const VersionedStoreSnapshot& MutableVersionedCollection::AbstractVersionedCollection_getSnapshot () const {

    return this->mStore;
}

} // namespace Padamose
