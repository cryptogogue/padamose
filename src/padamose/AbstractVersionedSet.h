// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_ABSTRACTVERSIONEDSET_H
#define PADAMOSE_ABSTRACTVERSIONEDSET_H

#include <padamose/padamose-common.h>
#include <padamose/VersionedStore.h>

namespace Padamose {

//================================================================//
// VersionedSetState
//================================================================//
// TODO: doxygen
class VersionedSetState {
private:

    friend class AbstractVersionedSet;
    friend class VersionedMap;
    friend class VersionedSet;
    friend class VersionedSetSnapshot;
    friend class VersionedSetIterator;
    
    size_t      mHead;              // ID of head of active list
    size_t      mTail;              // ID of tail of active list
    size_t      mSize;              // total nodes in active list
    
    size_t      mFreeStack;         // top of key free stack
    size_t      mTotalNodes;        // total nodes created
};

//================================================================//
// VersionedSetNode
//================================================================//
// TODO: doxygen
class VersionedSetNode {
private:

    friend class AbstractVersionedSet;
    friend class VersionedMap;
    friend class VersionedSet;
    friend class VersionedSetSnapshot;
    friend class VersionedSetIterator;
    
    size_t      mID;
    string      mKey;
    size_t      mPrev;              // ID of prev node in the list (active or free)
    size_t      mNext;              // ID of next node in the list (active or free)
};

//================================================================//
// AbstractVersionedSet
//================================================================//
// TODO: doxygen
class AbstractVersionedSet {
protected:

    static constexpr const char* SET_NODES_POSTFIX      = ".nodes.";
    static constexpr const char* SET_VALUES_POSTFIX     = ".values.";

    static const size_t         INVALID_NODE_INDEX = ( size_t )-1;

    string                      mNodePrefix;
    string                      mValuePrefix;

    string                      mMapName;
    
    VersionedSetState           mState;
    
    //----------------------------------------------------------------//
    void            pushNode                    ( size_t nodeID, string nodeKey );
    void            setName                     ( string name );
    
    //----------------------------------------------------------------//
    virtual const VersionedStoreSnapshot&       AbstractVersionedSet_getSnapshot        () const = 0;
    
public:

    //----------------------------------------------------------------//
    operator const VersionedStoreSnapshot& () const {
    
        return this->AbstractVersionedSet_getSnapshot ();
    }

    //----------------------------------------------------------------//
                    AbstractVersionedSet        ();
    virtual         ~AbstractVersionedSet       ();
    string          getName                     () const;
    size_t          getSize                     () const;
    
    //----------------------------------------------------------------//
    // TODO: doxygen
    template < typename TYPE >
    TYPE getValue ( string key ) const {
        
        return this->AbstractVersionedSet_getSnapshot ().getValue < TYPE >( this->mValuePrefix + key );
    }
};

} // namespace Padamose
#endif
