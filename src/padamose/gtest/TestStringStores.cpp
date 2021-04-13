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
    public AbstractFactory < AbstractStringStore > {
private:

    mutable shared_ptr < DebugStringStore > mDebugStringStore;

public:

    //----------------------------------------------------------------//
    shared_ptr < AbstractStringStore > make () const override {
        if ( !this->mDebugStringStore ) {
            this->mDebugStringStore = make_shared < DebugStringStore >();
        }
        return this->mDebugStringStore;
    }
};

//================================================================//
// SQLiteStringStoreFactory
//================================================================//
class SQLiteStringStoreFactory :
    public SQLiteLocalDB,
    public AbstractFactory < AbstractStringStore > {
public:

    //----------------------------------------------------------------//
    shared_ptr < AbstractStringStore > make () const override {
        shared_ptr < SQLiteStringStore > provider = make_shared < SQLiteStringStore >();
        provider->open ( SQLITE_FILENAME );
        return provider;
    }
};

//================================================================//
// StringStoreTest
//================================================================//
template < typename TYPE >
class StringStoreTest :
    public TestWithFactory < TYPE, AbstractStringStore >  {
};

using testing::Types;
typedef Types < DebugStringStoreFactory, SQLiteStringStoreFactory > Implementations;
TYPED_TEST_SUITE ( StringStoreTest, Implementations );

//----------------------------------------------------------------//
TYPED_TEST ( StringStoreTest, test_interface ) {

    shared_ptr < AbstractStringStore > stringStore = this->mFactory->make ();

    ASSERT_EQ ( stringStore->hasString ( KEY0 ), false );
    
    stringStore->setString ( KEY0, STR0 );
    
    ASSERT_EQ ( stringStore->hasString ( KEY0 ), true );
    ASSERT_EQ ( stringStore->getString ( KEY0 ), STR0 );
    
    stringStore->setString ( KEY0, STR1 );
    
    ASSERT_EQ ( stringStore->hasString ( KEY0 ), true );
    ASSERT_EQ ( stringStore->getString ( KEY0 ), STR1 );
    
    stringStore->eraseString ( KEY0 );
    
    ASSERT_EQ ( stringStore->hasString ( KEY0 ), false );
    
    stringStore->setString ( KEY0, STR2 );
    
    ASSERT_EQ ( stringStore->hasString ( KEY0 ), true );
    ASSERT_EQ ( stringStore->getString ( KEY0 ), STR2 );
}

} // namespace Test
} // namespace Padamose
