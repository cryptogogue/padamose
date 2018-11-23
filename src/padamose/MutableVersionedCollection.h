// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_MUTABLEVERSIONEDCOLLECTION_H
#define PADAMOSE_MUTABLEVERSIONEDCOLLECTION_H

#include <padamose/padamose-common.h>
#include <padamose/AbstractVersionedCollection.h>

namespace Padamose {

//================================================================//
// MutableVersionedCollection
//================================================================//
/** \brief  Base class for mutable collections referencing a VersionedStore.
*/
class MutableVersionedCollection :
    public AbstractVersionedCollection {
protected:

    /// The versioned store.
    VersionedStore& mStore;
    
    
    //----------------------------------------------------------------//
    bool            affirmState             ();
    void            appendNode              ( size_t nodeID, string fullKey, string nodeKey );
    bool            isActiveKey             ( string encodedNodeID );
    void            prependNode             ( size_t nodeID, string fullKey, string nodeKey );
    size_t          removeNode              ( string encodedNodeID, size_t prevID = INVALID_NODE_INDEX );
    
    //----------------------------------------------------------------//
    const VersionedStoreSnapshot&       AbstractVersionedCollection_getSnapshot         () const override;
    
public:

    //----------------------------------------------------------------//
                    MutableVersionedCollection              ( VersionedStore& store, string name );
                    ~MutableVersionedCollection             ();
};

} // namespace Padamose
#endif
