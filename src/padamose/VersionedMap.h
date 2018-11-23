// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_VERSIONEDMAP_H
#define PADAMOSE_VERSIONEDMAP_H

#include <padamose/padamose-common.h>
#include <padamose/MutableVersionedCollection.h>

namespace Padamose {

//================================================================//
// VersionedMap
//================================================================//
// TODO: doxygen
class VersionedMap :
    public MutableVersionedCollection {
private:
    
    static constexpr const char* COLLISION_POSTFIX          = ".collision.";
    static constexpr const char* DECOLLIDER_POSTFIX         = ".decollider.";
    
    static constexpr const size_t HASH_PORTION_MASK         = 0x0000ffffffffffff;
    static constexpr const size_t COUNTER_PORTION_SHIFT     = 48;
    static constexpr const size_t COUNTER_PORTION_MAX       = 0x7fff;
    
    string          mCollisionPrefix;
    string          mDecolliderPrefix;
    
    //----------------------------------------------------------------//
    size_t          affirmKey                   ( string key );
    size_t          getHashPortion              ( string key ) const;
    string          provisionKey                ();
    
    //----------------------------------------------------------------//
    virtual size_t  VersionedMap_hash           ( string key ) const;
    
public:

    //----------------------------------------------------------------//
    void            deleteKey                   ( string key );
    bool            hasKey                      ( string key ) const;
                    VersionedMap                ( VersionedStore& store, string name );
                    ~VersionedMap               ();
    
    //----------------------------------------------------------------//
    // TODO: doxygen
    template < typename TYPE >
    void setValue ( string key, const TYPE& value ) {
    
        this->affirmKey ( key );
        this->mStore.setValue < TYPE >( this->mValuePrefix + key, value );
    }
};

} // namespace Padamose
#endif
