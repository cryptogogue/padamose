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

    this->mFreeStack.mTop = this->removeNode ( key, this->mFreeStack.mTop );
    this->mStore.setValue < VersionedSetFreeStack >( this->mFreeStackKey, this->mFreeStack );
}

//----------------------------------------------------------------//
// TODO: doxygen
string VersionedSet::provisionKey () {
    
    // grab the node ID from the free stack, or create a new key
    size_t nodeID = ( this->mFreeStack.mTop == INVALID_NODE_INDEX ) ? this->mFreeStack.mTotalNodes++ : this->mFreeStack.mTop;
    string key = nodeIDToString ( nodeID );
    string nodeKey = this->mNodePrefix + key;
    
    if ( this->mFreeStack.mTop != INVALID_NODE_INDEX ) {
        VersionedSetNode node = this->mStore.getValue < VersionedSetNode >( nodeKey );
        this->mFreeStack.mTop = node.mPrev;
    }
    
    this->insertNode ( nodeID, key, nodeKey );
    this->mStore.setValue < VersionedSetFreeStack >( this->mFreeStackKey, this->mFreeStack );

    return key;
}

//----------------------------------------------------------------//
// TODO: doxygen
VersionedSet::VersionedSet ( VersionedStore& store, string mapName ) :
    MutableVersionedCollection ( store, mapName ) {

    this->mFreeStackKey = this->mMapName + SET_FREE_STACK_POSTFIX;

    if ( this->affirmState ()) {
        this->mFreeStack = this->mStore.getValue < VersionedSetFreeStack >( this->mFreeStackKey );
    }
    else {
        this->mFreeStack.mTop = INVALID_NODE_INDEX;
        this->mFreeStack.mTotalNodes = 0;
        
        this->mStore.setValue < VersionedSetFreeStack >( this->mFreeStackKey, this->mFreeStack );
    }
}

//----------------------------------------------------------------//
// TODO: doxygen
VersionedSet::~VersionedSet () {
}

} // namespace Padamose
