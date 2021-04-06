// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <padamose/AbstractVersionedBranch.h>
#include <padamose/AbstractVersionedStoreInspector.h>
#include <padamose/VersionedStoreRef.h>

namespace Padamose {

//================================================================//
// AbstractVersionedStoreInspector
//================================================================//

//----------------------------------------------------------------//
AbstractVersionedStoreInspector::AbstractVersionedStoreInspector () {
}

//----------------------------------------------------------------//
AbstractVersionedStoreInspector::~AbstractVersionedStoreInspector () {
}

//----------------------------------------------------------------//
/** \brief Recursively searches the branch to find the value for the key. The most recent version
    equal to or earlier will be returned.

    A pointer to the value or NULL is returned.

    \param      key         The key.
    \return                 A pointer to the value for the key or NULL.
*/
Variant AbstractVersionedStoreInspector::getValueVariant ( string key ) const {
    return this->getValueVariant ( key, this->getRef ().mVersion );
}

//----------------------------------------------------------------//
/** \brief Recursively searches the branch to find the value for the key. The most recent version
    equal to or earlier will be returned.

    A pointer to the value or NULL is returned.

    \param      version     Search for this version of the most recent lesser version of the value;
    \param      key         The key.
    \return                 A pointer to the value for the key or NULL.
*/
Variant AbstractVersionedStoreInspector::getValueVariant ( string key, size_t version ) const {
    const VersionedStoreRef& ref = this->getRef ();
    return ref.mSourceBranch ? ref.mSourceBranch->getValueVariant ( version <= ref.mVersion ? version : ref.mVersion, key ) : Variant ();
}

//----------------------------------------------------------------//
/** \brief Recursively searches the branch to see if a the given key exists.

    \param      key         The key.
    \return                 TRUE if the key is found. FALSE if not.
*/
bool AbstractVersionedStoreInspector::hasKey ( string key ) const {
    const VersionedStoreRef& ref = this->getRef ();
    return ref.mSourceBranch ? ref.mSourceBranch->hasKey ( ref.mVersion, key ) : false;
}

//----------------------------------------------------------------//
/** \brief  Check to see if the value can be found.

    \param  key     The key.
    \return         True if the value exists. False if it doesn't.
*/
bool AbstractVersionedStoreInspector::hasValue ( string key ) const {
    return this->hasValue ( key, this->getRef ().mVersion );
}

//----------------------------------------------------------------//
/** \brief  Check to see if the value can be found for the given version.

            Check the value for the most recent version equal to or less than
            the given version.

    \param  key     The key.
    \return         True if the value exists. False if it doesn't.
*/
bool AbstractVersionedStoreInspector::hasValue ( string key, size_t version ) const {
    return ( !this->getValueVariant ( key, version ).isNull ());
}

} // namespace Padamose
