// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_ABSTRACTHASHVERSIONEDSTOREREF_H
#define PADAMOSE_ABSTRACTHASHVERSIONEDSTOREREF_H

#include <padamose/padamose-common.h>
#include <padamose/Variant.h>

namespace Padamose {

class AbstractVersionedBranch;
class VersionedStoreRef;

//================================================================//
// AbstractHasVersionedStoreRef
//================================================================//
// TODO: doxygen
class AbstractHasVersionedStoreRef {
public:

    typedef shared_ptr < const AbstractVersionedBranch >    ConstBranchPtr;
    typedef shared_ptr < AbstractVersionedBranch >          BranchPtr;

protected:

    friend class VersionedStoreRef;

    //----------------------------------------------------------------//
    virtual VersionedStoreRef&          AbstractHasVersionedStoreRef_getRef     () = 0;
    virtual const VersionedStoreRef&    AbstractHasVersionedStoreRef_getRef     () const = 0;

public:

    //----------------------------------------------------------------//
                                    AbstractHasVersionedStoreRef            ();
    virtual                         ~AbstractHasVersionedStoreRef           ();
    VersionedStoreRef&              getRef                                  ();
    const VersionedStoreRef&        getRef                                  () const;
//    ConstBranchPtr                  getSourceBranch                         () const;
    BranchPtr                       getSourceBranch                         () const;
    size_t                          getVersion                              () const;
};

} // namespace Padamose
#endif
