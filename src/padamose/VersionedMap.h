// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_VERSIONEDMAP_H
#define PADAMOSE_VERSIONEDMAP_H

#include <padamose/padamose-common.h>
#include <padamose/MutableVersionedCollection.h>

namespace Padamose {

//================================================================//
// VersionedMap
//================================================================//
/** \brief  VersionedMap is a map collection with iterable keys. Unlike
            VersionedSet, keys must be explicitely provided by the user.
 
            User-provided keys are used to generate node IDs by hashing them.
            This can lead to collisions. Collisions are handled by maintaining
            a table of collisiones and augmenting node IDs with a counter.
 
            Each node ID has a 48-bit hash portion and a 15-bit counter
            portion. If there is no collision, the counter portion is 0 (hash+0).
 
            When a collision occurs, the counter for the prefix is
            incremented. The combination of the prefix and the counter is
            then stored in a "decollider" table, indexed by the original key.
 
            When generating a node ID, the given key is first checked
            against the decollider table. If a decollider exists, the
            decollider is used as the node ID (hash+N), instead of the
            prefix alone (hash+0).
 
            By default, the collisions for the VersionedMap are stored
            in tables prefixed with the collection name, thus scoping
            them "locally" to the container. These prefixes may be
            overridden by the user. For example, in a database with many
            VersionedMaps, all sharing the same pool of keys, it is
            more efficient for them to share a common collision table.
 
            Since all versions of the container (and thus its collisions)
            are permanently remembered by the VersionedStore, there is
            no significant advantage to clean up unused decolliders. The
            decollider table is also slightly more efficient than typical
            methods for handling collisions in terms of storage and code
            complexity.
*/
class VersionedMap :
    public MutableVersionedCollection {
private:
    
    /// Postfix for collision look up. Lookup key is: <collection name>COLLISION_POSTFIX<string encoded hash portion>
    static constexpr const char* COLLISION_POSTFIX          = ".collision.";
    
    /// Postfix for decollider look up. Lookup key is: <collection name>DECOLLIDER_POSTFIX<key>
    static constexpr const char* DECOLLIDER_POSTFIX         = ".decollider.";
    
    /// Bitmask for the hash portion of the node ID. Hash portion = (( hash ( key ) & HASH_PORTION_MASK ).
    static constexpr const size_t HASH_PORTION_MASK         = 0x0000ffffffffffff;
        
    /// Bitmask for the counter portion of the node ID. Counter portion = (( collision count ) & COUNTER_PORTION_MASK ).
    static constexpr const size_t COUNTER_PORTION_MASK      = 0x7fff;
    
    /// Shift for the counter portion. Node ID = (( hash portion ) | (( counter portion ) << COUNTER_PORTION_SHIFT ))
    static constexpr const size_t COUNTER_PORTION_SHIFT     = 48;
    
    /// Prefix string of collision counter table. Append string encoded hash portion to look up collision counter.
    string          mCollisionPrefix;
    
    /// Prefix string of decollider table. Append full key string to look up decollider.
    string          mDecolliderPrefix;
    
    //----------------------------------------------------------------//
    size_t          affirmKey                   ( string key );
    size_t          getHashPortion              ( string key ) const;
    
    //----------------------------------------------------------------//
    virtual size_t  VersionedMap_hash           ( string key ) const;
    
public:

    //----------------------------------------------------------------//
    void            deleteKey                   ( string key );
    void            setCollisionPrefixes        ( string collisionPrefix, string decolliderPrefix );
                    VersionedMap                ( VersionedStore& store, string name );
                    ~VersionedMap               ();
    
    //----------------------------------------------------------------//
    /** \brief  Sets or overwrites the value for the corresponding key.
     
        \param  key         The key.
        \param  value       The value.
    */
    template < typename TYPE >
    void setValue ( string key, const TYPE& value ) {
        
        size_t nodeID = this->affirmKey ( key );
        this->MutableVersionedCollection::setValue < TYPE >( nodeID, key, value );
    }
};

} // namespace Padamose
#endif
