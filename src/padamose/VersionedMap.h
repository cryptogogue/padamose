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
    
    //----------------------------------------------------------------//
    size_t          affirmKey                   ( string key );
    static size_t   getNodeID                   ( string key );
    string          provisionKey                ();
    
public:

    //----------------------------------------------------------------//
    void            deleteKey                   ( string key );
    bool            hasKey                      ( string key ) const;
                    VersionedMap                ( VersionedStore& store, string mapName );
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
