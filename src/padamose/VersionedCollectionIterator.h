// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_VERSIONEDCOLLECTIONITERATOR_H
#define PADAMOSE_VERSIONEDCOLLECTIONITERATOR_H

#include <padamose/padamose-common.h>
#include <padamose/VersionedCollectionSnapshot.h>

namespace Padamose {

//================================================================//
// VersionedCollectionIterator
//================================================================//
// TODO: doxygen
class VersionedCollectionIterator :
    public VersionedCollectionSnapshot {
protected:
    
    enum {
        VALID,
        EMPTY,
        
        // these are only set *after* a call to prev() or next().
        // they are not meant to be exposed or for general use.
        NO_PREV,
        NO_NEXT,
    };

    string              mKey;
    VersionedSetNode    mIteratorNode;
    int                 mIteratorState;
    
    //----------------------------------------------------------------//
    bool                step            ( size_t nextNodeID, int blockingState, int unblockingState );
    void                seek            ( size_t nodeID );
    
public:

    //----------------------------------------------------------------//
    /** \brief Operator overload for bool. False if the iterator is empty
        or has been iterated past its bounds. Calls isValid() internally.
    */
    operator bool () const {
        return this->isValid ();
    }
    
    //----------------------------------------------------------------//
    /** \brief Prefix increment operator. Calls next() internally.
    */
    VersionedCollectionIterator& operator++ () {
        this->next ();
        return *this;
    }
    
    //----------------------------------------------------------------//
    /** \brief Prefix decrement operator. Calls prev() internally.
    */
    VersionedCollectionIterator& operator-- () {
        this->prev ();
        return *this;
    }

    //----------------------------------------------------------------//
    bool                isValid                             () const;
    string              key                                 () const;
    bool                next                                ();
    bool                prev                                ();
    void                seekBack                            ();
    void                seekFront                           ();
                        VersionedCollectionIterator         ( const AbstractVersionedCollection& versionedSet );
                        VersionedCollectionIterator         ( const VersionedStoreSnapshot& snapshot, string mapName );
                        ~VersionedCollectionIterator        ();
    
    //----------------------------------------------------------------//
    // TODO: doxygen
    template < typename TYPE >
    const TYPE& value () const {
        
        const TYPE* value = this->mSnapshot.getValueOrNil < TYPE >( this->mValuePrefix + this->mIteratorNode.mKey );
        assert ( value );
        return *value;
    }
};

} // namespace Padamose
#endif
