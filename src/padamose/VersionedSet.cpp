// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <padamose/VersionedSet.h>

namespace Padamose {

//================================================================//
// VersionedStore
//================================================================//

//----------------------------------------------------------------//
/** \brief  Removes the key from the active list and pushes it onto the
            free stack for later reuse.

    \param      key         They key to remove.
 
    \throws     KeyNotFoundException    The key could not be found.
*/
void VersionedSet::deleteKey ( string key ) {

    this->mFreeStack.mTop = this->removeNode ( key, key, this->mFreeStack.mTop );
    this->mStore.setValue < VersionedSetFreeStack >( this->mFreeStackKey, this->mFreeStack );
}

//----------------------------------------------------------------//
/** \brief  Companion to provisionKey(). Returns the numerid ID of the next
            node that will be provisioned.

    \return                 The numeric ID of the next node that provisionKey() will provision.
*/
size_t VersionedSet::peekNextKey () {

   return ( this->mFreeStack.mTop == INVALID_NODE_INDEX ) ? this->mFreeStack.mTotalNodes : this->mFreeStack.mTop;
}

//----------------------------------------------------------------//
/** \brief  Adds a key to the beginning or end of the list. The key may
            be reused, or will be created if no unused keys are available.

    \param      nextNodeID      The numeric ID of the node expected to be provisioned. Used for a sanity check.
    \param      append          If TRUE, key will be appended. If false, key will be prepended.
    \return                     The provisioned key.
*/
string VersionedSet::provisionKey ( size_t nextNodeID, bool append ) {
    
    // grab the node ID from the free stack, or create a new key
    size_t nodeID = ( this->mFreeStack.mTop == INVALID_NODE_INDEX ) ? this->mFreeStack.mTotalNodes++ : this->mFreeStack.mTop;
    
    assert ( nodeID == nextNodeID );
    
    string key = encodeNodeID ( nodeID );
    string nodeKey = this->mNodePrefix + key;
    
    if ( this->mFreeStack.mTop != INVALID_NODE_INDEX ) {
        VersionedCollectionNode node = this->mStore.getValue < VersionedCollectionNode >( nodeKey );
        this->mFreeStack.mTop = node.mPrev;
    }
    
    this->mStore.setValue < VersionedSetFreeStack >( this->mFreeStackKey, this->mFreeStack );
    
    if ( append ) {
        this->appendNode ( nodeID, key, nodeKey );
    }
    else {
        this->prependNode ( nodeID, key, nodeKey );
    }

    return key;
}

//----------------------------------------------------------------//
/** \brief  Construct the collection in (or from) the given store.

    \param      store       The versioned store that contains (or will contain) the collection.
    \param      name        The name of the collection.
*/
VersionedSet::VersionedSet ( VersionedStore& store, string name ) :
    MutableVersionedCollection ( store, name ) {

    this->mFreeStackKey = this->mName + SET_FREE_STACK_POSTFIX;

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
VersionedSet::~VersionedSet () {
}

} // namespace Padamose
