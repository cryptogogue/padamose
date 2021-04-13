// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <padamose/AbstractVersionedBranch.h>
#include <padamose/BranchInspector.h>

namespace Padamose {

//================================================================//
// BranchInspector
//================================================================//
    
//----------------------------------------------------------------//
BranchInspector::BranchInspector ( shared_ptr < AbstractVersionedBranch > branch ) :
    mBranch ( branch ) {
}

//----------------------------------------------------------------//
BranchInspector::BranchInspector ( const BranchInspector& branchInspector ) :
    mBranch ( branchInspector.mBranch ) {
}

//----------------------------------------------------------------//
BranchInspector::~BranchInspector () {
}

//----------------------------------------------------------------//
size_t BranchInspector::countDependencies () const {

    shared_ptr < AbstractVersionedBranch > branch = this->mBranch.lock ();
    assert ( branch );
    return branch->countDependencies ();
}

//----------------------------------------------------------------//
size_t BranchInspector::getBaseVersion () const {

    shared_ptr < AbstractVersionedBranch > branch = this->mBranch.lock ();
    assert ( branch );
    return branch->getVersion ();
}

//----------------------------------------------------------------//
size_t BranchInspector::getImmutableTop () const {

    shared_ptr < AbstractVersionedBranch > branch = this->mBranch.lock ();
    assert ( branch );
    return branch->findImmutableTop ();
}

//----------------------------------------------------------------//
size_t BranchInspector::getTopVersion () const {

    shared_ptr < AbstractVersionedBranch > branch = this->mBranch.lock ();
    assert ( branch );
    return branch->getTopVersion ();
}

//----------------------------------------------------------------//
bool BranchInspector::hasKey ( size_t version, string key ) const {

    shared_ptr < AbstractVersionedBranch > branch = this->mBranch.lock ();
    assert ( branch );
    return branch->hasKey ( version, key );
}

//----------------------------------------------------------------//
bool BranchInspector::isLocked () const {

    shared_ptr < AbstractVersionedBranch > branch = this->mBranch.lock ();
    assert ( branch );
    return branch->isLocked ();
}

//----------------------------------------------------------------//
bool BranchInspector::isPersistent () const {

    shared_ptr < AbstractVersionedBranch > branch = this->mBranch.lock ();
    assert ( branch );
    return branch->isPersistent ();
}

} // namespace Padamose
