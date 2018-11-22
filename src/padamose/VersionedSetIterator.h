// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_VERSIONEDSETITERATOR_H
#define PADAMOSE_VERSIONEDSETITERATOR_H

#include <padamose/padamose-common.h>
#include <padamose/VersionedSetSnapshot.h>

namespace Padamose {

//================================================================//
// VersionedSetIterator
//================================================================//
// TODO: doxygen
class VersionedSetIterator :
    public VersionedSetSnapshot {
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
    VersionedSetIterator& operator++ () {
        this->next ();
        return *this;
    }
    
    //----------------------------------------------------------------//
    /** \brief Prefix decrement operator. Calls prev() internally.
    */
    VersionedSetIterator& operator-- () {
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
                        VersionedSetIterator                ( const AbstractVersionedSet& versionedSet );
                        VersionedSetIterator                ( const VersionedStoreSnapshot& snapshot, string mapName );
                        ~VersionedSetIterator               ();
    
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
