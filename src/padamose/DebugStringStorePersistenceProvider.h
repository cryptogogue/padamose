// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_DEBUGSTRINGSTOREPERSISTENCEPROVIDER_H
#define PADAMOSE_DEBUGSTRINGSTOREPERSISTENCEPROVIDER_H

#include <padamose/padamose-common.h>
#include <padamose/AbstractStringStorePersistenceProvider.h>

namespace Padamose {

//================================================================//
// DebugStringStorePersistenceProvider
//================================================================//
// TODO: doxygen
class DebugStringStorePersistenceProvider :
    public AbstractStringStorePersistenceProvider {
protected:

    map < string, string > mStrings;

    //----------------------------------------------------------------//
    void            AbstractStringStore_eraseString     ( string key ) override;
    string          AbstractStringStore_getString       ( string key ) const override;
    bool            AbstractStringStore_hasString       ( string key ) const override;
    void            AbstractStringStore_setString       ( string key, string value ) override;

public:

    //----------------------------------------------------------------//
                    DebugStringStorePersistenceProvider     ();
    virtual         ~DebugStringStorePersistenceProvider    ();
    void            dump                                    () const;
};

} // namespace Padamose
#endif
