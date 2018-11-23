// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <padamose/MutableVersionedCollection.h>

namespace Padamose {

//================================================================//
// MutableVersionedCollection
//================================================================//

//----------------------------------------------------------------//
/** \brief  Load or create a state record for the collection. Returns
            TRUE if the state already exists and FALSE if it did not
            (and had to be created and stored).

    \return TRUE if the state exists and was loaded. FALSE if the state was created and stored.
*/
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
/** \brief  Insert a node (and its key) into the collection's list of active nodes.

    \param      nodeID      The numeric ID of the node.
    \param      key         The key to be tracked by the node.
    \param      nodeKey     The full key of the node in the versioned store.
*/
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
/** \brief  Construct the collection in (or from) the given store.

    \param      store       The versioned store that contains (or will contain) the collection.
    \param      name        The name of the collection.
*/
MutableVersionedCollection::MutableVersionedCollection ( VersionedStore& store, string name ) :
    mStore ( store ) {

    this->setName ( name );
}

//----------------------------------------------------------------//
MutableVersionedCollection::~MutableVersionedCollection () {
}

//----------------------------------------------------------------//
/** \brief  Remove a node (and its key) from the collection's list of active nodes.

    \param      key         The encoded node ID.
    \param      prevID      A numeric ID to assign to the node's prev. Default is INVALID_NODE_INDEX.
    \return                 The numeric ID of the node removed.
*/
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
/** \brief  Implementation of the pure virtual base method to return
            the versioned snapshot associated with the container.

    \return The VersionedStore (as a VersionedStoreSnapshot).
*/
const VersionedStoreSnapshot& MutableVersionedCollection::AbstractVersionedCollection_getSnapshot () const {

    return this->mStore;
}

} // namespace Padamose
