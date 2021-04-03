// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_VERSIONEDSTOREITERATOR_H
#define PADAMOSE_VERSIONEDSTOREITERATOR_H

#include <padamose/padamose-common.h>
#include <padamose/ConstVersionedStoreTag.h>

namespace Padamose {

class AbstractValueStack;

//================================================================//
// VersionedStoreIterator
//================================================================//
/** \brief Implements a version-by-version iterator of a value store.

    The iterator can traverse from the current version of the value
    stack used to initialize it back to version zero. Iteration works
    in both directions, though iterating forward may incur additional
    overhead in cases where multiple branches exist.
 
    When the iterator is initialized, an "anchor" snapshot is created.
    This ensures that the values being iterated will not change, even
    if the initializing snapshot is rewound and modified. In other words,
    the iterator is itself a snapshot of the entire version history.
 
    If the iterator is iterated past the beginning or end of its
    range, it will become invalid. To restore it, step the iterator
    in the opposite direction.
*/
class VersionedStoreIterator :
    public ConstVersionedStoreTag {
protected:

    enum {
        VALID,
        EMPTY,
        
        // these are only set *after* a call to prev() or next().
        // they are not meant to be exposed or for general use.
        NO_PREV,
        NO_NEXT,
    };

    /// The anchor snapshot.
    ConstVersionedStoreTag      mAnchor;
    
    /// Internal state of the iterator.
    int                         mState;
    
    /// Used internally to bound iteration to the current branch.
    size_t                      mTopVersion;

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
    VersionedStoreIterator& operator++ () {
        this->next ();
        return *this;
    }
    
    //----------------------------------------------------------------//
    /** \brief Prefix decrement operator. Calls prev() internally.
    */
    VersionedStoreIterator& operator-- () {
        this->prev ();
        return *this;
    }
    
    //----------------------------------------------------------------//
    bool                isCurrent                           () const;
    bool                isValid                             () const;
    bool                next                                ();
    bool                prev                                ();
    void                seek                                ( size_t version );
                        VersionedStoreIterator              ( const ConstVersionedStoreTag& versionedStore );
                        VersionedStoreIterator              ( const ConstVersionedStoreTag& versionedStore, size_t version );
    virtual             ~VersionedStoreIterator             ();
};

} // namespace Padamose
#endif
