// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <padamose/AbstractStringStorePersistenceProvider.h>
#include <padamose/StringStoreVersionedBranch.h>
#include <padamose/VersionedStoreSnapshot.h>

namespace Padamose {

//================================================================//
// StringStoreVersionedBranch
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
string StringStoreVersionedBranch::formatKeyForLayerIndexByMemberName ( size_t version, string name ) const {

    ostringstream stream;
    stream << this->mBranchID << ".layerForVersion." << version << ".indexByName." << name;
    return stream.str ();
}

//----------------------------------------------------------------//
// TODO: doxygen
string StringStoreVersionedBranch::formatKeyForLayerMemberNameByIndex ( size_t version, size_t index ) const {

    ostringstream stream;
    stream << this->mBranchID << ".layerForVersion." << version << ".nameByIndex." << index;
    return stream.str ();
}

//----------------------------------------------------------------//
// TODO: doxygen
string StringStoreVersionedBranch::formatKeyForLayerSize ( size_t version ) const {

    ostringstream stream;
    stream << this->mBranchID << ".layerForVersion." << version << ".size";
    return stream.str ();
}

//----------------------------------------------------------------//
// TODO: doxygen
string StringStoreVersionedBranch::formatKeyForLayerStackSize () const {

    ostringstream stream;
    stream << this->mBranchID << ".layerStackSize";
    return stream.str ();
}

//----------------------------------------------------------------//
// TODO: doxygen
string StringStoreVersionedBranch::formatKeyForValueByIndex ( string key, size_t index ) const {

    ostringstream stream;
    stream << this->mBranchID << ".valueStackForKey." << key << ".valueByIndex." << index;
    return stream.str ();
}

//----------------------------------------------------------------//
// TODO: doxygen
string StringStoreVersionedBranch::formatKeyForValueIndexByVersion ( string key, size_t version ) const {
    
    ostringstream stream;
    stream << this->mBranchID << ".valueStackForKey." << key << ".indexByVersion." << version;
    return stream.str ();
}

//----------------------------------------------------------------//
// TODO: doxygen
string StringStoreVersionedBranch::formatKeyForValueStackSize ( string key ) const {
    
    ostringstream stream;
    stream << this->mBranchID << ".valueStackForKey." << key << ".size";
    return stream.str ();
}

//----------------------------------------------------------------//
// TODO: doxygen
string StringStoreVersionedBranch::formatKeyForValueStackType ( string key ) const {

    ostringstream stream;
    stream << this->mBranchID << ".valueStackForKey." << key << ".valueType";
    return stream.str ();
}

//----------------------------------------------------------------//
// TODO: doxygen
string StringStoreVersionedBranch::formatKeyForValueVersionByIndex ( string key, size_t index ) const {

    ostringstream stream;
    stream << this->mBranchID << ".valueStackForKey." << key << ".versionByIndex." << index;
    return stream.str ();
}

//----------------------------------------------------------------//
// TODO: doxygen
AbstractStringStore& StringStoreVersionedBranch::getStore () {

    assert ( this->mStringStore ); // TODO: throw exception
    return *this->mStringStore;
}

//----------------------------------------------------------------//
// TODO: doxygen
const AbstractStringStore& StringStoreVersionedBranch::getStoreConst () const {

    assert ( this->mStringStore ); // TODO: throw exception
    return *this->mStringStore;
}

//----------------------------------------------------------------//
// TODO: doxygen
StringStoreVersionedBranch::StringStoreVersionedBranch () :
    mStringStore ( 0 ) {
}

//----------------------------------------------------------------//
// TODO: doxygen
StringStoreVersionedBranch::~StringStoreVersionedBranch () {
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
shared_ptr < AbstractVersionedBranch > StringStoreVersionedBranch::AbstractVersionedBranch_fork ( size_t baseVersion ) {
    
//    shared_ptr < StringStoreVersionedBranch > child = make_shared < StringStoreVersionedBranch >();
//
//    assert (( this->mVersion <= baseVersion ) && ( baseVersion <= this->getTopVersion ()));
//
//    child->setBranch ( this->mVersion < baseVersion ? this->shared_from_this () : this->mSourceBranch, baseVersion );
//
//    map < size_t, Layer >::const_iterator layerIt = this->mLayers.find ( baseVersion );
//    if ( layerIt != this->mLayers.cend ()) {
//
//        const Layer& fromLayer = layerIt->second;
//        Layer& toLayer = child->mLayers [ baseVersion ];
//
//        Layer::const_iterator keyIt = fromLayer.cbegin ();
//        for ( ; keyIt != fromLayer.cend (); ++keyIt ) {
//
//            toLayer.insert ( *keyIt );
//
//            const EphemeralValueStack* fromStack = this->findValueStack ( *keyIt );
//            assert ( fromStack );
//
//            unique_ptr < EphemeralValueStack >& toStack = child->mValueStacksByKey [ *keyIt ];
//            if ( !toStack ) {
//                toStack = fromStack->makeEmptyCopy ();
//            }
//            toStack->copyValueFrom ( *fromStack, baseVersion );
//        }
//    }
//    return child;

    return NULL;
}

//----------------------------------------------------------------//
/** \brief Returns the top of the version stack or 0 if the stack is empty.

    \return     The top of the version stack or 0 if the branch is empty.
*/
size_t StringStoreVersionedBranch::AbstractVersionedBranch_getTopVersion () const {

    const AbstractStringStore& store = this->getStoreConst ();

    // get the top index
    string keyForLayerStackSize = this->formatKeyForLayerStackSize ();
    size_t layerStackSize = store.get < size_t >( keyForLayerStackSize, INVALID_LAYER_INDEX );
    if ( layerStackSize != INVALID_LAYER_INDEX ) {
        return this->mVersion + layerStackSize;
    }
    return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
size_t StringStoreVersionedBranch::AbstractVersionedBranch_getValueNextVersion ( string key, size_t version ) const {

    const AbstractStringStore& store = this->getStoreConst ();
    
    // get the index of the current version
    string keyForValueIndexByVersion = this->formatKeyForValueIndexByVersion ( key, version );
    size_t index = store.get < size_t >( keyForValueIndexByVersion, INVALID_LAYER_INDEX ); // TODO: throw exception
    
    // get the top index
    string keyForValueStackSize = this->formatKeyForValueStackSize ( key );
    size_t top = store.get < size_t >( keyForValueStackSize, INVALID_LAYER_INDEX ); // TODO: throw exception
    
    if ( index < ( top - 1 )) {
        string keyForValueVersionByIndex = this->formatKeyForValueVersionByIndex ( key, index + 1 );
        return store.get < size_t >( keyForValueVersionByIndex, version );
    }
    return version;
}

//----------------------------------------------------------------//
// TODO: doxygen
size_t StringStoreVersionedBranch::AbstractVersionedBranch_getValuePrevVersion ( string key, size_t version ) const {

    const AbstractStringStore& store = this->getStoreConst ();

    // get the index of the current version
    string keyForValueIndexByVersion = this->formatKeyForValueIndexByVersion ( key, version );
    size_t index = store.get < size_t >( keyForValueIndexByVersion, INVALID_LAYER_INDEX ); // TODO: throw exception
    if ( index > 0 ) {
        string keyForValueVersionByIndex = this->formatKeyForValueVersionByIndex ( key, index - 1 );
        return store.get < size_t >( keyForValueVersionByIndex, version );
    }
    return version;
}

//----------------------------------------------------------------//
// TODO: doxygen
Variant StringStoreVersionedBranch::AbstractVersionedBranch_getValueVariant ( size_t version, string key ) const {

    if ( this->mVersion <= version ) {

        const AbstractStringStore& store = this->getStoreConst ();

        string keyForValueStackSize = this->formatKeyForValueStackSize ( key );
        size_t top = store.get < size_t >( keyForValueStackSize, 0 );
        
        if ( top > 0 ) {
        
            // TODO: binary search
            size_t cursor = top;
            do {
                cursor--;
                string keyForValueVersion = formatKeyForValueVersionByIndex ( key, cursor );
                size_t valueVersion = store.get < size_t >( keyForValueVersion, 0 );
                if ( valueVersion <= version ) {
                
                    string keyForValue = this->formatKeyForValueByIndex ( key, cursor );
                    string value = store.get < string >( keyForValue, "" );
                    assert ( value.size () > 0 ); // TODO: throw exception
                    
                    Variant strVariant ( value );
                    
                    string keyForValueStackType = this->formatKeyForValueStackType ( key );
                    size_t type = store.get < size_t >( keyForValueStackType, Variant::NULL_VARIANT );
                    assert ( type != Variant::NULL_VARIANT ); // TODO: throw exception
                    
                    switch ( type ) {
                        case Variant::BOOL_VARIANT:
                            return Variant ( strVariant.get < bool >());
                        case Variant::DOUBLE_VARIANT:
                            return Variant ( strVariant.get < double >());
                        case Variant::INT_VARIANT:
                            return Variant ( strVariant.get < int >());
                        case Variant::SIZE_VARIANT:
                            return Variant ( strVariant.get < size_t >());
                        case Variant::STRING_VARIANT:
                            return strVariant;
                    }
                }
            } while ( cursor > 0 );
        }
    }
    return Variant ();
}

//----------------------------------------------------------------//
// TODO: doxygen
bool StringStoreVersionedBranch::AbstractVersionedBranch_getValueVersionExtents ( string key, size_t upperBound, size_t& first, size_t& last ) const {

    if ( upperBound < this->mVersion ) return false; // can't have any matching version for given bound

    const AbstractStringStore& store = this->getStoreConst ();
    
    string keyForValueStackSize = formatKeyForValueStackSize ( key );
    size_t top = store.get < size_t >( keyForValueStackSize, 0 );
    
    if ( top > 0 ) {
    
        string keyForFirst = this->formatKeyForValueVersionByIndex ( key, 0 );
        first = store.get < size_t >( keyForFirst, 0 );
        
        if ( first <= upperBound ) {
        
            // TODO: binary search
            size_t cursor = top;
            do {
                cursor--;
                string keyForLast = formatKeyForValueVersionByIndex ( key, cursor );
                last = store.get < size_t >( keyForLast, 0 );
                if ( last <= upperBound ) {
                    return true;
                }
            } while ( cursor > 0 );
        }
    }
    return false;
}

//----------------------------------------------------------------//
// TODO: doxygen
bool StringStoreVersionedBranch::AbstractVersionedBranch_hasKey ( string key, size_t upperBound ) const {

    if ( this->mVersion <= upperBound ) {

        const AbstractStringStore& store = this->getStoreConst ();

        string keyForValueStackSize = this->formatKeyForValueStackSize ( key );
        size_t top = store.get < size_t >( keyForValueStackSize, 0 );
        
        if ( top > 0 ) {
            string keyForValueVersion = formatKeyForValueVersionByIndex ( key, 0 );
            size_t valueVersion = store.get < size_t >( keyForValueVersion, 0 );
            
            return ( valueVersion <= upperBound );
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
void StringStoreVersionedBranch::AbstractVersionedBranch_optimize () {

//    LGN_LOG_SCOPE ( PDM_FILTER_ROOT, INFO, "StringStoreVersionedBranch::optimize ()" );
//
//    // use one loop to find the immutable top and to identify a child branch that
//    // may be joined to the parent branch.
//
//    size_t immutableTop = this->mVersion; // immutable top won't be less than the branch's base version.
//    bool preventJoin = this->preventJoin (); // don't allow join if there are any direct references to the current branch. (May be over-cautious.)
//    AbstractVersionedBranchClient* bestJoin = NULL; // a join will be performed if this is non-NULL.
//
//    // loop through every client...
//    LGN_LOG ( PDM_FILTER_ROOT, INFO, "evaluating clients for possible concatenation..." );
//    set < AbstractVersionedBranchClient* >::const_iterator clientIt = this->mClients.cbegin ();
//    for ( ; clientIt != this->mClients.cend (); ++clientIt ) {
//
//        AbstractVersionedBranchClient* client = *clientIt;
//        LGN_LOG_SCOPE ( PDM_FILTER_ROOT, INFO, "client %p", client );
//
//        size_t clientVersion = client->getVersionDependency (); // store the client's version dependency to avoid extra function calls.
//
//        // the immutable top is the *highest* client version dependency. update it if and only if
//        // a client with a higher depenency is found.
//        if ( immutableTop < clientVersion ) {
//
//            LGN_LOG ( PDM_FILTER_ROOT, INFO, "found new immutable top: %d -> %d", ( int )immutableTop, ( int )clientVersion );
//
//            immutableTop = clientVersion;
//
//            // if we've found a more recent client, invalidate any candidate for joining.
//            // we can only optimize by joining a child branch located at the top of
//            // the branch.
//            bestJoin = NULL;
//        }
//
//        // if nothing is preventing a join, check to see if we can (and should) select
//        // the current client as our new candidate for join. only conder client if dependency
//        // is greater or equal to the current immutable top.
//        if (( !preventJoin ) && client->canJoin () && ( client->getVersionDependency () >= immutableTop )) {
//
//            // a client branch with direct refereces will prevent any join.
//            if ( client->preventJoin ()) {
//                preventJoin = true; // stop considering join candidates.
//                bestJoin = NULL; // clear out any existing join candidate.
//            }
//            else {
//
//                // the "join score" is just the length of the branch. if we don't yet have a join
//                // candidate, pick the current client. if we already have a candidate, pick the
//                // one with the higher join score.
//                if (( !bestJoin ) || ( bestJoin->getJoinScore () < client->getJoinScore ())) {
//                    LGN_LOG ( PDM_FILTER_ROOT, INFO, "found a client that can join!" );
//                    LGN_LOG ( PDM_FILTER_ROOT, INFO, "bestJoin dependency: %d", ( int )client->getVersionDependency ());
//                    bestJoin = client;
//                }
//            }
//        }
//    }
//
//    LGN_LOG ( PDM_FILTER_ROOT, INFO, "immutableTop: %d", ( int )immutableTop );
//    LGN_LOG ( PDM_FILTER_ROOT, INFO, "topVersion: %d", ( int )this->getTopVersion ());
//
//    // throw away any versions equal to or greater than the immutable top.
//    this->truncate ( immutableTop );
//
//    // if we have a join candidate, perform the join.
//    if ( bestJoin ) {
//        assert ( bestJoin->getVersionDependency () >= immutableTop );
//        bestJoin->joinBranch ( *this );
//    }
}

//----------------------------------------------------------------//
// TODO: doxygen
void StringStoreVersionedBranch::AbstractVersionedBranch_persistSelf ( AbstractPersistenceProvider& provider ) {
    // do nothing; already persisted
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
void StringStoreVersionedBranch::AbstractVersionedBranch_setValueVariant ( size_t version, string key, const Variant& value ) {

    assert ( this->mVersion <= version );

    AbstractStringStore& store = this->getStore ();

    // first, set the value stack
    
    string keyForValueStackSize = this->formatKeyForValueStackSize ( key );
    size_t top = store.get < size_t >( keyForValueStackSize, 0 );
    store.set < size_t >( keyForValueStackSize, top + 1 );
    
    if ( top == 0 ) {
        string keyForValueStackType = this->formatKeyForValueStackType ( key );
        store.set < size_t >( keyForValueStackType, value.getType ());
    }
    
    string keyForValueByIndex = this->formatKeyForValueByIndex ( key, top );
    store.setString ( keyForValueByIndex, value.get < string >());
    
    string keyForValueVersionByIndex = this->formatKeyForValueVersionByIndex ( key, top );
    store.set < size_t >( keyForValueVersionByIndex, version );
    
    string keyForValueIndexByVersion = this->formatKeyForValueIndexByVersion ( key, version );
    store.set < size_t >( keyForValueIndexByVersion, top );
    
    // now, make sure the value is recorded in the layer
    string keyForLayerIndexByMemberName = this->formatKeyForLayerIndexByMemberName ( version, key );
    size_t layerIndex = store.get < size_t >( keyForLayerIndexByMemberName, INVALID_LAYER_INDEX );
    
    if ( layerIndex == INVALID_LAYER_INDEX ) {
    
        string keyForLayerSize = this->formatKeyForLayerSize ( version );
        layerIndex = store.get < size_t >( keyForLayerSize, 0 );
        
        string keyForLayerMemberNameByIndex = this->formatKeyForLayerMemberNameByIndex ( version, layerIndex );
        store.set < string >( keyForLayerMemberNameByIndex, key );
        
        store.set < size_t >( keyForLayerIndexByMemberName, layerIndex );
        store.set < size_t >( keyForLayerSize, layerIndex + 1 );
        
        string keyForLayerStackSize = this->formatKeyForLayerStackSize ();
        size_t layerStackSize = store.get < size_t >( keyForLayerStackSize, 0 );
        size_t topVersion = this->mVersion + layerStackSize;
        if ( topVersion < version ) {
            store.set < size_t >( keyForLayerStackSize, version - this->mVersion );
        }
    }
}

//----------------------------------------------------------------//
/** \brief Implementation of virtual method. Always returns true.
    \return     Always returns true.
*/
bool StringStoreVersionedBranch::AbstractVersionedBranchClient_canJoin () const {
    return true;
}

//----------------------------------------------------------------//
/** \brief Implementation of virtual method. Returns the top version.
    \return     The top version.
*/
size_t StringStoreVersionedBranch::AbstractVersionedBranchClient_getJoinScore () const {
    return this->getTopVersion ();
}

//----------------------------------------------------------------//
/** \brief Implementation of virtual method. Returns the base version;
    the branch depends on all versions less than the base version.
 
    \return     The base version.
*/
size_t StringStoreVersionedBranch::AbstractVersionedBranchClient_getVersionDependency () const {
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
void StringStoreVersionedBranch::AbstractVersionedBranchClient_joinBranch ( AbstractVersionedBranch& branch ) {

//    assert ( branch.getDirectReferenceCount () == 0 );
//    assert ( this->mDirectReferenceCount == 0 );
//
//    LGN_LOG_SCOPE ( PDM_FILTER_ROOT, INFO, "StringStoreVersionedBranch::AbstractVersionedBranchClient_joinBranch ()" );
//    LGN_LOG ( PDM_FILTER_ROOT, INFO, "JOINING PARENT BRANCH" );
//
//    this->optimize ();
//
//    shared_ptr < AbstractVersionedBranch > pinThis = this->shared_from_this ();
//
//    // copy the value stacks
//    map < string, unique_ptr < EphemeralValueStack >>::iterator valueStackIt = this->mValueStacksByKey.begin ();
//    for ( ; valueStackIt != this->mValueStacksByKey.end (); ++valueStackIt ) {
//
//        string key = valueStackIt->first;
//        const EphemeralValueStack* fromStack = this->findValueStack ( valueStackIt->first );
//        assert ( fromStack );
//        fromStack->join ( key, branch );
//    }
//
//    // copy the clients
//    set < AbstractVersionedBranchClient* >::iterator clientIt = this->mClients.begin ();
//    for ( ; clientIt != this->mClients.end (); ++clientIt ) {
//        AbstractVersionedBranchClient* client = *clientIt;
//        branch.insertClient ( *client );
//        client->mSourceBranch = branch.shared_from_this ();
//    }
//
//    pinThis = NULL;
}

//----------------------------------------------------------------//
/** \brief Implementation of virtual method. Prevents a join optimization
    from happening if the branch has any direct references.
 
    \return     True if the branch has any direct references. False otherwise.
*/
bool StringStoreVersionedBranch::AbstractVersionedBranchClient_preventJoin () const {
    return ( this->mDirectReferenceCount > 0 );
}

} // namespace Padamose
