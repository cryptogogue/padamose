// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <padamose/AbstractPersistenceProvider.h>
#include <padamose/AbstractPersistentVersionedBranch.h>
#include <padamose/EphemeralVersionedBranch.h>
#include <padamose/VersionedStoreTag.h>

namespace Padamose {

//================================================================//
// EphemeralVersionedBranch
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
void EphemeralVersionedBranch::copyValues ( AbstractVersionedBranch& other ) {

    // copy the value stacks
    map < string, shared_ptr < EphemeralValueStack >>::iterator valueStackIt = this->mValueStacksByKey.begin ();
    for ( ; valueStackIt != this->mValueStacksByKey.end (); ++valueStackIt ) {
        
        string key = valueStackIt->first;
        shared_ptr < const EphemeralValueStack > fromStack = this->findValueStack ( valueStackIt->first );
        assert ( fromStack );
        fromStack->join ( key, other );
    }
}

//----------------------------------------------------------------//
EphemeralVersionedBranch::EphemeralVersionedBranch () {
}

//----------------------------------------------------------------//
EphemeralVersionedBranch::~EphemeralVersionedBranch () {
}

//----------------------------------------------------------------//
/** \brief Returns the ValueStack corresponding to the given key, if it
    exists in the branch. Does not recurse: searches only the local branch.

    \param      key         The key.
    \return                 The ValueStack for the key or NULL.
*/
shared_ptr < const EphemeralValueStack > EphemeralVersionedBranch::findValueStack ( string key ) const {

    map < string, shared_ptr < EphemeralValueStack >>::const_iterator valueIt = this->mValueStacksByKey.find ( key );
    return ( valueIt != this->mValueStacksByKey.cend ()) ? valueIt->second : NULL;
}

//================================================================//
// overrides
//================================================================//

//----------------------------------------------------------------//
/** \brief Initializes a new branch using the given branch as a parent. If the base
    version is less than the top of the parent branch, then the corresponding layer is copied.
 
    A parent branch must be provided. If the base version given is below the parent's immutable
    top (i.e. other clients or children depend on later versions) then all values set in that
    version must be copied into the child branch.
 
    The given version must be greater than or equal to the base version of the parent branch. (We
    could iterate back through the list of branches if an earlier version is given, but for the
    use case of this constructor, that functionality isn't needed.)
 
    There is a special case when a child branch is created with a base version of zero. Since
    there are no earlier versions, the parent branch's version may still be copied from, but no
    parent is set in the child. The child becomes a new root and thus has no parents.
 
    \param      parent          Parent branch.
    \param      baseVersion     Base version of the child branch.
*/
shared_ptr < AbstractVersionedBranch > EphemeralVersionedBranch::AbstractVersionedBranch_fork ( size_t baseVersion ) {
    
    shared_ptr < EphemeralVersionedBranch > child = make_shared < EphemeralVersionedBranch >();
    
    assert ( this->mVersion <= baseVersion );

    child->setParent ( this->mVersion < baseVersion ? this->shared_from_this () : this->mSourceBranch, baseVersion );

    map < size_t, Layer >::const_iterator layerIt = this->mLayers.find ( baseVersion );
    if ( layerIt != this->mLayers.cend ()) {
    
        const Layer& fromLayer = layerIt->second;
        Layer& toLayer = child->mLayers [ baseVersion ];
        
        Layer::const_iterator keyIt = fromLayer.cbegin ();
        for ( ; keyIt != fromLayer.cend (); ++keyIt ) {
            
            toLayer.insert ( *keyIt );
            
            shared_ptr < const EphemeralValueStack > fromStack = this->findValueStack ( *keyIt );
            assert ( fromStack );
            
            shared_ptr < EphemeralValueStack >& toStack = child->mValueStacksByKey [ *keyIt ];
            if ( !toStack ) {
                toStack = fromStack->makeEmptyCopy ();
            }
            toStack->copyValueFrom ( *fromStack, baseVersion );
        }
    }
    return child;
}

//----------------------------------------------------------------//
/** \brief Returns the version above the top layer held in the branch.

    \return     The version above the top layer or branch's base version if the branch is empty.
*/
size_t EphemeralVersionedBranch::AbstractVersionedBranch_getTopVersion () const {

    // If there are any layers, use the top layer's version. Otherwise, 0.
    return this->mLayers.size () ? this->mLayers.rbegin ()->first + 1 : this->mVersion;
}

//----------------------------------------------------------------//
// TODO: doxygen
size_t EphemeralVersionedBranch::AbstractVersionedBranch_getValueNextVersion ( string key, size_t version ) const {

    shared_ptr < const EphemeralValueStack > valueStack = this->findValueStack ( key );
    if ( valueStack ) {
        return valueStack->getNextVersion ( version );
    }
    return version;
}

//----------------------------------------------------------------//
// TODO: doxygen
size_t EphemeralVersionedBranch::AbstractVersionedBranch_getValuePrevVersion ( string key, size_t version ) const {

    shared_ptr < const EphemeralValueStack > valueStack = this->findValueStack ( key );
    if ( valueStack ) {
        return valueStack->getPrevVersion ( version );
    }
    return version;
}

//----------------------------------------------------------------//
// TODO: doxygen
Variant EphemeralVersionedBranch::AbstractVersionedBranch_getValueVariant ( size_t version, string key ) const {

    if ( this->mVersion <= version ) {
    
        // check for a value stack without recursion.
        shared_ptr < const EphemeralValueStack > valueStack = this->findValueStack ( key );
        if ( valueStack ) {
            return valueStack->getValueVariant ( version );
        }
    }
    return Variant ();
}

//----------------------------------------------------------------//
// TODO: doxygen
bool EphemeralVersionedBranch::AbstractVersionedBranch_getValueVersionExtents ( string key, size_t upperBound, size_t& first, size_t& last ) const {

    shared_ptr < const EphemeralValueStack > valueStack = this->findValueStack ( key );
    if ( valueStack ) {
        return valueStack->getExtents ( upperBound, first, last );
    }
    return false;
}

//----------------------------------------------------------------//
// TODO: doxygen
bool EphemeralVersionedBranch::AbstractVersionedBranch_hasKey ( string key, size_t upperBound ) const {

    if ( this->mVersion <= upperBound ) {
        shared_ptr < const EphemeralValueStack > valueStack = this->findValueStack ( key );
        if ( valueStack ) {
            return valueStack->hasKey ( upperBound );
        }
    }
    return false;
}

//----------------------------------------------------------------//
// TODO: doxygen
bool EphemeralVersionedBranch::AbstractVersionedBranch_isPersistent () const {

    return false;
}

//----------------------------------------------------------------//
/** \brief Implementation of virtual method. Appends the contents of
    the branch onto the given branch and transfers all clients and
    children to the given branch.
 
    The branch is optimized before being appended. Optimization may
    recursively trigger additional joins.
 
    Neither branch is permitted to have direct references.
 
    \param      other       The branch to be appended to.
*/
void EphemeralVersionedBranch::AbstractVersionedBranch_joinBranch ( AbstractVersionedBranch& other ) {

    assert ( other.getDirectReferenceCount () == 0 );
    assert ( this->mLockCount == 0 );

    LGN_LOG_SCOPE ( PDM_FILTER_ROOT, INFO, "EphemeralVersionedBranch::AbstractVersionedBranchOrLeaf_joinBranch ()" );
    LGN_LOG ( PDM_FILTER_ROOT, INFO, "JOINING PARENT BRANCH" );
    
    this->optimize ();
    this->copyValues ( other );
    this->transferClients ( other );
}

//----------------------------------------------------------------//
// TODO: doxygen
void EphemeralVersionedBranch::AbstractVersionedBranch_persist ( shared_ptr < AbstractPersistentVersionedBranch > persist ) {

    this->copyValues ( *persist );
    this->transferClients ( *persist );
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
void EphemeralVersionedBranch::AbstractVersionedBranch_setValueVariant ( size_t version, string key, const Variant& value ) {

    assert ( this->mVersion <= version );
    
    shared_ptr < EphemeralValueStack >& valueStack = this->mValueStacksByKey [ key ];

    if ( !valueStack ) {
        valueStack = make_shared < EphemeralValueStack >( value.index ());
    }
    assert ( valueStack );

    valueStack->setValueVariant ( version, value );

    Layer& layer = this->mLayers [ version ];
    if ( layer.find ( key ) == layer.end ()) {
        layer.insert ( key );
    }
}

//----------------------------------------------------------------//
/** \brief Discards all layers and values with versions greater than or equal to the
    given version.
 
    \param      topVersion  Version for new "top" of branch.
*/
void EphemeralVersionedBranch::AbstractVersionedBranch_truncate ( size_t topVersion ) {

    LGN_LOG_SCOPE ( PDM_FILTER_ROOT, INFO, "truncate: %d -> %d", ( int )this->getTopVersion (), ( int )topVersion );

    map < size_t, Layer >::reverse_iterator layerIt = this->mLayers.rbegin ();
    while (( layerIt != this->mLayers.rend ()) && ( layerIt->first >= topVersion )) {
    
        LGN_LOG_SCOPE ( PDM_FILTER_ROOT, INFO, "popping layer: %d", ( int )layerIt->first );
        
        Layer& layer = layerIt->second;
        
        Layer::iterator keyIt = layer.begin ();
        for ( ; keyIt != layer.end (); ++keyIt ) {

            shared_ptr < EphemeralValueStack >& valueStack = this->mValueStacksByKey [ *keyIt ];
            assert ( valueStack );
            
            valueStack->erase ( layerIt->first );
            if ( valueStack->size () == 0 ) {
                this->mValueStacksByKey.erase ( *keyIt );
            }
        }
        this->mLayers.erase ( layerIt->first );
        layerIt = this->mLayers.rbegin ();
    }
}

} // namespace Padamose
