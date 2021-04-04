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
/** \brief  Implementation of versioned collection as an immutable snapshot.
*/
class VersionedCollectionSnapshot :
    public AbstractVersionedCollection {
protected:

    /// The versioned snapshot.
    VersionedStoreTag      mSnapshot;
    
    //----------------------------------------------------------------//
    const VersionedStoreTag&       AbstractVersionedCollection_getSnapshot         () const override;
    
public:

    //----------------------------------------------------------------//
                    VersionedCollectionSnapshot         ( const AbstractVersionedCollection& collection );
                    VersionedCollectionSnapshot         ( const VersionedStoreTag& snapshot, string name );
                    ~VersionedCollectionSnapshot        ();
};

} // namespace Padamose
#endif
