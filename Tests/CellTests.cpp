#include "gtest/gtest.h"

#include "Cell.h"

using namespace Sudoku;

using TestCell = Cell;

TEST( CellTests, CTOR )
{
    TestCell c( 9 );
    ASSERT_EQ( c.possibilities(), ( Nums{1, 2, 3, 4, 5, 6, 7, 8, 9} ) );
}

TEST( CellTests, removePossibilities )
{
    TestCell c( 9 );
    ASSERT_EQ( c.getVal(), 0 );

    c.remove( ( Nums{ 3, 4, 5, 6, 7, 8, 9 } ) );
    ASSERT_EQ( c.getVal(), 0 );

    ASSERT_EQ( c.possibilities(), ( Nums{1, 2} ) );
}

TEST( CellTests, hasVal )
{
    TestCell c( 9 );
    ASSERT_FALSE( c.hasVal() );

    c.remove( ( Nums{ 3, 4, 5, 6, 7, 8, 9 } ) );
    ASSERT_FALSE( c.hasVal() );

    c.setVal( 1 );
    ASSERT_TRUE( c.hasVal() );
}

TEST( CellTests, setVal )
{
    TestCell c( 9 );
    ASSERT_EQ( c.getVal(), 0 );
    ASSERT_EQ( c.possibilities().size(), 9 );

    c.setVal( 1 );
    ASSERT_EQ( c.getVal(), 1 );
    ASSERT_EQ( c.possibilities().size(), 1 );
}

TEST( CellTests, getVal )
{
    TestCell c( 9 );
    ASSERT_EQ( c.getVal(), 0 );

    c.remove( ( Nums{ 3, 4, 5, 6, 7, 8, 9 } ) );
    ASSERT_EQ( c.getVal(), 0 );

    c.setVal( 1 );
    ASSERT_EQ( c.getVal(), 1 );
}

TEST( CellTests, compare )
{
    TestCell c1( 9 );
    TestCell c2( 9 );

    EXPECT_EQ( c1, c2 );

    c1.remove( 1 );
    EXPECT_NE( c1, c2 );

    c2.remove( 1 );
    EXPECT_EQ( c1, c2 );
}
