// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <padamose/MutableVersionedCollection.h>

namespace Padamose {

//================================================================//
// MutableVersionedCollection
//================================================================//

//----------------------------------------------------------------//
bool MutableVersionedCollection::affirmState () {

    if ( this->mStore.hasKey ( this->mName )) {
        this->mState = this->mStore.getValue < VersionedCollectionState >( this->mName );
        return true;
    }

    this->mState.mHead = INVALID_NODE_INDEX;
    this->mState.mTail = INVALID_NODE_INDEX;
    this->mState.mSize = 0;
    
    this->mStore.setValue < VersionedCollectionState >( this->mName, this->mState );

    return false;
}

//----------------------------------------------------------------//
void MutableVersionedCollection::insertNode ( size_t nodeID, string key, string nodeKey ) {

    size_t prevID = this->mState.mTail;
    
    VersionedCollectionNode node;
    node.mID = nodeID;
    node.mKey = key;
    node.mPrev = prevID;
    node.mNext = INVALID_NODE_INDEX;
    this->mStore.setValue < VersionedCollectionNode >( nodeKey, node );

    this->mState.mTail = nodeID;
    this->mState.mSize++;

    // if there is a next node, update that node's prev.
    if ( prevID == INVALID_NODE_INDEX ) {
        this->mState.mHead = nodeID;
    }
    else {
        string prevNodeKey = this->mNodePrefix + encodeNodeID ( prevID );
        VersionedCollectionNode prevNode = this->mStore.getValue < VersionedCollectionNode >( prevNodeKey );
        prevNode.mNext = nodeID;
        this->mStore.setValue < VersionedCollectionNode >( prevNodeKey, prevNode );
    }
    
    this->mStore.setValue < VersionedCollectionState >( this->mName, this->mState );
}

//----------------------------------------------------------------//
// TODO: doxygen
MutableVersionedCollection::MutableVersionedCollection ( VersionedStore& store, string name ) :
    mStore ( store ) {

    this->setName ( name );
}

//----------------------------------------------------------------//
// TODO: doxygen
MutableVersionedCollection::~MutableVersionedCollection () {
}

//----------------------------------------------------------------//
size_t MutableVersionedCollection::removeNode ( string key, size_t prevID ) {

    string nodeKey = this->mNodePrefix + key;
    
    const VersionedCollectionNode* existingNode = this->mStore.getValueOrNil < VersionedCollectionNode >( nodeKey );
    if (( !existingNode ) || ( existingNode->mID == INVALID_NODE_INDEX )) throw KeyNotFoundException ();
    
    size_t nodeID = existingNode->mID;
    
    if ( existingNode->mPrev != INVALID_NODE_INDEX ) {
        string prevNodeKey = this->mNodePrefix + encodeNodeID ( existingNode->mPrev );
        VersionedCollectionNode prevNode = this->mStore.getValue < VersionedCollectionNode >( prevNodeKey );
        prevNode.mNext = existingNode->mNext;
        this->mStore.setValue < VersionedCollectionNode >( prevNodeKey, prevNode );
    }
    else {
        this->mState.mHead = existingNode->mNext;
    }
    
    if ( existingNode->mNext != INVALID_NODE_INDEX ) {
        string nextNodeKey = this->mNodePrefix + encodeNodeID ( existingNode->mNext );
        VersionedCollectionNode nextNode = this->mStore.getValue < VersionedCollectionNode >( nextNodeKey );
        nextNode.mPrev = existingNode->mPrev;
        this->mStore.setValue < VersionedCollectionNode >( nextNodeKey, nextNode );
    }
    else {
        this->mState.mTail = existingNode->mPrev;
    }

    VersionedCollectionNode node;
    node.mPrev = prevID;
    node.mNext = INVALID_NODE_INDEX;
    node.mID = INVALID_NODE_INDEX;
    
    this->mStore.setValue < VersionedCollectionNode >( nodeKey, node );
    
    this->mState.mSize--;
    this->mStore.setValue < VersionedCollectionState >( this->mName, this->mState );
    
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
