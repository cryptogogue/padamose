// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <padamose/AbstractVersionedStoreTag.h>
#include <padamose/EphemeralVersionedBranch.h>
#include <padamose/VersionedStoreTag.h>

namespace Padamose {

//================================================================//
// AbstractVersionedStoreTag
//================================================================//

//----------------------------------------------------------------//
AbstractVersionedStoreTag::AbstractVersionedStoreTag () {
}

//----------------------------------------------------------------//
AbstractVersionedStoreTag::~AbstractVersionedStoreTag () {
}

//----------------------------------------------------------------//
/** \brief Create a branch if none exists and add self as client.
*/
void AbstractVersionedStoreTag::affirmBranch () {

    VersionedStoreTag& tag = this->getTag ();

    if ( !tag.mSourceBranch ) {
        assert ( tag.mVersion == 0 );
        tag.setParent ( make_shared < EphemeralVersionedBranch >(), tag.mVersion );
    }
}

//----------------------------------------------------------------//
/** \brief Creates a new branch if there are any dependencies above
    the current version.

    \todo Internally implemented as a pop followed by a push. Look into
    a more efficient implementation.
*/
void AbstractVersionedStoreTag::clearVersion () {

    LGN_LOG_SCOPE ( PDM_FILTER_ROOT, INFO, "AbstractVersionedStoreTag::clearVersion ()" );

    VersionedStoreTag& tag = this->getTag ();
    size_t version = tag.mVersion;
    this->popVersion ();
    if ( version > 0 ) {
        this->pushVersion ();
    }
}

//----------------------------------------------------------------//
// TODO: doxygen
VersionedStoreTag& AbstractVersionedStoreTag::getTag () {
    return this->AbstractVersionedStoreTag_getTag ();
}

//----------------------------------------------------------------//
/** \brief Pop the top version and revert to the version before it.
*/
void AbstractVersionedStoreTag::popVersion () {

    LGN_LOG_SCOPE ( PDM_FILTER_ROOT, INFO, "AbstractVersionedStoreTag::  ()" );

    VersionedStoreTag& tag = this->getTag ();

    if ( tag.mSourceBranch ) {
    
        size_t version = tag.mVersion;
        shared_ptr < AbstractVersionedBranch > branch = tag.mSourceBranch;
        branch->eraseClient ( tag );
        
        tag.mSourceBranch = NULL;
        tag.mVersion = 0;
        
        if ( version == branch->mVersion ) {
            branch = branch->mSourceBranch;
        }
        
        if ( branch ) {
            branch->insertClient ( tag );
            tag.mSourceBranch = branch;
            tag.mVersion = version - 1;
            tag.mSourceBranch->optimize ();
        }
    }
}

//----------------------------------------------------------------//
/** \brief Make sure a branch exists to hold the value. If a branch already exists,
    create a new branch if setting a value for a version with dependencies.
 
    This function find the top version in the store below which all other versions
    should be considered immutable. The immutable top is simply the highest version
    returned by getVersionDependency() for each cursor and branch other than the
    current cursor.
 
    If the current cursor's version is below the immutable top, then a new branch
    must be created. The new branch will become a child of the original branch
    and the cursor will be moved to this new branch.
*/
void AbstractVersionedStoreTag::prepareForSetValue () {

    LGN_LOG_SCOPE ( PDM_FILTER_ROOT, INFO, "AbstractVersionedStoreTag::prepareForSetValue ()" );

    this->affirmBranch ();

    VersionedStoreTag& tag = this->getTag ();
    bool fork = tag.mSourceBranch->isLocked ();
    
    if ( !fork ) {
        size_t dependencies = tag.mSourceBranch->countDependencies ();
        if ( dependencies > 1 ) {
            size_t immutableTop = tag.mSourceBranch->findImmutableTop ( &tag );
            LGN_LOG ( PDM_FILTER_ROOT, INFO, "immutableTop: %d", ( int )immutableTop );
            
            fork = ( tag.mVersion < immutableTop );
        }
    }
    
    if ( fork ) {
    
        LGN_LOG ( PDM_FILTER_ROOT, INFO, "SPLIT!" );
    
        tag.mSourceBranch->eraseClient ( tag );
        tag.mSourceBranch = tag.mSourceBranch->fork ( tag.mVersion );
        tag.mSourceBranch->insertClient ( tag );
    }
}

//----------------------------------------------------------------//
/** \brief Increment the version.
 
    This function just increments the version counter. Since layers
    are created sparsely (and only on write), nothing gets created by
    this function unless the current version is less than the top
    version held in the branch. In this case, a child branch is created
    to represent the "empty" version and the cursor is moved to that
    branch.
*/
void AbstractVersionedStoreTag::pushVersion () {

    LGN_LOG_SCOPE ( PDM_FILTER_ROOT, INFO, "AbstractVersionedStoreTag::pushVersion ()" );

    VersionedStoreTag& tag = this->getTag ();

    if ( !tag.mSourceBranch ) {
        tag.mVersion = 0;
    }

    this->affirmBranch ();
    assert ( tag.mSourceBranch );

    tag.mVersion++;
    LGN_LOG ( PDM_FILTER_ROOT, INFO, "version: %d", ( int )tag.mVersion );
    
    if ( tag.mVersion < tag.mSourceBranch->getTopVersion ()) {
    
        LGN_LOG ( PDM_FILTER_ROOT, INFO, "SPLIT" );
    
        tag.mSourceBranch->eraseClient ( tag );
        tag.mSourceBranch = tag.mSourceBranch->fork ( tag.mVersion - 1 );
        tag.mSourceBranch->insertClient ( tag );
    }
}

//----------------------------------------------------------------//
/** \brief Revert to a previous version.
 
    This method may trigger branch optimizations and deletions. Once
    reverted, there is no way to seek forward again. Use takeSnapshot()
    before calling revert() if it's important to be able to return
    to the current version.
 
    \param  version                         the version to revert to.
    \throws VersionOutOfBoundsException
*/
void AbstractVersionedStoreTag::revert ( size_t version ) {

    LGN_LOG_SCOPE ( PDM_FILTER_ROOT, INFO, "AbstractVersionedStoreTag::rewind ( %d )", ( int )version );

    VersionedStoreTag& tag = this->getTag ();

    if ( version > tag.mVersion ) throw VersionOutOfBoundsException ();

    if (( tag.mSourceBranch ) && ( version < tag.mVersion )) {
    
        shared_ptr < AbstractVersionedBranch > branch = tag.mSourceBranch;
        branch->eraseClient ( tag );
        tag.mSourceBranch = NULL;
        
        size_t top = tag.mVersion;
        for ( ; branch && ( version < branch->mVersion ); branch = branch->mSourceBranch ) {
            top = branch->mVersion;
        }
        
        assert ( branch );
        assert ( branch->mVersion <= version );
        assert ( version < top );
        
        branch->insertClient ( tag );
        tag.mSourceBranch = branch;
        tag.mVersion = version;
        tag.mSourceBranch->optimize ();
    }
}

//----------------------------------------------------------------//
// TODO: doxygen
void AbstractVersionedStoreTag::revertAndClear ( size_t version ) {

    this->revert ( version );
    this->clearVersion ();
}

//----------------------------------------------------------------//
// TODO: doxygen
void AbstractVersionedStoreTag::setValueVariant ( string key, const Variant& value ) {
    this->prepareForSetValue ();
    VersionedStoreTag& tag = this->getTag ();
    assert ( tag.mSourceBranch );
    tag.mSourceBranch->setValueVariant ( tag.mVersion, key, value );
}

//----------------------------------------------------------------//
/** \brief Copy a snapshot.

    This is a relatively low-cost operation. Taking a snapshot will
    add a dependency on the shared branch but won't do anything else
    until the branch is altered.
 
    \param  other   The snapshot to copy.
*/
void AbstractVersionedStoreTag::takeSnapshot ( const AbstractVersionedStoreTag& other ) {
    const VersionedStoreRef& ref = other.getRef ();
    this->getTag ().setParent ( ref.getSourceBranch (), ref.getVersion ());
}

} // namespace Padamose
