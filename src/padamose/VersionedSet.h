// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_VERSIONEDSET_H
#define PADAMOSE_VERSIONEDSET_H

#include <padamose/padamose-common.h>
#include <padamose/AbstractVersionedSetSnapshot.h>

namespace Padamose {

//================================================================//
// VersionedSet
//================================================================//
// TODO: doxygen
class VersionedSet :
    public AbstractVersionedSetSnapshot {
private:

    VersionedStore&             mStore;
    
    //----------------------------------------------------------------//
    string          provisionKey                ();
    
    //----------------------------------------------------------------//
    const VersionedStoreSnapshot&       AbstractVersionedSetSnapshot_getSnapshot        () const override;
    
public:

    //----------------------------------------------------------------//
    void            deleteKey                   ( string key );
                    VersionedSet                ( VersionedStore& store, string mapName );
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
    
        this->setValue < TYPE >( this->mValuePrefix + key, value );
    }
};

} // namespace Padamose
#endif
