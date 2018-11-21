// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <padamose/VersionedSet.h>

namespace Padamose {

//================================================================//
// VersionedStore
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
void VersionedSet::deleteKey ( string key ) {

    string nodeKey = this->mNodePrefix + key;
    VersionedSetNode node = this->mStore.getValue < VersionedSetNode >( nodeKey );
    
    if ( node.mPrev != INVALID_NODE_INDEX ) {
        string prevNodeKey = this->mNodePrefix + to_string ( node.mPrev );
        VersionedSetNode prevNode = this->mStore.getValue < VersionedSetNode >( prevNodeKey );
        prevNode.mNext = node.mNext;
        this->mStore.setValue < VersionedSetNode >( prevNodeKey, prevNode );
    }
    else {
    
        // this is the first node, so we need tp update the list head.
        this->mState.mList = node.mNext;
    }
    
    if ( node.mNext != INVALID_NODE_INDEX ) {
        string nextNodeKey = this->mNodePrefix + to_string ( node.mNext );
        VersionedSetNode nextNode = this->mStore.getValue < VersionedSetNode >( nextNodeKey );
        nextNode.mPrev = node.mPrev;
        this->mStore.setValue < VersionedSetNode >( nextNodeKey, nextNode );
    }

    node.mPrev = this->mState.mFreeStack;
    node.mNext = INVALID_NODE_INDEX;
    this->mState.mFreeStack = node.mID;
    node.mID = INVALID_NODE_INDEX;
    
    this->mStore.setValue < VersionedSetNode >( nodeKey, node );
    
    this->mState.mSize--;
    this->setValue < VersionedSetState >( this->mMapName, this->mState );
}

//----------------------------------------------------------------//
// TODO: doxygen
string VersionedSet::provisionKey () {

    size_t nodeID;
    
    // grab the node ID from the free stack, or create a new key
    if ( this->mState.mFreeStack == INVALID_NODE_INDEX ) {
        nodeID = this->mState.mTotalNodes++;
    }
    else {
        nodeID = this->mState.mFreeStack;
    }
    
    string key = to_string ( nodeID );
    string nodeKey = this->mNodePrefix + key;
    
    if ( this->mState.mFreeStack != INVALID_NODE_INDEX ) {
        VersionedSetNode node = this->mStore.getValue < VersionedSetNode >( nodeKey );
        this->mState.mFreeStack = node.mPrev;
    }
    
    size_t nextID = this->mState.mList;
    
    VersionedSetNode node;
    node.mID = nodeID;
    node.mPrev = INVALID_NODE_INDEX;
    node.mNext = nextID;
    this->mStore.setValue < VersionedSetNode >( nodeKey, node );

    // if there is a next node, update that node's prev.
    if ( nextID != INVALID_NODE_INDEX ) {
        string nextNodeKey = this->mNodePrefix + to_string ( nextID );
        VersionedSetNode nextNode = this->mStore.getValue < VersionedSetNode >( nextNodeKey );
        nextNode.mPrev = nodeID;
        this->mStore.setValue < VersionedSetNode >( nextNodeKey, nextNode );
    }

    // update the list.
    this->mState.mList = nodeID;
    this->mState.mSize++;
    this->mStore.setValue < VersionedSetState >( this->mMapName, this->mState );

    return key;
}

//----------------------------------------------------------------//
// TODO: doxygen
VersionedSet::VersionedSet ( VersionedStore& store, string mapName ) :
    mStore ( store ) {

    this->setName ( mapName );

    if ( this->mStore.hasKey ( this->mMapName )) {
        this->mState = this->mStore.getValue < VersionedSetState >( this->mMapName );
    }
    else {
        if ( this->mMapName.find ( ':' ) != string::npos ) throw InvalidMapNameException ();
        
        this->mState.mList = INVALID_NODE_INDEX;
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
VersionedSet::~VersionedSet () {
}

//================================================================//
// overrides
//================================================================//

//----------------------------------------------------------------//
const VersionedStoreSnapshot& VersionedSet::AbstractVersionedSetSnapshot_getSnapshot () const {

    return this->mStore;
}

} // namespace Padamose
