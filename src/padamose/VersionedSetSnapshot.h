// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_VERSIONEDSETSNAPSHOT_H
#define PADAMOSE_VERSIONEDSETSNAPSHOT_H

#include <padamose/padamose-common.h>
#include <padamose/AbstractVersionedSetSnapshot.h>

namespace Padamose {

//================================================================//
// VersionedSetSnapshot
//================================================================//
// TODO: doxygen
class VersionedSetSnapshot :
    public AbstractVersionedSetSnapshot {
protected:

    VersionedStoreSnapshot      mSnapshot;
    
    //----------------------------------------------------------------//
    const VersionedStoreSnapshot&       AbstractVersionedSetSnapshot_getSnapshot        () const override;
    
public:

    //----------------------------------------------------------------//
                    VersionedSetSnapshot        ( const VersionedStoreSnapshot& snapshot, string mapName );
                    ~VersionedSetSnapshot       ();
};

} // namespace Padamose
#endif
