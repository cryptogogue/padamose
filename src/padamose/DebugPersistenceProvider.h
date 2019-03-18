// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_DEBUGPERSISTENCEPROVIDER_H
#define PADAMOSE_DEBUGPERSISTENCEPROVIDER_H

#include <padamose/padamose-common.h>

namespace Padamose {

//================================================================//
// DebugPersistenceProvider
//================================================================//
// TODO: doxygen
class DebugPersistenceProvider {
protected:

    map < string, string >      mData;

    //----------------------------------------------------------------//

public:

    //----------------------------------------------------------------//
                        DebugPersistenceProvider             ();
    virtual             ~DebugPersistenceProvider            ();
};

} // namespace Padamose
#endif
