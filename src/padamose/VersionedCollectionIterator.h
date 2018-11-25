// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_VERSIONEDCOLLECTIONITERATOR_H
#define PADAMOSE_VERSIONEDCOLLECTIONITERATOR_H

#include <padamose/padamose-common.h>
#include <padamose/BaseVersionedCollectionIterator.h>

namespace Padamose {

//================================================================//
// BaseVersionedCollectionIterator
//================================================================//
/** \brief  Implements an interator of the collection. The iterator is
            also a snapshot of the collection, so the iteration will
            persist even if the associated collection is changed.
*/
template < typename TYPE >
class VersionedCollectionIterator :
    public BaseVersionedCollectionIterator {
public:

    //----------------------------------------------------------------//
    /** \brief Dereference operator. Calls value() internally.
    */
    const TYPE& operator * () const {
        return this->value ();
    }

    //----------------------------------------------------------------//
    VersionedCollectionIterator ( const AbstractVersionedCollection& versionedSet ) :
        BaseVersionedCollectionIterator ( versionedSet ) {
    }
    
    VersionedCollectionIterator ( const VersionedStoreSnapshot& snapshot, string name ) :
        BaseVersionedCollectionIterator ( snapshot, name ) {
    }
    
    //----------------------------------------------------------------//
    ~VersionedCollectionIterator () {
    }
    
    //----------------------------------------------------------------//
    /** \brief Returns a const reference to the current value being iterated.

        \return     The current value of the iterator.
    */
    const TYPE& value () const {
        
        return this->BaseVersionedCollectionIterator::value < TYPE >();
    }
};

} // namespace Padamose
#endif
