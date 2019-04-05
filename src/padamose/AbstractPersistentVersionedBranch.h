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

    weak_ptr < AbstractPersistenceProvider >    mProvider;

    //----------------------------------------------------------------//
    void                setProvider                                 ( shared_ptr < AbstractPersistenceProvider > provider );

public:

    //----------------------------------------------------------------//
                        AbstractPersistentVersionedBranch           ();
                        ~AbstractPersistentVersionedBranch          ();
};

} // namespace Padamose
#endif
