#include <iostream>
#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "Common.h"
#include "Utils.h"

using namespace Sudoku;

TEST( FreeFunctions, contains )
{
    Nums n{ 1 };
    ASSERT_TRUE( contains( n, 1 ) );
    ASSERT_FALSE( contains( n, 2 ) );

    n = { 2, 3, 4 };
    ASSERT_TRUE( contains( n, 2 ) );
    ASSERT_FALSE( contains( n, 9 ) );
}

