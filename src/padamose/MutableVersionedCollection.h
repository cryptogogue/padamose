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
/** \brief  Base class for mutable collections referencing a VersionedStoreTag.
*/
class MutableVersionedCollection :
    public AbstractVersionedCollection {
protected:

    /// The versioned store.
    VersionedStoreTag& mStore;
    
    //----------------------------------------------------------------//
    bool            affirmState             ();
    void            appendNode              ( size_t nodeID, string fullKey, string nodeKey );
    void            prependNode             ( size_t nodeID, string fullKey, string nodeKey );
    size_t          removeNode              ( string key, string encodedNodeID, size_t prevID = INVALID_NODE_INDEX );
    void            setNode                 ( string key, const VersionedCollectionNode& node );
    void            storeState              ();
    
    //----------------------------------------------------------------//
    const VersionedStoreTag&       AbstractVersionedCollection_getSnapshot         () const override;
    
    //----------------------------------------------------------------//
    /** \brief  Set the value associated with a key. Note that inclusion
                in the list is not enforced: this method will not produce
                an error or throw an exception if the given key is not
                active.

        \param      nodeID      The numeric ID of the node associated with the key.
        \param      key         The key to assign the value to.
        \param      value       The value to be assigned.
    */
    template < typename TYPE >
    void setValue ( size_t nodeID, string key, const TYPE& value ) {
    
        this->mStore.setValue < u64 >( this->mLookupPrefix + key, nodeID );
        this->mStore.setValue < TYPE >( this->mValuePrefix + key, value );
    }
    
public:

    //----------------------------------------------------------------//
                    MutableVersionedCollection              ( VersionedStoreTag& store, string name );
                    ~MutableVersionedCollection             ();
};

} // namespace Padamose
#endif
