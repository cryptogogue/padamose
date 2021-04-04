// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <padamose/VersionedStoreInspector.h>
#include <padamose/AbstractVersionedBranch.h>

namespace Padamose {

//================================================================//
// VersionedStoreInspector
//================================================================//

//----------------------------------------------------------------//
/** \brief Recursively searches the branch to find the value for the key. The most recent version
    equal to or earlier will be returned.

    A pointer to the value or NULL is returned.

    \param      key         The key.
    \return                 A pointer to the value for the key or NULL.
*/
Variant VersionedStoreInspector::getValueVariant ( string key ) const {
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
Variant VersionedStoreInspector::getValueVariant ( string key, size_t version ) const {
    return this->mSourceBranch ? this->mSourceBranch->getValueVariant ( version <= this->mVersion ? version : this->mVersion, key ) : Variant ();
}

//----------------------------------------------------------------//
/** \brief Recursively searches the branch to see if a the given key exists.

    \param      key         The key.
    \return                 TRUE if the key is found. FALSE if not.
*/
bool VersionedStoreInspector::hasKey ( string key ) const {
    return this->mSourceBranch ? this->mSourceBranch->hasKey ( this->mVersion, key ) : false;
}

//----------------------------------------------------------------//
/** \brief  Check to see if the value can be found.

    \param  key     The key.
    \return         True if the value exists. False if it doesn't.
*/
bool VersionedStoreInspector::hasValue ( string key ) const {
    return this->hasValue ( key, this->mVersion );
}

//----------------------------------------------------------------//
/** \brief  Check to see if the value can be found for the given version.

            Check the value for the most recent version equal to or less than
            the given version.

    \param  key     The key.
    \return         True if the value exists. False if it doesn't.
*/
bool VersionedStoreInspector::hasValue ( string key, size_t version ) const {
    return ( !this->getValueVariant ( key, version ).isNull ());
}

//----------------------------------------------------------------//
VersionedStoreInspector::VersionedStoreInspector () {
}

//----------------------------------------------------------------//
VersionedStoreInspector::~VersionedStoreInspector () {
}

} // namespace Padamose
