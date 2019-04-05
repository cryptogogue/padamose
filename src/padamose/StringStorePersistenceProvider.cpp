// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <padamose/StringStorePersistenceProvider.h>
#include <padamose/StringStoreVersionedBranch.h>

namespace Padamose {

//================================================================//
// StringStorePersistenceProvider
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
void StringStorePersistenceProvider::eraseBranchID ( string branchID ) {

    this->mBranchIDs.erase ( branchID );
}

//----------------------------------------------------------------//
// TODO: doxygen
string StringStorePersistenceProvider::makeBranchID () {

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
StringStorePersistenceProvider::StringStorePersistenceProvider () {

    assert ( false );
}

//----------------------------------------------------------------//
StringStorePersistenceProvider::StringStorePersistenceProvider ( shared_ptr < AbstractStringStore > store ) {

    assert ( store );
    this->mStore = store;
}

//----------------------------------------------------------------//
StringStorePersistenceProvider::~StringStorePersistenceProvider () {
}

//================================================================//
// overrides
//================================================================//

//----------------------------------------------------------------//
shared_ptr < AbstractPersistentVersionedBranch > StringStorePersistenceProvider::AbstractPersistenceProvider_makePersistentBranch () {

    shared_ptr < StringStoreVersionedBranch > branch = make_shared < StringStoreVersionedBranch >();
    branch->mStringStore = this->mStore;
    branch->mBranchID = this->makeBranchID ();
    return branch;
}

} // namespace Padamose
