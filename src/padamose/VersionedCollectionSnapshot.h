// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_VERSIONEDCOLLECTIONSNAPSHOT_H
#define PADAMOSE_VERSIONEDCOLLECTIONSNAPSHOT_H

#include <padamose/padamose-common.h>
#include <padamose/AbstractVersionedCollection.h>

namespace Padamose {

//================================================================//
// VersionedCollectionSnapshot
//================================================================//
// TODO: doxygen
class VersionedCollectionSnapshot :
    public AbstractVersionedCollection {
protected:

    VersionedStoreSnapshot      mSnapshot;
    
    //----------------------------------------------------------------//
    const VersionedStoreSnapshot&       AbstractVersionedCollection_getSnapshot         () const override;
    
public:

    //----------------------------------------------------------------//
                    VersionedCollectionSnapshot         ( const VersionedStoreSnapshot& snapshot, string mapName );
                    ~VersionedCollectionSnapshot        ();
};

} // namespace Padamose
#endif
