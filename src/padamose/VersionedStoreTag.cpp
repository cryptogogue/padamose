// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <padamose/EphemeralVersionedBranch.h>
#include <padamose/VersionedStoreTag.h>

namespace Padamose {

//================================================================//
// VersionedStoreTag
//================================================================//

//----------------------------------------------------------------//
/** \brief Create a branch if none exists and add self as client.
*/
void VersionedStoreTag::affirmBranch () {

    if ( !this->mSourceBranch ) {
        assert ( this->mVersion == 0 );
        this->setBranch ( make_shared < EphemeralVersionedBranch >(), this->mVersion );
    }
}

//----------------------------------------------------------------//
/** \brief Creates a new branch if there are any dependencies above
    the current version.

    \todo Internally implemented as a pop followed by a push. Look into
    a more efficient implementation.
*/
void VersionedStoreTag::clearVersion () {

    LGN_LOG_SCOPE ( PDM_FILTER_ROOT, INFO, "VersionedStoreTag::clearVersion ()" );

    size_t version = this->mVersion;
    this->popVersion ();
    if ( version > 0 ) {
        this->pushVersion ();
    }
}

//----------------------------------------------------------------//
/** \brief Pop the top version and revert to the version before it.
*/
void VersionedStoreTag::popVersion () {

    LGN_LOG_SCOPE ( PDM_FILTER_ROOT, INFO, "VersionedStoreTag::  ()" );

    if ( this->mSourceBranch ) {
    
        size_t version = this->mVersion;
        shared_ptr < AbstractVersionedBranch > branch = this->mSourceBranch;
        branch->eraseClient ( *this );
        
        this->mSourceBranch = NULL;
        this->mVersion = 0;
        
        if ( version == branch->mVersion ) {
            branch = branch->mSourceBranch;
        }
        
        if ( branch ) {
            branch->insertClient ( *this );
            this->mSourceBranch = branch;
            this->mVersion = version - 1;
            this->mSourceBranch->optimize ();
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
void VersionedStoreTag::prepareForSetValue () {

    LGN_LOG_SCOPE ( PDM_FILTER_ROOT, INFO, "VersionedStoreTag::prepareForSetValue ()" );

    this->affirmBranch ();
    
    bool fork = this->mSourceBranch->isLocked ();
    
    if ( !fork ) {
        size_t dependencies = this->mSourceBranch->countDependencies ();
        if ( dependencies > 1 ) {
            size_t immutableTop = this->mSourceBranch->findImmutableTop ( this );
            LGN_LOG ( PDM_FILTER_ROOT, INFO, "immutableTop: %d", ( int )immutableTop );
            
            fork = ( this->mVersion < immutableTop );
        }
    }
    
    if ( fork ) {
    
        LGN_LOG ( PDM_FILTER_ROOT, INFO, "SPLIT!" );
    
        this->mSourceBranch->eraseClient ( *this );
        this->mSourceBranch = this->mSourceBranch->fork ( this->mVersion );
        this->mSourceBranch->insertClient ( *this );
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
void VersionedStoreTag::pushVersion () {

    LGN_LOG_SCOPE ( PDM_FILTER_ROOT, INFO, "VersionedStoreTag::pushVersion ()" );

    if ( !this->mSourceBranch ) {
        this->mVersion = 0;
    }

    this->affirmBranch ();
    assert ( this->mSourceBranch );

    this->mVersion++;
    LGN_LOG ( PDM_FILTER_ROOT, INFO, "version: %d", ( int )this->mVersion );
    
    if ( this->mVersion < this->mSourceBranch->getTopVersion ()) {
    
        LGN_LOG ( PDM_FILTER_ROOT, INFO, "SPLIT" );
    
        this->mSourceBranch->eraseClient ( *this );
        this->mSourceBranch = this->mSourceBranch->fork ( this->mVersion - 1 );
        this->mSourceBranch->insertClient ( *this );
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
void VersionedStoreTag::revert ( size_t version ) {

    LGN_LOG_SCOPE ( PDM_FILTER_ROOT, INFO, "VersionedStoreTag::rewind ( %d )", ( int )version );

    if ( version > this->mVersion ) throw VersionOutOfBoundsException ();

    if (( this->mSourceBranch ) && ( version < this->mVersion )) {
    
        shared_ptr < AbstractVersionedBranch > branch = this->mSourceBranch;
        branch->eraseClient ( *this );
        this->mSourceBranch = NULL;
        
        size_t top = this->mVersion;
        for ( ; branch && ( version < branch->mVersion ); branch = branch->mSourceBranch ) {
            top = branch->mVersion;
        }
        
        assert ( branch );
        assert ( branch->mVersion <= version );
        assert ( version < top );
        
        branch->insertClient ( *this );
        this->mSourceBranch = branch;
        this->mVersion = version;
        this->mSourceBranch->optimize ();
    }
}

//----------------------------------------------------------------//
// TODO: doxygen
void VersionedStoreTag::revertAndClear ( size_t version ) {

    this->revert ( version );
    this->clearVersion ();
}

//----------------------------------------------------------------//
/** \brief Copy a snapshot.

    This is a relatively low-cost operation. Taking a snapshot will
    add a dependency on the shared branch but won't do anything else
    until the branch is altered.
 
    \param  other   The snapshot to copy.
*/
void VersionedStoreTag::takeSnapshot ( const VersionedStoreTag& other ) {
    
    this->setBranch ( other );
}

//----------------------------------------------------------------//
VersionedStoreTag::VersionedStoreTag () {
}

//----------------------------------------------------------------//
VersionedStoreTag::VersionedStoreTag ( const VersionedStoreTag& other ) :
    VersionedStoreRef ( other ) {
}

//----------------------------------------------------------------//
VersionedStoreTag::~VersionedStoreTag () {
}

} // namespace Padamose
