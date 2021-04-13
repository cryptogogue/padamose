// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <padamose/gtest/gtest-helpers.h>
#include <padamose/gtest/SQLiteLocalDB.h>
#include <padamose/gtest/TestWithFactory.h>
#include <padamose/gtest/util.h>
#include <padamose/padamose.h>

namespace Padamose {
namespace Test {

//================================================================//
// DebugStringStoreFactory
//================================================================//
class DebugStringStoreFactory :
    public AbstractFactory < AbstractPersistenceProvider > {
private:

    mutable shared_ptr < DebugStringStore > mDebugStringStore;

public:

    //----------------------------------------------------------------//
    shared_ptr < AbstractPersistenceProvider > make () const override {
        if ( !this->mDebugStringStore ) {
            this->mDebugStringStore = make_shared < DebugStringStore >();
        }
        return this->mDebugStringStore;
    }
};

//================================================================//
// SQLitePersistenceProviderFactory
//================================================================//
class SQLitePersistenceProviderFactory :
    public SQLiteLocalDB,
    public AbstractFactory < AbstractPersistenceProvider > {
public:

    //----------------------------------------------------------------//
    shared_ptr < AbstractPersistenceProvider > make () const override {
        shared_ptr < SQLitePersistenceProvider > provider = make_shared < SQLitePersistenceProvider >();
        provider->open ( SQLITE_FILENAME );
        return provider;
    }
};

//================================================================//
// SQLiteStringStoreFactory
//================================================================//
class SQLiteStringStoreFactory :
    public SQLiteLocalDB,
    public AbstractFactory < AbstractPersistenceProvider > {
public:

    //----------------------------------------------------------------//
    shared_ptr < AbstractPersistenceProvider > make () const override {
        shared_ptr < SQLiteStringStore > provider = make_shared < SQLiteStringStore >();
        provider->open ( SQLITE_FILENAME );
        return provider;
    }
};

//================================================================//
// PersistenceTest
//================================================================//
template < typename TYPE >
class PersistenceTest :
    public TestWithFactory < TYPE,AbstractPersistenceProvider >  {
};

using testing::Types;
typedef Types < DebugStringStoreFactory, SQLiteStringStoreFactory, SQLitePersistenceProviderFactory > Implementations;
//typedef Types < SQLitePersistenceProviderFactory > Implementations;
TYPED_TEST_SUITE ( PersistenceTest, Implementations );

//----------------------------------------------------------------//
TYPED_TEST ( PersistenceTest, test_basic_persistence ) {

    {
        shared_ptr < AbstractPersistenceProvider > provider = this->mFactory->make ();

        {
            VersionedStoreTag tag;

            tag.setValue < string >( KEY0, STR0 );
            tag.pushVersion (); // version 1
            ASSERT_EQ ( tag.getValue < string >( KEY0 ), STR0 );

            tag.setValue < string >( KEY0, STR1 );
            tag.pushVersion (); // version 2
            ASSERT_EQ ( tag.getValue < string >( KEY0 ), STR1 );

            provider->persist ( tag, "master" );
            ASSERT_EQ ( tag.getValue < string >( KEY0 ), STR1 );

            tag.setValue < string >( KEY0, STR2 );
            tag.pushVersion (); // version 3
            ASSERT_EQ ( tag.getValue < string >( KEY0 ), STR2 );

            provider->persist ( tag, "master" );
            ASSERT_EQ ( tag.getValue < string >( KEY0 ), STR2 );

            tag.setValue < string >( KEY0, STR3 );
            ASSERT_EQ ( tag.getVersion (), 3 );
            ASSERT_EQ ( tag.getValue < string >( KEY0 ), STR3 );

            provider->persist ( tag, "master" );
        }

        VersionedStoreTag tag = provider->restore ( "master" );

        ASSERT_EQ ( tag.getVersion (), 3 );
        ASSERT_EQ ( tag.getValue < string >( KEY0, 0 ), STR0 );
        ASSERT_EQ ( tag.getValue < string >( KEY0, 1 ), STR1 );
        ASSERT_EQ ( tag.getValue < string >( KEY0, 2 ), STR2 );
        ASSERT_EQ ( tag.getValue < string >( KEY0, 3 ), STR3 );

        // get a lock
        VersionedStoreLock lock ( tag );

        // iterate by version
        VersionedStoreIterator tagIt ( lock );
        ASSERT_EQ ( tagIt.getValue < string >( KEY0 ), STR3 );

        tagIt.prev ();
        ASSERT_EQ ( tagIt.getValue < string >( KEY0 ), STR2 );

        tagIt.prev ();
        ASSERT_EQ ( tagIt.getValue < string >( KEY0 ), STR1 );

        tagIt.prev ();
        ASSERT_EQ ( tagIt.getValue < string >( KEY0 ), STR0 );

        tagIt.next ();
        ASSERT_EQ ( tagIt.getValue < string >( KEY0 ), STR1 );

        tagIt.next ();
        ASSERT_EQ ( tagIt.getValue < string >( KEY0 ), STR2 );

        tagIt.next ();
        ASSERT_EQ ( tagIt.getValue < string >( KEY0 ), STR3 );

        // iterate by value
        VersionedValueIterator < string > valueIt ( lock, KEY0 );
        ASSERT_EQ ( valueIt.value (), STR3 );

        valueIt.prev ();
        ASSERT_EQ ( valueIt.value (), STR2 );

        valueIt.prev ();
        ASSERT_EQ ( valueIt.value (), STR1 );

        valueIt.prev ();
        ASSERT_EQ ( valueIt.value (), STR0 );

        valueIt.next ();
        ASSERT_EQ ( valueIt.value (), STR1 );

        valueIt.next ();
        ASSERT_EQ ( valueIt.value (), STR2 );

        valueIt.next ();
        ASSERT_EQ ( valueIt.value (), STR3 );
    }

    {
        shared_ptr < AbstractPersistenceProvider > provider = this->mFactory->make ();

        VersionedStoreTag tag = provider->restore ( "master" );

        ASSERT_EQ ( tag.getVersion (), 3 );
        ASSERT_EQ ( tag.getValue < string >( KEY0, 0 ), STR0 );
        ASSERT_EQ ( tag.getValue < string >( KEY0, 1 ), STR1 );
        ASSERT_EQ ( tag.getValue < string >( KEY0, 2 ), STR2 );
        ASSERT_EQ ( tag.getValue < string >( KEY0, 3 ), STR3 );

        tag.revert ( 2 );

        ASSERT_EQ ( tag.getVersion (), 2 );
        ASSERT_EQ ( tag.getValue < string >( KEY0 ), STR2 );

        provider->persist ( tag, "master" );
    }

    {
        shared_ptr < AbstractPersistenceProvider > provider = this->mFactory->make ();

        VersionedStoreTag tag = provider->restore ( "master" );

        ASSERT_EQ ( tag.getVersion (), 2 );
        ASSERT_EQ ( tag.getValue < string >( KEY0 ), STR2 );
    }
}

//----------------------------------------------------------------//
TYPED_TEST ( PersistenceTest, test_branch_structure ) {

    shared_ptr < AbstractPersistenceProvider > provider = this->mFactory->make ();
    
    VersionedStoreTag tag;
    
    tag.setValue < string >( KEY0, STR0 );
    tag.pushVersion ();
    
    tag.setValue < string >( KEY0, STR1 );
    tag.pushVersion ();
    
    tag.setValue < string >( KEY0, STR2 );
    tag.pushVersion ();
    
    tag.setValue < string >( KEY0, STR3 );
    tag.pushVersion ();
    
    tag.setValue < string >( KEY0, STR4 );
    tag.pushVersion ();
        
    provider->persist ( tag, "master" );
    
    ASSERT_EQ ( tag.getBranchInspector ( 0 ).isPersistent (), true );
    ASSERT_EQ ( tag.getBranchInspector ( 0 ).countDependencies (), 2 );
    ASSERT_EQ ( tag.getBranchInspector ( 0 ).getTopVersion (), 5 );
    
    tag.revert ( 3 );
    
    ASSERT_EQ ( tag.getValue < string >( KEY0 ), STR3 );
    ASSERT_EQ ( tag.getBranchInspector ( 0 ).isPersistent (), true );
    
    tag.clearVersion ();
    
    ASSERT_EQ ( tag.getValue < string >( KEY0 ), STR2 );
    
    ASSERT_EQ ( tag.getBranchInspector ( 0 ).isPersistent (), false );
    ASSERT_EQ ( tag.getBranchInspector ( 0 ).getBaseVersion (), 2 );
    ASSERT_EQ ( tag.getBranchInspector ( 0 ).getTopVersion (), 3 );
    
    ASSERT_EQ ( tag.getBranchInspector ( 1 ).isPersistent (), true );
    ASSERT_EQ ( tag.getBranchInspector ( 1 ).getBaseVersion (), 0 );
    ASSERT_EQ ( tag.getBranchInspector ( 1 ).getTopVersion (), 5 );
    
    provider->persist ( tag, "master" );
    
    ASSERT_EQ ( tag.getValue < string >( KEY0, 0 ), STR0 );
    ASSERT_EQ ( tag.getValue < string >( KEY0, 1 ), STR1 );
    ASSERT_EQ ( tag.getValue < string >( KEY0, 2 ), STR2 );
    
    string val = tag.getValue < string >( KEY0, 3 );
    ASSERT_EQ ( tag.getValue < string >( KEY0, 3 ), STR2 );
    
    ASSERT_EQ ( tag.getBranchInspector ( 0 ).isPersistent (), true );
    ASSERT_EQ ( tag.getBranchInspector ( 0 ).countDependencies (), 2 );
    ASSERT_EQ ( tag.getBranchInspector ( 0 ).getBaseVersion (), 0 );
    ASSERT_EQ ( tag.getBranchInspector ( 0 ).getTopVersion (), 3 );
    
    tag.setValue < string >( KEY0, STR4 );
    
    ASSERT_EQ ( tag.getValue < string >( KEY0 ), STR4 );
    
    ASSERT_EQ ( tag.getBranchInspector ( 0 ).isPersistent (), false );
    ASSERT_EQ ( tag.getBranchInspector ( 0 ).getBaseVersion (), 3 );
    ASSERT_EQ ( tag.getBranchInspector ( 0 ).getTopVersion (), 4 );
    
    ASSERT_EQ ( tag.getBranchInspector ( 1 ).isPersistent (), true );
    ASSERT_EQ ( tag.getBranchInspector ( 1 ).getBaseVersion (), 0 );
    ASSERT_EQ ( tag.getBranchInspector ( 1 ).getTopVersion (), 3 );
    
    provider->persist ( tag, "master" );
    
    string value = tag.getValue < string >( KEY0 );
    
    ASSERT_EQ ( tag.getValue < string >( KEY0 ), STR4 );
    ASSERT_EQ ( tag.getValue < string >( KEY0, 3 ), STR4 );
}

//----------------------------------------------------------------//
TYPED_TEST ( PersistenceTest, test_locking ) {

    shared_ptr < AbstractPersistenceProvider > provider = this->mFactory->make ();
    
    VersionedStoreTag tag;
    
    tag.setValue < string >( KEY0, STR0 );
    tag.pushVersion ();
    
    tag.setValue < string >( KEY0, STR1 );
    tag.pushVersion ();
    
    tag.setValue < string >( KEY0, STR2 );
    tag.pushVersion ();
    
    tag.setValue < string >( KEY0, STR3 );
    tag.pushVersion ();
    
    tag.setValue < string >( KEY0, STR4 );
    tag.pushVersion ();
        
    provider->persist ( tag, "master" );
    
    VersionedStoreLock lock ( tag );
    
    tag.revert ( 3 );
    
    tag.setValue < string >( KEY0, STR3 );
    tag.pushVersion ();
    
    tag.setValue < string >( KEY0, STR4 );
    tag.pushVersion ();
    
    provider->persist ( tag, "master" );
    
    ASSERT_EQ ( tag.getBranchInspector ( 0 ).isPersistent (), true );
    ASSERT_EQ ( tag.getBranchInspector ( 0 ).getBaseVersion (), 3 );
    ASSERT_EQ ( tag.getBranchInspector ( 0 ).getTopVersion (), 5 );
    
    ASSERT_EQ ( tag.getBranchInspector ( 1 ).isPersistent (), true );
    ASSERT_EQ ( tag.getBranchInspector ( 1 ).getBaseVersion (), 0 );
    ASSERT_EQ ( tag.getBranchInspector ( 1 ).getTopVersion (), 5 );
    
    tag.revert ( 2 );
    
    ASSERT_EQ ( tag.getBranchInspector ( 0 ).isPersistent (), true );
    ASSERT_EQ ( tag.getBranchInspector ( 0 ).getBaseVersion (), 0 );
    ASSERT_EQ ( tag.getBranchInspector ( 0 ).getTopVersion (), 5 );
    
    lock.unlock ();
    provider->persist ( tag, "master" );
    
    ASSERT_EQ ( tag.getBranchInspector ( 0 ).isPersistent (), true );
    ASSERT_EQ ( tag.getBranchInspector ( 0 ).getBaseVersion (), 0 );
    ASSERT_EQ ( tag.getBranchInspector ( 0 ).getTopVersion (), 3 );
}

} // namespace Test
} // namespace Padamose
