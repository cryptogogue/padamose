// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_BRANCHINSPECTOR_H
#define PADAMOSE_BRANCHINSPECTOR_H

#include <padamose/padamose-common.h>

namespace Padamose {

class AbstractVersionedBranch;

//================================================================//
// BranchInspector
//================================================================//
// TODO: doxygen
class BranchInspector {
protected:

    mutable weak_ptr < AbstractVersionedBranch > mBranch;

public:
    
    //----------------------------------------------------------------//
    // TODO: doxygen
    operator bool () const {
        return ( this->mBranch.use_count () > 0 );
    }
    
    //----------------------------------------------------------------//
                    BranchInspector         ( shared_ptr < AbstractVersionedBranch > branch ) ;
                    BranchInspector         ( const BranchInspector& branchInspector );
                    ~BranchInspector        ();
    size_t          countDependencies       () const;
    size_t          getBaseVersion          () const;
    size_t          getImmutableTop         () const;
    size_t          getTopVersion           () const;
    bool            hasKey                  ( size_t version, string key ) const;
    bool            isLocked                () const;
    bool            isPersistent            () const;
};

} // namespace Padamose
#endif
