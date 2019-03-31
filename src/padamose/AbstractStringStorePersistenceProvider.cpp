// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <padamose/AbstractStringStorePersistenceProvider.h>
#include <padamose/StringStoreVersionedBranch.h>

namespace Padamose {

//================================================================//
// AbstractStringStorePersistenceProvider
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
void AbstractStringStorePersistenceProvider::eraseBranchID ( string branchID ) {

    this->mBranchIDs.erase ( branchID );
}

//----------------------------------------------------------------//
// TODO: doxygen
string AbstractStringStorePersistenceProvider::makeBranchID () {

    string branchID;
    do {
        static const size_t BUFF_SIZE = 5;
        char buffer [ BUFF_SIZE ];
        snprintf ( buffer, BUFF_SIZE, "%x", rand ());
        branchID = buffer;
    } while ( this->mBranchIDs.find ( branchID ) != this->mBranchIDs.cend ());
    this->mBranchIDs.insert ( branchID );
    
    return branchID;
}

//----------------------------------------------------------------//
AbstractStringStorePersistenceProvider::AbstractStringStorePersistenceProvider () {
}

//----------------------------------------------------------------//
AbstractStringStorePersistenceProvider::~AbstractStringStorePersistenceProvider () {
}

//================================================================//
// overrides
//================================================================//

//----------------------------------------------------------------//
shared_ptr < AbstractVersionedBranch > AbstractStringStorePersistenceProvider::AbstractPersistenceProvider_makePersistentBranch () {

    shared_ptr < StringStoreVersionedBranch > branch = make_shared < StringStoreVersionedBranch >();
    branch->mStringStore = this;
    branch->mBranchID = this->makeBranchID ();
    return branch;
}

} // namespace Padamose
