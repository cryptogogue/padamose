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
// TODO: doxygen
class MutableVersionedCollection :
    public AbstractVersionedCollection {
protected:

    VersionedStore&             mStore;
    
    
    //----------------------------------------------------------------//
    bool            affirmState                             ();
    void            insertNode                              ( size_t nodeID, string key, string nodeKey );
    size_t          removeNode                              ( string key, size_t prevID = INVALID_NODE_INDEX );
    
    //----------------------------------------------------------------//
    const VersionedStoreSnapshot&       AbstractVersionedCollection_getSnapshot         () const override;
    
public:

    //----------------------------------------------------------------//
                    MutableVersionedCollection              ( VersionedStore& store, string name );
                    ~MutableVersionedCollection             ();
};

} // namespace Padamose
#endif
