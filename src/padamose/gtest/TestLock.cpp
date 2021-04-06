// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <padamose/gtest/gtest-helpers.h>
#include <padamose/gtest/util.h>
#include <padamose/padamose.h>

namespace Padamose {
namespace Test {

//----------------------------------------------------------------//
TEST ( Lock, test_lock_unlock ) {
        
    VersionedStoreTag store;
    
    store.setValue < string >( "KEY", "VERSION 0" );
    
    store.pushVersion ();
    store.printTree ();
    store.setValue < string >( "KEY", "VERSION 1" );
    
    store.pushVersion ();
    store.printTree ();
    store.setValue < string >( "KEY", "VERSION 2" );
    
    store.pushVersion ();
    store.printTree ();
    
    VersionedStoreLock lock ( store );
    store.setValue < string >( "KEY", "VERSION 3" );
    
    store.pushVersion ();
    store.printTree ();
    
    store.revert ( 1 );
    store.printTree ();
    
    lock.unlock ();
    store.printTree ();
}

} // namespace Test
} // namespace Padamose
