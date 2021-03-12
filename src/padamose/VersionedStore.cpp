// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <padamose/VersionedStore.h>

namespace Padamose {

//================================================================//
// VersionedStore
//================================================================//

//----------------------------------------------------------------//
/** \brief Creates a new branch if there are any dependencies above
    the current version.

    \todo Internally implemented as a pop followed by a push. Look into
    a more efficient implementation.
*/
void VersionedStore::clearVersion () {

    LGN_LOG_SCOPE ( PDM_FILTER_ROOT, INFO, "VersionedStore::clearVersion ()" );

    size_t version = this->mVersion;
    this->popVersion ();
    if ( version > 0 ) {
        this->pushVersion ();
    }
}

//----------------------------------------------------------------//
/** \brief Pop the top version and revert to the version before it.
*/
void VersionedStore::popVersion () {

    LGN_LOG_SCOPE ( PDM_FILTER_ROOT, INFO, "VersionedStore::  ()" );

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
void VersionedStore::prepareForSetValue () {

    LGN_LOG_SCOPE ( PDM_FILTER_ROOT, INFO, "VersionedStore::prepareForSetValue ()" );

    this->affirmBranch ();
    
    size_t dependencies = this->mSourceBranch->countDependencies ();
    if ( dependencies > 1 ) {
    
        size_t immutableTop = this->mSourceBranch->findImmutableTop ( this );
        LGN_LOG ( PDM_FILTER_ROOT, INFO, "immutableTop: %d", ( int )immutableTop );
        
        if ( this->mVersion < immutableTop ) {
        
            LGN_LOG ( PDM_FILTER_ROOT, INFO, "SPLIT!" );
        
            this->mSourceBranch->eraseClient ( *this );
            this->mSourceBranch = this->mSourceBranch->fork ( this->mVersion );
            this->mSourceBranch->insertClient ( *this );
        }
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
void VersionedStore::pushVersion () {

    LGN_LOG_SCOPE ( PDM_FILTER_ROOT, INFO, "VersionedStore::pushVersion ()" );

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
void VersionedStore::revert ( size_t version ) {

    LGN_LOG_SCOPE ( PDM_FILTER_ROOT, INFO, "VersionedStore::rewind ( %d )", ( int )version );

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
void VersionedStore::revertAndClear ( size_t version ) {

    this->revert ( version );
    this->clearVersion ();
}

//----------------------------------------------------------------//
VersionedStore::VersionedStore () {
}

//----------------------------------------------------------------//
VersionedStore::VersionedStore ( VersionedStoreSnapshot& other ) :
    VersionedStoreSnapshot ( other ) {
}

//----------------------------------------------------------------//
// TODO: doxygen
VersionedStore::VersionedStore ( shared_ptr < AbstractPersistenceProvider > provider, string branchName ) {

    this->takeSnapshot ( provider, branchName );
}

//----------------------------------------------------------------//
VersionedStore::~VersionedStore () {
}

} // namespace Padamose
