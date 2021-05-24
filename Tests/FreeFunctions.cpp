#include <iostream>
#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "Common.h"
#include "Utils.h"

TEST( FreeFunctions, contains )
{
    Nums n{ 1 };
    ASSERT_TRUE( contains( n, 1 ) );
    ASSERT_FALSE( contains( n, 2 ) );

    n = { 2, 3, 4 };
    ASSERT_TRUE( contains( n, 2 ) );
    ASSERT_FALSE( contains( n, 9 ) );
}

TEST( FreeFunctions, printBoard )
{
    std::array<std::array<Num, 9>, 9> values{
    {
        {0,0,0,0,0,0,0,0,0},
        {5,9,0,0,3,4,6,0,0},
        {0,6,0,0,0,0,0,8,0},
        {4,0,0,0,0,8,0,0,9},
        {0,1,0,0,0,0,0,7,6},
        {0,0,0,0,0,0,5,0,0},
        {0,7,0,9,0,0,0,0,3},
        {3,0,0,8,0,0,2,6,0},
        {0,5,0,0,7,0,0,0,0},
    }
    };

    Board b( values );

    std::cout << b << std::endl;
}
