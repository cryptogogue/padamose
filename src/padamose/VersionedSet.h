// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_VERSIONEDSET_H
#define PADAMOSE_VERSIONEDSET_H

#include <padamose/padamose-common.h>
#include <padamose/MutableVersionedCollection.h>

namespace Padamose {

//================================================================//
// VersionedSetState
//================================================================//
// TODO: doxygen
class VersionedSetFreeStack {
private:

    friend class VersionedSet;
    
    size_t      mTop;               // top of key free stack
    size_t      mTotalNodes;        // total nodes created
};

//================================================================//
// VersionedSet
//================================================================//
// TODO: doxygen
class VersionedSet :
    public MutableVersionedCollection {
private:
    
    static constexpr const char* SET_FREE_STACK_POSTFIX     = ".free";
    
    string                  mFreeStackKey;
    VersionedSetFreeStack   mFreeStack;
    
    //----------------------------------------------------------------//
    string          provisionKey                ();
    
public:

    //----------------------------------------------------------------//
    void            deleteKey                   ( string key );
                    VersionedSet                ( VersionedStore& store, string name );
                    ~VersionedSet               ();
    
    //----------------------------------------------------------------//
    // TODO: doxygen
    template < typename TYPE >
    string insertValue ( const TYPE& value ) {
    
        string key = this->provisionKey ();
        this->setValue < TYPE >( key, value );
        return key;
    }
    
    //----------------------------------------------------------------//
    // TODO: doxygen
    template < typename TYPE >
    void setValue ( string key, const TYPE& value ) {
    
        this->mStore.setValue < TYPE >( this->mValuePrefix + key, value );
    }
};

} // namespace Padamose
#endif
