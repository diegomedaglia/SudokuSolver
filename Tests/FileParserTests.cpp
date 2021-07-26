#include "gtest/gtest.h"

#include "FileParser.h"

using namespace Sudoku;

using TestBoard = Sudoku::Board;

//0 0 0 0 0 0 0 0 0
//5 9 0 0 3 4 6 0 0
//0 6 0 0 0 0 0 8 0
//4 0 0 0 0 8 0 0 9
//0 1 0 0 0 0 0 7 6
//0 0 0 0 0 0 5 0 0
//0 7 0 9 0 0 0 0 3
//3 0 0 8 0 0 2 6 0
//0 5 0 0 7 0 0 0 0
TEST( FileParserTests, good )
{
    TestBoard b( 3 );
    ASSERT_NO_THROW( b = parseFile( 3, "Good.txt" ) );

    EXPECT_EQ( b.at( 1, 0 ), 5 );
    EXPECT_EQ( b.at( 1, 1 ), 9 );
    EXPECT_EQ( b.at( 2, 1 ), 6 );
    EXPECT_EQ( b.at( 4, 7 ), 7 );
    EXPECT_EQ( b.at( 4, 8 ), 6 );
    EXPECT_EQ( b.at( 8, 1 ), 5 );
    EXPECT_EQ( b.at( 8, 4 ), 7 );
}
//0 0 0 0 0 0 0 0 0
//5 9 0 0 3 4 6 0 0
//0 6 0 0 0 0 0 8 0
//4 0 0 0 0 8 0 0 9
//0 1 0 0 0 0 0 7 6
//0 0 0 0 0 0 5 0 0
//0 7 0 9 0 0 0 0 3
//3 0 0 8 0 0 2 6 0
//0 5 0 0 7 0 0
TEST( FileParserTests, incomplete )
{
    ASSERT_ANY_THROW( parseFile( 3, "Incomplete.txt" ) );
}

//0 0 0 0 0 0 0 0 0
//5 9 0 0 3 4 6 0 0
//0 6 0 0 0 0 0 8 0
//4 0 0 0 0 8 0 0 9
//0 1 0 0 0 0 0 7 6
//0 0 0 0 0 0 5 0 0
//0 7 0 9 0 0 0 0 3
//3 0 0 8 0 0 2 6 0
//0 5 0 0 7 0 0 0 a
TEST( FileParserTests, bad )
{
    ASSERT_ANY_THROW( parseFile( 3, "BadInput.txt" ) );
}

TEST( FileParserTests, Large )
{
    Board b( 4 );
    ASSERT_NO_THROW( b = parseFile( 4, "Large16x16.txt" ) );
    EXPECT_EQ( b.at( 15, 15 ), 2 );
    EXPECT_EQ( b.at( 0, 0 ), 14 );
    EXPECT_EQ( b.at( 13, 1 ), 15 );
}
