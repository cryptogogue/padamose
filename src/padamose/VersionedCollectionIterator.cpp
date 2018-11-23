// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <padamose/VersionedCollectionIterator.h>

namespace Padamose {

//================================================================//
// VersionedCollectionSnapshot
//================================================================//

//----------------------------------------------------------------//
/** \brief Returns true if iterator is in the valid range for iteration.

    \return     True if iteration is in the valid range.
*/
bool VersionedCollectionIterator::isValid () const {

    return ( this->mIteratorState == VALID );
}

//----------------------------------------------------------------//
/** \brief Return the key contained in the current node.

    \return     The key contained in the current node.
*/
string VersionedCollectionIterator::key () const {

    return this->mIteratorNode.mKey;
}

//----------------------------------------------------------------//
/** \brief Iterates forward by one node.

    \return     True if the step was successful.
*/
bool VersionedCollectionIterator::next () {
    
    return this->step ( this->mIteratorNode.mNext, NO_NEXT, NO_PREV );
}

//----------------------------------------------------------------//
/** \brief Iterates backward by one node.

    \return     True if the step was successful.
*/
bool VersionedCollectionIterator::prev () {
    
    return this->step ( this->mIteratorNode.mPrev, NO_PREV, NO_NEXT );
}

//----------------------------------------------------------------//
/** \brief  Seek to the given node.

    \param      nodeID      Numeric ID of the node to seek to.
*/
void VersionedCollectionIterator::seek ( size_t nodeID ) {

    if ( this->mState.mSize == 0 ) {
        this->mIteratorState = EMPTY;
        return;
    }
    this->mNodeKey = this->mNodePrefix + encodeNodeID ( nodeID );
    this->mIteratorNode = this->mSnapshot.getValue < VersionedCollectionNode >( this->mNodeKey );
    this->mIteratorState = VALID;
}

//----------------------------------------------------------------//
/** \brief  Reset iteration to the back of the list.
*/
void VersionedCollectionIterator::seekBack () {
    
    this->seek ( this->mState.mTail );
}

//----------------------------------------------------------------//
/** \brief  Reset iteration to the front of the list.
*/
void VersionedCollectionIterator::seekFront () {

    this->seek ( this->mState.mHead );
}

//----------------------------------------------------------------//
/** \brief  Steps the iterator forward or backward. This is a generic
            implemented used interfally by prev() and next().

    \param      nextNodeID          Numeric ID of the node to step to.
    \param      blockingState       State to block against the intended step direction (prev of next).
    \param      unblockingState     State to unblock from if stepping away (in the oppositer direction).
 
    \return                         The step was completed and not blocked.
*/
bool VersionedCollectionIterator::step ( size_t nextNodeID, int blockingState, int unblockingState ) {

    if ( this->mIteratorState == EMPTY ) return false;

    if ( this->mIteratorState == unblockingState ) {
        this->mIteratorState = VALID;
    }
    else if ( this->mIteratorState != blockingState ) {

        if ( nextNodeID == INVALID_NODE_INDEX ) {
            this->mIteratorState = blockingState;
        }
        else {
            this->mNodeKey = this->mNodePrefix + encodeNodeID ( nextNodeID );
            this->mIteratorNode = this->mSnapshot.getValue < VersionedCollectionNode >( this->mNodeKey );
        }
    }
    return ( this->mIteratorState != blockingState );
}

//----------------------------------------------------------------//
/** \brief  Intitialize the iterator with an existing collection.

    \param      collection      The existing collection.
*/
VersionedCollectionIterator::VersionedCollectionIterator ( const AbstractVersionedCollection& versionedSet ) :
    VersionedCollectionSnapshot ( versionedSet, versionedSet.getName ()) {
    
    this->seekFront ();
}

//----------------------------------------------------------------//
/** \brief  Initialize the base snapshot for iteration. Iteration begins
            at the front of the list. Internally calls the constructor
            for VersionedCollectionSnapshot.

    \param      store       The versioned store that contains the collection.
    \param      name        The name of the collection.
 
    \throws     VersionedCollectionNotFoundException    No valid state exists for the collection.
*/
VersionedCollectionIterator::VersionedCollectionIterator ( const VersionedStoreSnapshot& snapshot, string name ) :
    VersionedCollectionSnapshot ( snapshot, name ) {
    
    this->seekFront ();
}

//----------------------------------------------------------------//
VersionedCollectionIterator::~VersionedCollectionIterator () {
}

} // namespace Padamose
