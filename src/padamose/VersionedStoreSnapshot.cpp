// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <padamose/VersionedStoreSnapshot.h>
#include <padamose/AbstractPersistenceProvider.h>

namespace Padamose {

//================================================================//
// VersionedStoreSnapshot
//================================================================//

//----------------------------------------------------------------//
/** \brief Create a branch if none exists and add self as client.
*/
void VersionedStoreSnapshot::affirmBranch () {

    if ( !this->mSourceBranch ) {
        assert ( this->mVersion == 0 );
        this->setBranch ( make_shared < EphemeralVersionedBranch >(), this->mVersion );
    }
}

//----------------------------------------------------------------//
/** \brief Abandons branch and sets version to 0.
*/
void VersionedStoreSnapshot::clear () {

    this->setBranch ( NULL, 0 );
}

//----------------------------------------------------------------//
/** \brief Recursively searches the branch to find the value for the key. The most recent version
    equal to or earlier will be returned.

    A pointer to the value or NULL is returned.

    \param      key         The key.
    \return                 A pointer to the value for the key or NULL.
*/
Variant VersionedStoreSnapshot::getValueVariant ( string key ) const {
    return this->getValueVariant ( key, this->mVersion );
}

//----------------------------------------------------------------//
/** \brief Recursively searches the branch to find the value for the key. The most recent version
    equal to or earlier will be returned.

    A pointer to the value or NULL is returned.

    \param      version     Search for this version of the most recent lesser version of the value;
    \param      key         The key.
    \return                 A pointer to the value for the key or NULL.
*/
Variant VersionedStoreSnapshot::getValueVariant ( string key, size_t version ) const {
    return this->mSourceBranch ? this->mSourceBranch->getValueVariant ( version <= this->mVersion ? version : this->mVersion, key ) : Variant ();
}

//----------------------------------------------------------------//
/** \brief Return the current version.

    \return             The current version.
*/
size_t VersionedStoreSnapshot::getVersion () const {

    return this->mVersion;
}

//----------------------------------------------------------------//
/** \brief Recursively searches the branch to see if a the given key exists.

    \param      key         The key.
    \return                 TRUE if the key is found. FALSE if not.
*/
bool VersionedStoreSnapshot::hasKey ( string key ) const {

    return this->mSourceBranch ? this->mSourceBranch->hasKey ( this->mVersion, key ) : false;
}

//----------------------------------------------------------------//
/** \brief  Check to see if the value can be found.

    \param  key     The key.
    \return         True if the value exists. False if it doesn't.
*/
bool VersionedStoreSnapshot::hasValue ( string key ) const {
    return this->hasValue ( key, this->mVersion );
}

//----------------------------------------------------------------//
/** \brief  Check to see if the value can be found for the given version.

            Check the value for the most recent version equal to or less than
            the given version.

    \param  key     The key.
    \return         True if the value exists. False if it doesn't.
*/
bool VersionedStoreSnapshot::hasValue ( string key, size_t version ) const {

    return ( !this->getValueVariant ( key, version ).isNull ());
}

//----------------------------------------------------------------//
// TODO: doxygen
void VersionedStoreSnapshot::persist ( AbstractPersistenceProvider& provider, string branchName ) {

    assert ( this->mSourceBranch );
    this->persistSource ( provider );
    provider.tagBranch ( *this->mSourceBranch, branchName, this->mVersion );
}

//----------------------------------------------------------------//
/** \brief Set the debug name. Implemented in _DEBUG builds only. Otehrwise a no-op.
 
    \param  debugName   The debug name.
*/
void VersionedStoreSnapshot::setDebugName ( string debugName ) {

    #ifdef _DEBUG
        this->mDebugName = debugName;
    #endif
}

//----------------------------------------------------------------//
/** \brief Copy a snapshot.

    This is a relatively low-cost operation. Taking a snapshot will
    add a dependency on the shared branch but won'y do anything else
    until the branch is altered.
 
    \param  other   The snapshot to copy.
*/
void VersionedStoreSnapshot::takeSnapshot ( const VersionedStoreSnapshot& other ) {

    this->setBranch ( other.mSourceBranch, other.mVersion );
}

//----------------------------------------------------------------//
// TODO: doxygen
void VersionedStoreSnapshot::takeSnapshot ( AbstractPersistenceProvider& provider, string branchName ) {

    BranchTag& tag = provider.getTag ( branchName );
    this->setBranch ( tag.mBranch, tag.mVersion );
}

//----------------------------------------------------------------//
VersionedStoreSnapshot::VersionedStoreSnapshot () {
}

//----------------------------------------------------------------//
/** \brief Copy a snapshot.
 
    \param  other   The snapshot to copy.
*/
VersionedStoreSnapshot::VersionedStoreSnapshot ( const VersionedStoreSnapshot& other ) {

    this->takeSnapshot ( other );
}

//----------------------------------------------------------------//
// TODO: doxygen
VersionedStoreSnapshot::VersionedStoreSnapshot ( AbstractPersistenceProvider& provider, string branchName ) {

    this->takeSnapshot ( provider, branchName );
}

//----------------------------------------------------------------//
VersionedStoreSnapshot::~VersionedStoreSnapshot () {
}

//================================================================//
// overrides
//================================================================//

//----------------------------------------------------------------//
/** \brief Implementation of virtual method. Always returns false.
    \return     Always returns false.
*/
bool VersionedStoreSnapshot::AbstractVersionedBranchClient_canJoin () const {
    return false;
}

//----------------------------------------------------------------//
/** \brief Implementation of virtual method. Asserts false in debug builds;
    returns zero in non-debug builds.
*/
size_t VersionedStoreSnapshot::AbstractVersionedBranchClient_getJoinScore () const {
    assert ( false );
    return 0;
}

//----------------------------------------------------------------//
/** \brief Return the version depended on in the branch by this snapshot.
    This version must not be altered by any other snapshot.

    All values are held in a branch. The "dependent version" is the
    version below which all layers must be unique to the snapshot. If any other
    snapshot needs to alter any layer below the depdent layer, a new branch
    must be created to hold the changes. The new branch will contain a
    full copy of the layer being changed.
 
    For snapshots, since they reference the "current version" where
    values will be written, the "dependent version" is the mVersion + 1.
 
    \return             The dependent version.
*/
size_t VersionedStoreSnapshot::AbstractVersionedBranchClient_getVersionDependency () const {
    return this->mVersion + 1;
}

//----------------------------------------------------------------//
/** \brief Implementation of virtual method. Asserts false in debug builds;
    does nothing in non-debug builds.
*/
void VersionedStoreSnapshot::AbstractVersionedBranchClient_joinBranch ( AbstractVersionedBranch& branch ) {
    assert ( false );
}

//----------------------------------------------------------------//
/** \brief Implementation of virtual method. Always returns false.
    \return     Always returns false.
*/
bool VersionedStoreSnapshot::AbstractVersionedBranchClient_preventJoin () const {
    return false;
}

} // namespace Padamose
