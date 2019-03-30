// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <padamose/AbstractPersistenceProvider.h>
#include <padamose/EphemeralVersionedBranch.h>
#include <padamose/VersionedStoreSnapshot.h>

namespace Padamose {

//================================================================//
// EphemeralVersionedBranch
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
void EphemeralVersionedBranch::copyValues ( AbstractVersionedBranch& other ) {

    // copy the value stacks
    map < string, unique_ptr < EphemeralValueStack >>::iterator valueStackIt = this->mValueStacksByKey.begin ();
    for ( ; valueStackIt != this->mValueStacksByKey.end (); ++valueStackIt ) {
        
        string key = valueStackIt->first;
        const EphemeralValueStack* fromStack = this->findValueStack ( valueStackIt->first );
        assert ( fromStack );
        fromStack->join ( key, other );
    }
}

//----------------------------------------------------------------//
EphemeralVersionedBranch::EphemeralVersionedBranch () {
}

//----------------------------------------------------------------//
/** \brief Asserts that no direct references remain.
*/
EphemeralVersionedBranch::~EphemeralVersionedBranch () {
}

//----------------------------------------------------------------//
/** \brief Returns the ValueStack corresponding to the given key, if it
    exists in the branch. Does not recurse: searches only the local branch.

    \param      key         The key.
    \return                 The ValueStack for the key or NULL.
*/
const EphemeralValueStack* EphemeralVersionedBranch::findValueStack ( string key ) const {

    map < string, unique_ptr < EphemeralValueStack >>::const_iterator valueIt = this->mValueStacksByKey.find ( key );
    return ( valueIt != this->mValueStacksByKey.cend ()) ? valueIt->second.get () : NULL;
}

//----------------------------------------------------------------//
/** \brief Discards all layers and values with versions greater than or equal to the
    given version.
 
    \param      topVersion  Version for new "top" of branch.
*/
void EphemeralVersionedBranch::truncate ( size_t topVersion ) {

    LGN_LOG_SCOPE ( PDM_FILTER_ROOT, INFO, "truncate: %d -> %d", ( int )this->getTopVersion (), ( int )topVersion );

    map < size_t, Layer >::reverse_iterator layerIt = this->mLayers.rbegin ();
    while (( layerIt != this->mLayers.rend ()) && ( layerIt->first >= topVersion )) {
    
        LGN_LOG_SCOPE ( PDM_FILTER_ROOT, INFO, "popping layer: %d", ( int )layerIt->first );
        
        Layer& layer = layerIt->second;
        
        Layer::iterator keyIt = layer.begin ();
        for ( ; keyIt != layer.end (); ++keyIt ) {

            unique_ptr < EphemeralValueStack >& valueStack = this->mValueStacksByKey [ *keyIt ];
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

//================================================================//
// overrides
//================================================================//

//----------------------------------------------------------------//
/** \brief Initializes a new branch using the given branch as a parent. If the base
    version is less than the top of the parent branch, then the corresponding layer is copied.
 
    A parent branch must be provided. If the base version given is below the parent's immutable
    top (i.e. other clients or children depend on later versions) then all values set in that
    version must be copied into the child branch.
 
    The given version must be greated than or equal to the base version of the parent branch. (We
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
    
    assert (( this->mVersion <= baseVersion ) && ( baseVersion <= this->getTopVersion ()));

    child->setBranch ( this->mVersion < baseVersion ? this->shared_from_this () : this->mSourceBranch, baseVersion );

    map < size_t, Layer >::const_iterator layerIt = this->mLayers.find ( baseVersion );
    if ( layerIt != this->mLayers.cend ()) {
    
        const Layer& fromLayer = layerIt->second;
        Layer& toLayer = child->mLayers [ baseVersion ];
        
        Layer::const_iterator keyIt = fromLayer.cbegin ();
        for ( ; keyIt != fromLayer.cend (); ++keyIt ) {
            
            toLayer.insert ( *keyIt );
            
            const EphemeralValueStack* fromStack = this->findValueStack ( *keyIt );
            assert ( fromStack );
            
            unique_ptr < EphemeralValueStack >& toStack = child->mValueStacksByKey [ *keyIt ];
            if ( !toStack ) {
                toStack = fromStack->makeEmptyCopy ();
            }
            toStack->copyValueFrom ( *fromStack, baseVersion );
        }
    }
    return child;
}

//----------------------------------------------------------------//
/** \brief Returns the top of the version stack or 0 if the stack is empty.

    \return     The top of the version stack or 0 if the branch is empty.
*/
size_t EphemeralVersionedBranch::AbstractVersionedBranch_getTopVersion () const {

    // If there are any layers, use the top layer's version. Otherwise, 0.
    return this->mLayers.size () ? this->mLayers.rbegin ()->first + 1 : 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
size_t EphemeralVersionedBranch::AbstractVersionedBranch_getValueNextVersion ( string key, size_t version ) const {

    const EphemeralValueStack* valueStack = this->findValueStack ( key );
    if ( valueStack ) {
        return valueStack->getNextVersion ( version );
    }
    return version;
}

//----------------------------------------------------------------//
// TODO: doxygen
size_t EphemeralVersionedBranch::AbstractVersionedBranch_getValuePrevVersion ( string key, size_t version ) const {

    const EphemeralValueStack* valueStack = this->findValueStack ( key );
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
        const EphemeralValueStack* valueStack = this->findValueStack ( key );
        if ( valueStack ) {
            return valueStack->getValueVariant ( version );
        }
    }
    return Variant ();
}

//----------------------------------------------------------------//
// TODO: doxygen
bool EphemeralVersionedBranch::AbstractVersionedBranch_getValueVersionExtents ( string key, size_t upperBound, size_t& first, size_t& last ) const {

    const EphemeralValueStack* valueStack = this->findValueStack ( key );
    if ( valueStack ) {
        return valueStack->getExtents ( upperBound, first, last );
    }
    return false;
}

//----------------------------------------------------------------//
// TODO: doxygen
bool EphemeralVersionedBranch::AbstractVersionedBranch_hasKey ( string key, size_t upperBound ) const {

    if ( this->mVersion <= upperBound ) {
        const EphemeralValueStack* valueStack = this->findValueStack ( key );
        if ( valueStack ) {
            return valueStack->hasKey ( upperBound );
        }
    }
    return false;
}

//----------------------------------------------------------------//
/** \brief Attempts to optimize the branch.

    "Optimizing" a branch means two things: trim any unused versions from the
    top of the branch, and concatenate the longest child branch if the child
    branches from the top of the current branch.
 
    Unused versions are simply those equal to or greatet than the "immutable top"
    version. The immutable top can fall below the top version following the
    removal of the top client. In this case, the branch can be truncated.
 
    Following truncation, if there are child branches with their bases at the
    top pf the branch, one of those children may be joined to the branch. In
    this case, we select the longest child branch. When the child is joined to
    the parent, all of its clients must also be moved to the parent.
 
    Optimization is performed recursively on any child branch selected to be
    be joined to the parent.
 
    The presence of "direct references" will prevent the join optimization.
    A "direct reference" is a pointer to a branch internal, such as a value.
    As the join operation may invalidate internals, it must be prevented as
    long as direct references exist. This is accomplished using a reference
    count maintained by the VersionedValue object.
*/
void EphemeralVersionedBranch::AbstractVersionedBranch_optimize () {

    LGN_LOG_SCOPE ( PDM_FILTER_ROOT, INFO, "EphemeralVersionedBranch::optimize ()" );
    
    // use one loop to find the immutable top and to identify a child branch that
    // may be joined to the parent branch.
    
    size_t immutableTop = this->mVersion; // immutable top won't be less than the branch's base version.
    bool preventJoin = this->preventJoin (); // don't allow join if there are any direct references to the current branch. (May be over-cautious.)
    AbstractVersionedBranchClient* bestJoin = NULL; // a join will be performed if this is non-NULL.
    
    // loop through every client...
    LGN_LOG ( PDM_FILTER_ROOT, INFO, "evaluating clients for possible concatenation..." );
    set < AbstractVersionedBranchClient* >::const_iterator clientIt = this->mClients.cbegin ();
    for ( ; clientIt != this->mClients.cend (); ++clientIt ) {

        AbstractVersionedBranchClient* client = *clientIt;
        LGN_LOG_SCOPE ( PDM_FILTER_ROOT, INFO, "client %p", client );
        
        size_t clientVersion = client->getVersionDependency (); // store the client's version dependency to avoid extra function calls.
        
        // the immutable top is the *highest* client version dependency. update it if and only if
        // a client with a higher depenency is found.
        if ( immutableTop < clientVersion ) {
        
            LGN_LOG ( PDM_FILTER_ROOT, INFO, "found new immutable top: %d -> %d", ( int )immutableTop, ( int )clientVersion );
        
            immutableTop = clientVersion;
            
            // if we've found a more recent client, invalidate any candidate for joining.
            // we can only optimize by joining a child branch located at the top of
            // the branch.
            bestJoin = NULL;
        }
        
        // if nothing is preventing a join, check to see if we can (and should) select
        // the current client as our new candidate for join. only conder client if dependency
        // is greater or equal to the current immutable top.
        if (( !preventJoin ) && client->canJoin () && ( client->getVersionDependency () >= immutableTop )) {

            // a client branch with direct refereces will prevent any join.
            if ( client->preventJoin ()) {
                preventJoin = true; // stop considering join candidates.
                bestJoin = NULL; // clear out any existing join candidate.
            }
            else {
            
                // the "join score" is just the length of the branch. if we don't yet have a join
                // candidate, pick the current client. if we already have a candidate, pick the
                // one with the higher join score.
                if (( !bestJoin ) || ( bestJoin->getJoinScore () < client->getJoinScore ())) {
                    LGN_LOG ( PDM_FILTER_ROOT, INFO, "found a client that can join!" );
                    LGN_LOG ( PDM_FILTER_ROOT, INFO, "bestJoin dependency: %d", ( int )client->getVersionDependency ());
                    bestJoin = client;
                }
            }
        }
    }
    
    LGN_LOG ( PDM_FILTER_ROOT, INFO, "immutableTop: %d", ( int )immutableTop );
    LGN_LOG ( PDM_FILTER_ROOT, INFO, "topVersion: %d", ( int )this->getTopVersion ());
    
    // throw away any versions equal to or greater than the immutable top.
    this->truncate ( immutableTop );
    
    // if we have a join candidate, perform the join.
    if ( bestJoin ) {
        assert ( bestJoin->getVersionDependency () >= immutableTop );
        bestJoin->joinBranch ( *this );
    }
}

//----------------------------------------------------------------//
// TODO: doxygen
void EphemeralVersionedBranch::AbstractVersionedBranch_persistSelf ( AbstractPersistenceProvider& provider ) {

    shared_ptr < AbstractVersionedBranch > persist = provider.makePersistentBranch ();
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
    
    unique_ptr < EphemeralValueStack >& valueStack = this->mValueStacksByKey [ key ];

    if ( !valueStack ) {
        valueStack = make_unique < EphemeralValueStack >( value.index ());
    }
    assert ( valueStack );

    valueStack->setValueVariant ( version, value );

    Layer& layer = this->mLayers [ version ];
    if ( layer.find ( key ) == layer.end ()) {
        layer.insert ( key );
    }
}

//----------------------------------------------------------------//
/** \brief Implementation of virtual method. Always returns true.
    \return     Always returns true.
*/
bool EphemeralVersionedBranch::AbstractVersionedBranchClient_canJoin () const {
    return true;
}

//----------------------------------------------------------------//
/** \brief Implementation of virtual method. Returns the top version.
    \return     The top version.
*/
size_t EphemeralVersionedBranch::AbstractVersionedBranchClient_getJoinScore () const {
    return this->getTopVersion ();
}

//----------------------------------------------------------------//
/** \brief Implementation of virtual method. Returns the base version;
    the branch depends on all versions less than the base version.
 
    \return     The base version.
*/
size_t EphemeralVersionedBranch::AbstractVersionedBranchClient_getVersionDependency () const {
    return this->mVersion;
}

//----------------------------------------------------------------//
/** \brief Implementation of virtual method. Appends the contents of
    the branch onto the given branch and transfers all clients and
    children to the given branch.
 
    The branch is optimized before being appended. Optimization may
    recursively trigger additional joins.
 
    Neither branch is permitted to have direct references.
 
    \param      branch      The branch to be appended to.
*/
void EphemeralVersionedBranch::AbstractVersionedBranchClient_joinBranch ( AbstractVersionedBranch& other ) {

    assert ( other.getDirectReferenceCount () == 0 );
    assert ( this->mDirectReferenceCount == 0 );

    LGN_LOG_SCOPE ( PDM_FILTER_ROOT, INFO, "EphemeralVersionedBranch::AbstractVersionedBranchClient_joinBranch ()" );
    LGN_LOG ( PDM_FILTER_ROOT, INFO, "JOINING PARENT BRANCH" );
    
    this->optimize ();
    this->copyValues ( other );
    this->transferClients ( other );
}

//----------------------------------------------------------------//
/** \brief Implementation of virtual method. Prevents a join optimization
    from happening if the branch has any direct references.
 
    \return     True if the branch has any direct references. False otherwise.
*/
bool EphemeralVersionedBranch::AbstractVersionedBranchClient_preventJoin () const {
    return ( this->mDirectReferenceCount > 0 );
}

} // namespace Padamose
