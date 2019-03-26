// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_EPHEMERALVALUESTACK_H
#define PADAMOSE_EPHEMERALVALUESTACK_H

#include <padamose/padamose-common.h>
#include <padamose/Variant.h>

class AbstractVersionedBranch;

namespace Padamose {

//================================================================//
// EphemeralValueStack
//================================================================//
/** \brief Sparse stack of values indexed by version. Intended to
    support fast lookup of a value for the most recent version.
*/
class EphemeralValueStack {
protected:

    friend class AbstractVersionedValueIterator;
    friend class EphemeralVersionedBranch;
    friend class VersionedStoreSnapshot;
    
    /// Sparse stack of values indexed by version.
    map < size_t, Variant >     mValuesByVersion;
    size_t                      mTypeIndex;

    //----------------------------------------------------------------//
    void                                    copyValueFrom           ( const EphemeralValueStack& from, size_t version );
    void                                    erase                   ( size_t version );
    bool                                    getExtents              ( size_t upperBound, size_t& first, size_t& last ) const;
    size_t                                  getNextVersion          ( size_t version ) const;
    size_t                                  getPrevVersion          ( size_t version ) const;
    Variant                                 getValueVariant         ( size_t version ) const;
    bool                                    hasKey                  ( size_t upperBound ) const;
//    void                                    join                    ( EphemeralValueStack& to ) const;
    void                                    join                    ( string key, AbstractVersionedBranch& branch ) const;
    unique_ptr < EphemeralValueStack >      makeEmptyCopy           () const;
    void                                    setValueVariant         ( size_t version, const Variant& value );
    size_t                                  size                    () const;

public:

    //----------------------------------------------------------------//
                        EphemeralValueStack                  ( size_t typeIndex );
    virtual             ~EphemeralValueStack                 ();
};

} // namespace Padamose
#endif
