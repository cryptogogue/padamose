// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <padamose/AbstractVersionedBranch.h>
#include <padamose/AbstractHasVersionedBranch.h>
#include <padamose/HasVersionedBranch.h>

namespace Padamose {

//================================================================//
// AbstractHasVersionedBranch
//================================================================//

//----------------------------------------------------------------//
AbstractHasVersionedBranch::AbstractHasVersionedBranch () {
}

//----------------------------------------------------------------//
AbstractHasVersionedBranch::~AbstractHasVersionedBranch () {
}

//----------------------------------------------------------------//
// TODO: doxygen
HasVersionedBranch& AbstractHasVersionedBranch::getRef () {
    return this->AbstractHasVersionedBranch_getRef ();
}

//----------------------------------------------------------------//
// TODO: doxygen
const HasVersionedBranch& AbstractHasVersionedBranch::getRef () const {
    return this->AbstractHasVersionedBranch_getRef ();
}

//----------------------------------------------------------------//
// TODO: doxygen
HasVersionedBranch::BranchPtr AbstractHasVersionedBranch::getSourceBranch () const {
    return this->getRef ().mSourceBranch;
}

//----------------------------------------------------------------//
// TODO: doxygen
size_t AbstractHasVersionedBranch::getVersion () const {
    return this->getRef ().mVersion;
}



//----------------------------------------------------------------//
/** \brief Recursively searches the branch to find the value for the key. The most recent version
    equal to or earlier will be returned.

    A pointer to the value or NULL is returned.

    \param      key         The key.
    \return                 A pointer to the value for the key or NULL.
*/
Variant AbstractHasVersionedBranch::getValueVariant ( string key ) const {
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
Variant AbstractHasVersionedBranch::getValueVariant ( string key, size_t version ) const {
    const HasVersionedBranch& ref = this->getRef ();
    return ref.mSourceBranch ? ref.mSourceBranch->getValueVariant ( version <= ref.mVersion ? version : ref.mVersion, key ) : Variant ();
}

//----------------------------------------------------------------//
/** \brief Recursively searches the branch to see if a the given key exists.

    \param      key         The key.
    \return                 TRUE if the key is found. FALSE if not.
*/
bool AbstractHasVersionedBranch::hasKey ( string key ) const {
    const HasVersionedBranch& ref = this->getRef ();
    return ref.mSourceBranch ? ref.mSourceBranch->hasKey ( ref.mVersion, key ) : false;
}

//----------------------------------------------------------------//
/** \brief  Check to see if the value can be found.

    \param  key     The key.
    \return         True if the value exists. False if it doesn't.
*/
bool AbstractHasVersionedBranch::hasValue ( string key ) const {
    return this->hasValue ( key, this->getRef ().mVersion );
}

//----------------------------------------------------------------//
/** \brief  Check to see if the value can be found for the given version.

            Check the value for the most recent version equal to or less than
            the given version.

    \param  key     The key.
    \return         True if the value exists. False if it doesn't.
*/
bool AbstractHasVersionedBranch::hasValue ( string key, size_t version ) const {
    return ( !this->getValueVariant ( key, version ).isNull ());
}


} // namespace Padamose
