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
// TODO: doxygen
class VersionedCollectionState {
private:

    friend class AbstractVersionedCollection;
    friend class MutableVersionedCollection;
    friend class VersionedMap;
    friend class VersionedSet;
    friend class VersionedCollectionSnapshot;
    friend class VersionedCollectionIterator;
    
    size_t      mHead;              // ID of head of active list
    size_t      mTail;              // ID of tail of active list
    size_t      mSize;              // total nodes in active list
};

//================================================================//
// VersionedSetNode
//================================================================//
// TODO: doxygen
class VersionedSetNode {
private:

    friend class AbstractVersionedCollection;
    friend class MutableVersionedCollection;
    friend class VersionedMap;
    friend class VersionedSet;
    friend class VersionedCollectionSnapshot;
    friend class VersionedCollectionIterator;
    
    size_t      mID;
    string      mKey;
    size_t      mPrev;              // ID of prev node in the list (active or free)
    size_t      mNext;              // ID of next node in the list (active or free)
};

//================================================================//
// AbstractVersionedCollection
//================================================================//
// TODO: doxygen
class AbstractVersionedCollection {
protected:

    static constexpr const char* SET_NODES_POSTFIX      = ".nodes.";
    static constexpr const char* SET_VALUES_POSTFIX     = ".values.";

    static const size_t         INVALID_NODE_INDEX = ( size_t )-1;

    string                      mNodePrefix;
    string                      mValuePrefix;

    string                      mMapName;
    
    VersionedCollectionState    mState;
    
    //----------------------------------------------------------------//
    const VersionedStoreSnapshot&       getSnapshot             () const;
    void                                pushNode                ( size_t nodeID, string nodeKey );
    static string                       nodeIDToString          ( size_t nodeID );
    void                                setName                 ( string name );
    
    //----------------------------------------------------------------//
    virtual const VersionedStoreSnapshot&       AbstractVersionedCollection_getSnapshot         () const = 0;
    
public:

    //----------------------------------------------------------------//
    operator const VersionedStoreSnapshot& () const {
    
        return this->AbstractVersionedCollection_getSnapshot ();
    }

    //----------------------------------------------------------------//
                    AbstractVersionedCollection         ();
    virtual         ~AbstractVersionedCollection        ();
    string          getName                             () const;
    size_t          getSize                             () const;
    
    //----------------------------------------------------------------//
    // TODO: doxygen
    template < typename TYPE >
    TYPE getValue ( string key ) const {
        
        return this->AbstractVersionedCollection_getSnapshot ().getValue < TYPE >( this->mValuePrefix + key );
    }
};

} // namespace Padamose
#endif
