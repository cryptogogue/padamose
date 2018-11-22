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

    if ( this->mIteratorState == EMPTY ) return false;

    if ( this->mIteratorState == NO_PREV ) {
        this->mIteratorState = VALID;
    }
    else if ( this->mIteratorState != NO_NEXT ) {

        if ( this->mIteratorNode.mNext == INVALID_NODE_INDEX ) {
            this->mIteratorState = NO_NEXT;
        }
        else {
            this->mIteratorNode = this->mSnapshot.getValue < VersionedSetNode >( this->mNodePrefix + to_string ( this->mIteratorNode.mNext ));
        }
    }
    return ( this->mIteratorState != NO_NEXT );
}

//----------------------------------------------------------------//
bool VersionedSetIterator::prev () {

    if ( this->mIteratorState == EMPTY ) return false;

    if ( this->mIteratorState == NO_NEXT ) {
        this->mIteratorState = VALID;
    }
    else if ( this->mIteratorState != NO_PREV ) {

        if ( this->mIteratorNode.mPrev == INVALID_NODE_INDEX ) {
            this->mIteratorState = NO_PREV;
        }
        else {
            this->mIteratorNode = this->mSnapshot.getValue < VersionedSetNode >( this->mNodePrefix + to_string ( this->mIteratorNode.mPrev ));
        }
    }
    return ( this->mIteratorState != NO_PREV );
}

//----------------------------------------------------------------//
void VersionedSetIterator::seekBack () {

    if ( this->mState.mSize == 0 ) {
        this->mIteratorState = EMPTY;
        return;
    }
    this->mIteratorNode = this->mSnapshot.getValue < VersionedSetNode >( this->mNodePrefix + to_string ( this->mState.mTail ));
    this->mIteratorState = VALID;
}

//----------------------------------------------------------------//
void VersionedSetIterator::seekFront () {

    if ( this->mState.mSize == 0 ) {
        this->mIteratorState = EMPTY;
        return;
    }
    this->mIteratorNode = this->mSnapshot.getValue < VersionedSetNode >( this->mNodePrefix + to_string ( this->mState.mHead ));
    this->mIteratorState = VALID;
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
