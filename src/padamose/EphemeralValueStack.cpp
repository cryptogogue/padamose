// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <padamose/AbstractVersionedBranch.h>
#include <padamose/EphemeralValueStack.h>

namespace Padamose {

//================================================================//
// EphemeralValueStack
//================================================================//

//----------------------------------------------------------------//
/** \brief Copies a single value from the source stack to the destination stack.

    \param      from        The value stack to copy from.
    \param      version     The version of the value to copy.
*/
void EphemeralValueStack::copyValueFrom ( const EphemeralValueStack& from, size_t version ) {
    
    Variant value = from.getValueVariant ( version );
    if ( !value.isNull ()) {
        this->setValueVariant ( version, value );
    }
}

//----------------------------------------------------------------//
EphemeralValueStack::EphemeralValueStack ( size_t typeIndex ) :
    mTypeIndex ( typeIndex ) {
}

//----------------------------------------------------------------//
EphemeralValueStack::~EphemeralValueStack () {
}

//----------------------------------------------------------------//
/** \brief Erases the value (if any) corresponding to the given version.

    \param      version     The version of the value to erase.
*/
void EphemeralValueStack::erase ( size_t version ) {
    this->mValuesByVersion.erase ( version );
}

//----------------------------------------------------------------//
// TODO: doxygen
bool EphemeralValueStack::getExtents ( size_t upperBound, size_t& first, size_t& last ) const {

    if ( this->mValuesByVersion.size ()) {
    
        first = this->mValuesByVersion.begin ()->first;
    
        if ( first > upperBound ) return false;
    
        typename map < size_t, Variant >::const_iterator valueIt = this->mValuesByVersion.lower_bound ( upperBound );
    
        if ( valueIt == this->mValuesByVersion.cend ()) {
            last = this->mValuesByVersion.rbegin ()->first;
        }
        else {
            if ( valueIt->first > upperBound ) {
                valueIt--;
            }
            last = valueIt->first;
        }
        return true;
    }
    return false;
}

//----------------------------------------------------------------//
// TODO: doxygen
size_t EphemeralValueStack::getNextVersion ( size_t version ) const {

    typename map < size_t, Variant >::const_iterator valueIt = this->mValuesByVersion.find ( version );
    assert ( valueIt != this->mValuesByVersion.cend ());
    
    valueIt++;
    if ( valueIt != this->mValuesByVersion.cend ()) {
        version = valueIt->first;
    }
    return version;
}

//----------------------------------------------------------------//
// TODO: doxygen
size_t EphemeralValueStack::getPrevVersion ( size_t version ) const {

    typename map < size_t, Variant >::const_iterator valueIt = this->mValuesByVersion.find ( version );
    assert ( valueIt != this->mValuesByVersion.cend ());
    
    if ( valueIt != this->mValuesByVersion.cbegin ()) {
        valueIt--;
        version = valueIt->first;
    }
    return version;
}

//----------------------------------------------------------------//
/** \brief  Returns a pointer to the value for a key at a given version
            or NULL if the value cannot be found.
 
            Returns the value for the most recent version equal to or less than
            the given version.
 
    \param  key         The key.
    \param  version     The version.
    \return             A pointer to the value or NULL.
*/
Variant EphemeralValueStack::getValueVariant ( size_t version ) const {
    
    if ( this->mValuesByVersion.size ()) {
    
        typename map < size_t, Variant >::const_iterator valueIt = this->mValuesByVersion.lower_bound ( version );
        
        if ( valueIt == this->mValuesByVersion.cend ()) {
            return this->mValuesByVersion.rbegin ()->second;
        }
        else {
        
            if ( valueIt->first > version ) {
                if ( valueIt == this->mValuesByVersion.cbegin ()) return Variant ();
                valueIt--;
            }
            return valueIt->second;
        }
    }
    return Variant ();
}

//----------------------------------------------------------------//
// TODO: doxygen
bool EphemeralValueStack::hasKey ( size_t upperBound ) const {

    if ( this->mValuesByVersion.size ()) {
        typename map < size_t, Variant >::const_iterator valueIt = this->mValuesByVersion.cbegin ();
        return ( valueIt->first <= upperBound );
    }
    return false;
}

//----------------------------------------------------------------//
// TODO: doxygen
void EphemeralValueStack::join ( string key, AbstractVersionedBranch& branch ) const {

    map < size_t, Variant >::const_iterator valueIt = this->mValuesByVersion.cbegin ();
    for ( ; valueIt != this->mValuesByVersion.cend (); ++valueIt ) {
        branch.setValueVariant ( valueIt->first, key, valueIt->second );
    }
}

//----------------------------------------------------------------//
/** \brief Creates an empty copy of the source stack.

    \return     A new EphemeralValueStack with the same type implementation as the source.
*/
unique_ptr < EphemeralValueStack > EphemeralValueStack::makeEmptyCopy () const {
    return make_unique < EphemeralValueStack >( this->mTypeIndex );
}

//----------------------------------------------------------------//
/** \brief  Sets or overwrites the value for the corresponding key at the
            current version.
 
    \param  key         The key.
    \param  value       The value.
*/
void EphemeralValueStack::setValueVariant ( size_t version, const Variant& value ) {

    if ( value.index () != this->mTypeIndex ) throw TypeMismatchOnAssignException ();
    this->mValuesByVersion [ version ] = value;
}

//----------------------------------------------------------------//
/** \brief Returns the size (i.e. total values) held in the stack.

    \return     The size of the stack.
*/
size_t EphemeralValueStack::size () const {

    return this->mValuesByVersion.size ();
}

} // namespace Padamose
