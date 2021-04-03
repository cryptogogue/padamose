// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_VERSIONEDLIST_H
#define PADAMOSE_VERSIONEDLIST_H

#include <padamose/padamose-common.h>
#include <padamose/MutableVersionedCollection.h>

namespace Padamose {

//================================================================//
// VersionedListFreeStack
//================================================================//
/** \brief  Persistent state object for the stack of free nodes. Just
            tracks the top of the state, and the total nodes (active
            and free). The total node count is used to create new node
            IDs when all exisrting node IDs are in use.
*/
class VersionedListFreeStack {
private:

    friend class VersionedList;
    
    size_t      mTop;               // top of key free stack
    size_t      mTotalNodes;        // total nodes created
};

//================================================================//
// VersionedList
//================================================================//
/** \brief  VersionedList is a collection in which keys are automatically
            provisioned and reused. When an element is added to the set,
            a key is either created or reused. When an element is removed
            from the set, its key is added to a stack of unused keys.
 
            VersionedList uses a simple counter to generate keys.
 
            Internally, VersionedList is implemented as a doubly linked list.
*/
class VersionedList :
    public MutableVersionedCollection {
private:
    
    /// Postfix for free stack. Lookup key is: <collection name>SET_FREE_STACK_POSTFIX<string encoded node ID>
    static constexpr const char* SET_FREE_STACK_POSTFIX     = ".free";
    
    /// Fully resolved key for free node stack.
    string                  mFreeStackKey;
    
    /// State of the free stack. Persisted in the VersionedStoreTag.
    VersionedListFreeStack   mFreeStack;
    
    //----------------------------------------------------------------//
    void            loadFreeStack               ();
    size_t          peekNextKey                 ();
    string          provisionKey                ( size_t nextNodeID, bool append );
    void            storeFreeStack              ();
    
public:

    //----------------------------------------------------------------//
    void            deleteKey                   ( string key );
                    VersionedList                ( VersionedStoreTag& store, string name );
                    ~VersionedList               ();
    
    //----------------------------------------------------------------//
    /** \brief  Add an element at the end of the list. Returns a key that
                may be used to directly address the element later.

        \param      value       The value to be appended.
        \return                 The key assigned to the element.
    */
    template < typename TYPE >
    string pushBack ( const TYPE& value ) {
    
        size_t nodeID = this->peekNextKey ();
        string key = this->provisionKey ( nodeID, true );
        this->MutableVersionedCollection::setValue < TYPE >( nodeID, key, value );
        return key;
    }
    
    //----------------------------------------------------------------//
    /** \brief  Add an element at the front of the list. Returns a key that
                may be used to directly address the element later.

        \param      value       The value to be appended.
        \return                 The key assigned to the element.
    */
    template < typename TYPE >
    string pushFront ( const TYPE& value ) {
    
        size_t nodeID = this->peekNextKey ();
        string key = this->provisionKey ( nodeID, false );
        this->MutableVersionedCollection::setValue < TYPE >( nodeID, key, value );
        return key;
    }
    
    //----------------------------------------------------------------//
    /** \brief  Set the value associated with a key. The validity of the
                key is checked. If it is not active and a member of the
                list, a KeyNotFoundException will be thrown.

        \param      key         The key to assign the value to.
        \param      value       The value to be assigned.
     
        \throws     KeyNotFoundException    The key does not exist in the collection, or exists but is not active.
    */
    template < typename TYPE >
    void setValue ( string key, const TYPE& value ) {
    
        size_t nodeID = this->lookupNodeID ( key );
        if ( nodeID == INVALID_NODE_INDEX ) throw KeyNotFoundException ( key );
        this->MutableVersionedCollection::setValue < TYPE >( nodeID, key, value );
    }
};

} // namespace Padamose
#endif
