// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_DEBUGSTRINGSTORE_H
#define PADAMOSE_DEBUGSTRINGSTORE_H

#include <padamose/padamose-common.h>
#include <padamose/AbstractStringStore.h>

namespace Padamose {

//================================================================//
// DebugStringStore
//================================================================//
// TODO: doxygen
class DebugStringStore :
    public AbstractStringStore {
protected:

    map < string, string > mStrings;

    //----------------------------------------------------------------//
    void            AbstractPersistenceProvider_begin       () override;
    void            AbstractPersistenceProvider_commit      () override;
    void            AbstractStringStore_eraseString         ( string key ) override;
    string          AbstractStringStore_getString           ( string key ) const override;
    bool            AbstractStringStore_hasString           ( string key ) const override;
    void            AbstractStringStore_setString           ( string key, string value ) override;

public:

    //----------------------------------------------------------------//
                    DebugStringStore        ( string prefix = "" );
    virtual         ~DebugStringStore       ();
    void            dump                    () const;
};

} // namespace Padamose
#endif
