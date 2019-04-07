// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_ABSTRACTPERSISTENTVERSIONEDBRANCH_H
#define PADAMOSE_ABSTRACTPERSISTENTVERSIONEDBRANCH_H

#include <padamose/padamose-common.h>
#include <padamose/AbstractVersionedBranch.h>

namespace Padamose {

class AbstractPersistenceProvider;

//================================================================//
// AbstractPersistentVersionedBranch
//================================================================//
// TODO: doxygen
class AbstractPersistentVersionedBranch :
    public AbstractVersionedBranch {
protected:

    friend class EphemeralVersionedBranch;

    //----------------------------------------------------------------//
    bool                                            isFrozen                                    () const;

    //----------------------------------------------------------------//
    virtual const AbstractPersistenceProvider*      AbstractPersistentVersionedBranch_getProvider       () const = 0;

public:

    //----------------------------------------------------------------//
                                                    AbstractPersistentVersionedBranch           ();
                                                    ~AbstractPersistentVersionedBranch          ();
    const AbstractPersistenceProvider*              getProvider                                 () const;
};

} // namespace Padamose
#endif
