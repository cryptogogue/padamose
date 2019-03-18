// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_EPHEMERALVERSIONEDBRANCH_H
#define PADAMOSE_EPHEMERALVERSIONEDBRANCH_H

#include <padamose/padamose-common.h>
#include <padamose/AbstractVersionedBranch.h>
#include <padamose/AbstractVersionedBranchClient.h>
#include <padamose/ValueStack.h>

namespace Padamose {

class AbstractVersionedBranchClient;
class VersionedStoreSnapshot;

//================================================================//
// EphemeralVersionedBranch
//================================================================//
/** \brief EphemeralVersionedBranch is an internal data structure used to store
    a contiguous block of versioned values.
 
    Each branch is a sparse record of changes to the database over a
    span of versions. It contains a map of ValueStack instances referenced
    by key. In addition, a sparse stack of version layers (implemented as a map of
    string sets) is used to quickly identify the keys of values modified in that layer.
 
    The branch tracks a base version. Its "top" version is the highest version
    index in the layer stack. When a layer is removed, the keys of the values that were
    set in that layer are retrieved from the layer stack and used to erase the
    corresponding values from each value stack.
 
    Any branch may have a parent branch. When searching for values, the tree will
    be searched recurively until a value is found.
*/
class EphemeralVersionedBranch :
    public enable_shared_from_this < EphemeralVersionedBranch >,
    public AbstractVersionedBranch {
private:

    friend class AbstractVersionedBranchClient;
    friend class AbstractVersionedValueIterator;
    friend class VersionedStore;
    friend class VersionedStoreSnapshot;
    friend class VersionedStoreIterator;
    template < typename > friend class VersionedValue;
    template < typename > friend class VersionedValueIterator;

    typedef set < string > Layer;
    
    /// Sparse array mapping versions onto layers. Each layer holds the set of keys corresponding to values that were set or modified in the version.
    map < size_t, Layer >                                   mLayers;
    
    /// Map of value stacks, indexed by key.
    map < string, unique_ptr < AbstractValueStack >>        mValueStacksByKey;
    
    //----------------------------------------------------------------//
    /** \brief Recursively searches the branch to find the value for the key. The most recent version
        equal to or earlier will be returned.

        A pointer to the value or NULL is returned.

        \param      version     Search for this version or the most recent lesser version of the value.
        \param      key         The key.
        \return                 A raw pointer to the value for the key or NULL.
    */
    template < typename TYPE >
    const TYPE* getValueOrNil ( size_t version, string key ) const {

        // start searching at the current branch.
        const AbstractVersionedBranch* branch = this;
        
        // iterate through parent branches.
        for ( ; branch; branch = branch->mSourceBranch.get ()) {
        
            // ignore branches above the version we're searching for.
            if ( branch->mVersion <= version ) {
            
                // TODO: obviously, this is temp
                const EphemeralVersionedBranch* ephemeralBranch = dynamic_cast < const EphemeralVersionedBranch* >( branch );
                assert ( ephemeralBranch );
                
                // check for a value stack without recursion.
                const AbstractValueStack* abstractValueStack = ephemeralBranch->findValueStack ( key );
                
                if ( abstractValueStack ) {
                
                    // we can optimize this later if we need to.
                    const ValueStack < TYPE >* valueStack = dynamic_cast < const ValueStack < TYPE >* >( abstractValueStack );
                    if ( !valueStack ) {
                        return NULL; // value exists, but is wrong type.
                    }
                    
                    // check if value stack contains an ID for a matching or most recent lesser version.
                    // it's possible that all values are more recent than the version, in which case
                    // NULL will be returned and we'll keep searching.
                    const TYPE* value = valueStack->getValueOrNil ( version );
                    if ( value ) {
                        return value;
                    }
                }
            }
            
            // cap the version at the base version before moving to the parent branch.
            // necessary because branches don't always emerge from the top.
            version = branch->mVersion;
        }
        return NULL;
    }

    //----------------------------------------------------------------//
    /** \brief Sets a value at the given version. If the version doesn't exist,
        a new layer will be created. Also creates a value stack if none exists. Throws
        a TypeMismatchOnAssignException if a value of a different type has already been
        assigned to the key.
     
        \param      version     The version to set the value at. Must be equal to or greater than the branch's base version.
        \param      key         Key of the value to set.
        \param      value       Raw pointer to value to set.
     
        \throws     TypeMismatchOnAssignException
    */
    template < typename TYPE >
    void setValue ( size_t version, string key, const TYPE& value ) {
        
        assert ( this->mVersion <= version );
        
        unique_ptr < AbstractValueStack >& abstractValueStack = this->mValueStacksByKey [ key ];
        
        if ( !abstractValueStack ) {
            abstractValueStack = make_unique < ValueStack < TYPE >>();
        }
        assert ( abstractValueStack );
        
        // we can optimize this later if we need to.
        ValueStack < TYPE >* valueStack = dynamic_cast < ValueStack < TYPE >* >( abstractValueStack.get ());
        if ( !valueStack ) throw TypeMismatchOnAssignException ();

        valueStack->setValue ( version, value );
        
        Layer& layer = this->mLayers [ version ];
        if ( layer.find ( key ) == layer.end ()) {
            layer.insert ( key );
        }
    }

    //----------------------------------------------------------------//
    const AbstractValueStack*       findValueStack              ( string key ) const;
    void                            truncate                    ( size_t topVersion );

    //----------------------------------------------------------------//
    shared_ptr < AbstractVersionedBranch >  AbstractVersionedBranch_fork            ( size_t baseVersion ) override;
    size_t                  AbstractVersionedBranch_getTopVersion                   () const override;
    bool                    AbstractVersionedBranch_hasKey                          ( size_t version, string key ) const override;
    void                    AbstractVersionedBranch_optimize                        () override;
    bool                    AbstractVersionedBranchClient_canJoin                   () const override;
    size_t                  AbstractVersionedBranchClient_getJoinScore              () const override;
    size_t                  AbstractVersionedBranchClient_getVersionDependency      () const override;
    void                    AbstractVersionedBranchClient_joinBranch                ( AbstractVersionedBranch& branch ) override;
    bool                    AbstractVersionedBranchClient_preventJoin               () const override;

public:

    //----------------------------------------------------------------//
                    EphemeralVersionedBranch        ();
                    ~EphemeralVersionedBranch       ();
};

} // namespace Padamose
#endif
