// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_GTEST_TESTWITHFACTORY_H
#define PADAMOSE_GTEST_TESTWITHFACTORY_H

#include <padamose/gtest/util.h>

namespace Padamose {
namespace Test {

//================================================================//
// AbstractFactory
//================================================================//
template < typename TYPE >
class AbstractFactory{
public:

    //----------------------------------------------------------------//
    virtual shared_ptr < TYPE >     make ()     const = 0;

    //----------------------------------------------------------------//
    virtual ~AbstractFactory () {
    }
};

//================================================================//
// TestWithFactory
//================================================================//
template < typename FACTORY_TYPE, typename PRODUCT_TYPE >
class TestWithFactory :
    public virtual testing::Test {
protected:

    shared_ptr < AbstractFactory < PRODUCT_TYPE > >   mFactory;

public:

    //----------------------------------------------------------------//
    TestWithFactory () :
        mFactory ( make_shared < FACTORY_TYPE >()) {
    }
};

} // namespace Test
} // namespace Padamose
#endif
