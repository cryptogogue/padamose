// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_ABSTRACTVERSIONEDCOLLECTION_H
#define PADAMOSE_ABSTRACTVERSIONEDCOLLECTION_H

#include <padamose/padamose-common.h>
#include <padamose/VersionedStore.h>

namespace Padamose {

//================================================================//
// VersionedCollectionState
//================================================================//
/** \brief  Persistent state object common to all collections. It tracks
            the head and tail of the doubly linked list of active key nodes
            (VersionedCollectionNode). It also tracks the size of the list.
*/
class VersionedCollectionState {
private:

    friend class BaseVersionedCollectionIterator;
    friend class AbstractVersionedCollection;
    friend class MutableVersionedCollection;
    friend class VersionedMap;
    friend class VersionedSet;
    friend class VersionedCollectionSnapshot;
    
    /// Numeric ID of the head of the list of active keys.
    size_t      mHead;
    
    /// Numeric ID of the tail of the list of active keys.
    size_t      mTail;
    
    /// Count of total active keys in the list.
    size_t      mSize;
};

//================================================================//
// VersionedCollectionNode
//================================================================//
/** \brief  Linked list node containing a key and its numeric ID. Nodes are
            addressed by numeric ID, which may be assigned and reused (as in
            VersionedSet) or derived from hashing heys (as in VersionedMap).
*/
class VersionedCollectionNode {
private:

    friend class BaseVersionedCollectionIterator;
    friend class AbstractVersionedCollection;
    friend class MutableVersionedCollection;
    friend class VersionedMap;
    friend class VersionedSet;
    friend class VersionedCollectionSnapshot;
    
    /// Numeric ID of the node.
    size_t      mID;
    
    /// The key tracked by the node.
    string      mKey;
    
    /// Numeric ID of previous node in the list.
    size_t      mPrev;
    
    /// Numeric ID of next node in the list.
    size_t      mNext;
};

//================================================================//
// AbstractVersionedCollection
//================================================================//
/** \brief  Abstract base class for versioned collections and versioned
            collection snapshots. Collections are iterable. This is achieved by
            internally maintainting a doubly linked list of keys. The linked
            list is also held in the versioned store.
*/
class AbstractVersionedCollection {
protected:

    /// Postfix for node look up. Lookup key is: <collection name>SET_NODES_POSTFIX<string encoded node ID>
    static constexpr const char* SET_NODES_POSTFIX      = ".nodes.";
    
    /// Postfix for value look up. Lookup key is: <collection name>SET_VALUES_POSTFIX<key string>
    static constexpr const char* SET_VALUES_POSTFIX     = ".values.";

    /// Used to designate an invalid node ID. Needed to terminate lists.
    static const size_t INVALID_NODE_INDEX = ( size_t )-1;

    /// Prefix string of list node keys. Append string encoded node IDs to look up nodes.
    string      mNodePrefix;
    
    /// Prefix string of values. Append key strings to look up values.
    string      mValuePrefix;

    /// Name of the collection in the VersionedStore.
    string      mName;
    
    /// State of the collection persisted in the VersionedStore.
    VersionedCollectionState    mState;
    
    //----------------------------------------------------------------//
    static string                       encodeNodeID            ( size_t nodeID );
    const VersionedStoreSnapshot&       getSnapshot             () const;
    void                                pushNode                ( size_t nodeID, string nodeKey );
    void                                setName                 ( string name );
    
    //----------------------------------------------------------------//
    virtual const VersionedStoreSnapshot&       AbstractVersionedCollection_getSnapshot         () const = 0;
    
public:

    //----------------------------------------------------------------//
    /** \brief  Cast operator to return the VersionedStoreSnapshot holding
                the collection.
        
        \return The VersionedStoreSnapshot holding the collection.
    */
    operator const VersionedStoreSnapshot& () const {
    
        return this->getSnapshot ();
    }

    //----------------------------------------------------------------//
                    AbstractVersionedCollection         ();
    virtual         ~AbstractVersionedCollection        ();
    string          getName                             () const;
    size_t          getSize                             () const;
    
    //----------------------------------------------------------------//
    /** \brief  Return a copy of the value for a key in the collection. Throws a
                KeyNotFoundException exception if the key does not exist.
     
                This function does not test to see if the key is active. Since
                keys can persist in the versioned store indefinitely, may return
                values for keys even after they have been "deleted" from the
                collection's iterable list.
     
        \param  key                     The key.
        \return                         A copy of the value.
        \throws KeyNotFoundException    No value was be found for the given key.
    */
    template < typename TYPE >
    TYPE getValue ( string key ) const {
        
        return this->AbstractVersionedCollection_getSnapshot ().getValue < TYPE >( this->mValuePrefix + key );
    }
};

} // namespace Padamose
#endif
