// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <padamose/VersionedSetIterator.h>

namespace Padamose {

//================================================================//
// VersionedSetSnapshot
//================================================================//

//----------------------------------------------------------------//
bool VersionedSetIterator::isValid () const {

    return ( this->mIteratorState == VALID );
}

//----------------------------------------------------------------//
bool VersionedSetIterator::next () {
    
    return this->step ( this->mIteratorNode.mNext, NO_NEXT, NO_PREV );
}

//----------------------------------------------------------------//
bool VersionedSetIterator::prev () {
    
    return this->step ( this->mIteratorNode.mPrev, NO_PREV, NO_NEXT );
}

//----------------------------------------------------------------//
void VersionedSetIterator::seek ( size_t nodeID ) {

    if ( this->mState.mSize == 0 ) {
        this->mIteratorState = EMPTY;
        return;
    }
    this->mIteratorNode = this->mSnapshot.getValue < VersionedSetNode >( this->mNodePrefix + to_string ( nodeID ));
    this->mIteratorState = VALID;
}

//----------------------------------------------------------------//
void VersionedSetIterator::seekBack () {
    
    this->seek ( this->mState.mTail );
}

//----------------------------------------------------------------//
void VersionedSetIterator::seekFront () {

    this->seek ( this->mState.mHead );
}

//----------------------------------------------------------------//
bool VersionedSetIterator::step ( size_t nextNodeID, int blockingState, int unblockingState ) {

    if ( this->mIteratorState == EMPTY ) return false;

    if ( this->mIteratorState == unblockingState ) {
        this->mIteratorState = VALID;
    }
    else if ( this->mIteratorState != blockingState ) {

        if ( nextNodeID == INVALID_NODE_INDEX ) {
            this->mIteratorState = blockingState;
        }
        else {
            this->mIteratorNode = this->mSnapshot.getValue < VersionedSetNode >( this->mNodePrefix + to_string ( nextNodeID ));
        }
    }
    return ( this->mIteratorState != blockingState );
}

//----------------------------------------------------------------//
VersionedSetIterator::VersionedSetIterator ( const AbstractVersionedSet& versionedSet ) :
    VersionedSetSnapshot ( versionedSet, versionedSet.getName ()) {
    
    this->seekFront ();
}

//----------------------------------------------------------------//
VersionedSetIterator::VersionedSetIterator ( const VersionedStoreSnapshot& snapshot, string mapName ) :
    VersionedSetSnapshot ( snapshot, mapName ) {
    
    this->seekFront ();
}

//----------------------------------------------------------------//
VersionedSetIterator::~VersionedSetIterator () {
}

} // namespace Padamose
