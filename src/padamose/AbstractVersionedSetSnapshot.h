// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_ABSTRACTVERSIONEDSETSNAPSHOT_H
#define PADAMOSE_ABSTRACTVERSIONEDSETSNAPSHOT_H

#include <padamose/padamose-common.h>
#include <padamose/VersionedStore.h>

namespace Padamose {

//================================================================//
// VersionedSetState
//================================================================//
// TODO: doxygen
class VersionedSetState {
private:

    friend class AbstractVersionedSetSnapshot;
    friend class VersionedSet;
    friend class VersionedSetSnapshot;
    
    size_t      mList;              // ID of head of active list
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

    friend class AbstractVersionedSetSnapshot;
    friend class VersionedSet;
    friend class VersionedSetSnapshot;
    
    size_t      mID;
    size_t      mPrev;              // ID of prev node in the list (active or free)
    size_t      mNext;              // ID of next node in the list (active or free)
};

//================================================================//
// AbstractVersionedSetSnapshot
//================================================================//
// TODO: doxygen
class AbstractVersionedSetSnapshot {
protected:

    static constexpr const char* SET_NODES_POSTFIX      = ".nodes.";
    static constexpr const char* SET_VALUES_POSTFIX     = ".values.";

    static const size_t         INVALID_NODE_INDEX = ( size_t )-1;

    string                      mNodePrefix;
    string                      mValuePrefix;

    string                      mMapName;
    
    VersionedSetState           mState;
    
    //----------------------------------------------------------------//
    void            setName                     ( string name );
    
    //----------------------------------------------------------------//
    virtual const VersionedStoreSnapshot&       AbstractVersionedSetSnapshot_getSnapshot        () const;
    
public:

    //----------------------------------------------------------------//
                    AbstractVersionedSetSnapshot        ();
    virtual         ~AbstractVersionedSetSnapshot       ();
    size_t          getSize                             () const;
    
    //----------------------------------------------------------------//
    // TODO: doxygen
    template < typename TYPE >
    TYPE getValue ( string key ) const {
        
        return this->AbstractVersionedSetSnapshot_getSnapshot ().getValue < TYPE >( this->mValuePrefix + key );
    }
};

} // namespace Padamose
#endif
