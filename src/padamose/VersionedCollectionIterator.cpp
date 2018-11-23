// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <padamose/VersionedCollectionIterator.h>

namespace Padamose {

//================================================================//
// VersionedCollectionSnapshot
//================================================================//

//----------------------------------------------------------------//
bool VersionedCollectionIterator::isValid () const {

    return ( this->mIteratorState == VALID );
}

//----------------------------------------------------------------//
string VersionedCollectionIterator::key () const {

    return this->mKey;
}

//----------------------------------------------------------------//
bool VersionedCollectionIterator::next () {
    
    return this->step ( this->mIteratorNode.mNext, NO_NEXT, NO_PREV );
}

//----------------------------------------------------------------//
bool VersionedCollectionIterator::prev () {
    
    return this->step ( this->mIteratorNode.mPrev, NO_PREV, NO_NEXT );
}

//----------------------------------------------------------------//
void VersionedCollectionIterator::seek ( size_t nodeID ) {

    if ( this->mState.mSize == 0 ) {
        this->mIteratorState = EMPTY;
        return;
    }
    this->mKey = this->mNodePrefix + nodeIDToString ( nodeID );
    this->mIteratorNode = this->mSnapshot.getValue < VersionedSetNode >( this->mKey );
    this->mIteratorState = VALID;
}

//----------------------------------------------------------------//
void VersionedCollectionIterator::seekBack () {
    
    this->seek ( this->mState.mTail );
}

//----------------------------------------------------------------//
void VersionedCollectionIterator::seekFront () {

    this->seek ( this->mState.mHead );
}

//----------------------------------------------------------------//
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
            this->mKey = this->mNodePrefix + nodeIDToString ( nextNodeID );
            this->mIteratorNode = this->mSnapshot.getValue < VersionedSetNode >( this->mKey );
        }
    }
    return ( this->mIteratorState != blockingState );
}

//----------------------------------------------------------------//
VersionedCollectionIterator::VersionedCollectionIterator ( const AbstractVersionedCollection& versionedSet ) :
    VersionedCollectionSnapshot ( versionedSet, versionedSet.getName ()) {
    
    this->seekFront ();
}

//----------------------------------------------------------------//
VersionedCollectionIterator::VersionedCollectionIterator ( const VersionedStoreSnapshot& snapshot, string mapName ) :
    VersionedCollectionSnapshot ( snapshot, mapName ) {
    
    this->seekFront ();
}

//----------------------------------------------------------------//
VersionedCollectionIterator::~VersionedCollectionIterator () {
}

} // namespace Padamose
