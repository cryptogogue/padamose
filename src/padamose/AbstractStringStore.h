// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_ABSTRACTSTRINGSTORE_H
#define PADAMOSE_ABSTRACTSTRINGSTORE_H

#include <padamose/padamose-common.h>
#include <padamose/Variant.h>

namespace Padamose {

//================================================================//
// AbstractStringStore
//================================================================//
// TODO: doxygen
class AbstractStringStore {
protected:

    //----------------------------------------------------------------//
    virtual string      AbstractStringStore_getString       ( string key ) const = 0;
    virtual bool        AbstractStringStore_hasString       ( string key ) const = 0;
    virtual void        AbstractStringStore_setString       ( string key, string value ) = 0;

public:

    //----------------------------------------------------------------//
                        AbstractStringStore                 ();
    virtual             ~AbstractStringStore                ();
    string              getString                           ( string key ) const;
    bool                hasString                           ( string key ) const;
    void                setString                           ( string key, string value );
    
    //----------------------------------------------------------------//
    template < typename TYPE >
    TYPE get ( string key, const TYPE value ) const {
        if ( this->hasString ( key )) {
            Variant var ( this->getString ( key ));
            return var.get < TYPE >();
        }
        return value;
    }
    
    //----------------------------------------------------------------//
    template < typename TYPE >
    void set ( string key, const TYPE value ) {
        Variant var ( value );
        this->setString ( key, var.get < string >());
    }
};

} // namespace Padamose
#endif
