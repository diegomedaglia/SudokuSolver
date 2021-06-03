#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "Board.h"
#include "Utils.h"

TEST( BoardTests, CTOR )
{
    Board b;

    for( int i = 0; i < 9; ++i )
    {
        for( int j = 0; j < 9; ++j )
        {
            auto cell = b.cell( i, j );
            ASSERT_EQ( cell.possibilities(), ( Nums{ 1,2,3,4,5,6,7,8,9 } ) );
        }
    }
}

TEST( BoardTests, updateInRow )
{
    Board b;

    b.set( 0, 0, 1 );
    auto cell = b.cell( 0, 0 );
    ASSERT_TRUE( cell.hasVal() );
    ASSERT_EQ( cell.getVal(), 1 );


    ASSERT_FALSE( contains( b.cell( 0, 1 ).possibilities(), 1 ) );
    ASSERT_FALSE( contains( b.cell( 0, 2 ).possibilities(), 1 ) );
    ASSERT_FALSE( contains( b.cell( 0, 3 ).possibilities(), 1 ) );
    ASSERT_FALSE( contains( b.cell( 0, 4 ).possibilities(), 1 ) );
    ASSERT_FALSE( contains( b.cell( 0, 5 ).possibilities(), 1 ) );
    ASSERT_FALSE( contains( b.cell( 0, 6 ).possibilities(), 1 ) );
    ASSERT_FALSE( contains( b.cell( 0, 7 ).possibilities(), 1 ) );
    ASSERT_FALSE( contains( b.cell( 0, 8 ).possibilities(), 1 ) );
}

TEST( BoardTests, updateInCol )
{
    Board b;

    b.set( 0, 0, 1 );
    auto cell = b.cell( 0, 0 );
    ASSERT_TRUE( cell.hasVal() );
    ASSERT_EQ( cell.getVal(), 1 );

    EXPECT_FALSE( contains( b.cell( 1, 0 ).possibilities(), 1 ) );
    EXPECT_FALSE( contains( b.cell( 2, 0 ).possibilities(), 1 ) );
    EXPECT_FALSE( contains( b.cell( 3, 0 ).possibilities(), 1 ) );
    EXPECT_FALSE( contains( b.cell( 4, 0 ).possibilities(), 1 ) );
    EXPECT_FALSE( contains( b.cell( 5, 0 ).possibilities(), 1 ) );
    EXPECT_FALSE( contains( b.cell( 6, 0 ).possibilities(), 1 ) );
    EXPECT_FALSE( contains( b.cell( 7, 0 ).possibilities(), 1 ) );
    EXPECT_FALSE( contains( b.cell( 8, 0 ).possibilities(), 1 ) );
}

TEST( BoardTests, updateInQuadrant )
{
    Board b;

    for( int row = 0; row < 7; row += 3 )
    {
        for( int col = 0; col < 7; col += 3 )
        {
            b.set( row, col, 1 );
            auto cell = b.cell( row, col );
            ASSERT_TRUE( cell.hasVal() ) << row << " " << col;
            ASSERT_EQ( cell.getVal(), 1 ) << row << " " << col;

            EXPECT_FALSE( contains( b.cell( row, col + 1 ).possibilities(), 1 ) ) << row << " " << col;
            EXPECT_FALSE( contains( b.cell( row, col + 2 ).possibilities(), 1 ) ) << row << " " << col;
            EXPECT_FALSE( contains( b.cell( row + 1, col ).possibilities(), 1 ) ) << row << " " << col;
            EXPECT_FALSE( contains( b.cell( row + 1, col + 1 ).possibilities(), 1 ) ) << row << " " << col;
            EXPECT_FALSE( contains( b.cell( row + 1, col + 2 ).possibilities(), 1 ) ) << row << " " << col;
            EXPECT_FALSE( contains( b.cell( row + 2, col ).possibilities(), 1 ) ) << row << " " << col;
            EXPECT_FALSE( contains( b.cell( row + 2, col + 1 ).possibilities(), 1 ) ) << row << " " << col;
            EXPECT_FALSE( contains( b.cell( row + 2, col + 2 ).possibilities(), 1 ) ) << row << " " << col;
        }
    }
}

TEST( BoardTests, CTOR_2 )
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

    for( int i = 0; i < 9; ++i )
    {
        for( int j = 0; j < 9; ++j )
        {
            auto cell = b.cell( i, j );
            if( values[i][j] != 0 )
            {
                EXPECT_TRUE( cell.hasVal() );
                EXPECT_EQ( cell.getVal(), values[i][j] );
            }
        }
    }
}

TEST( BoardTests, CTOR_3 )
{
    std::array<std::array<Num, 9>, 9> values{
        {
            {1,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0},
        }
    };

    Board b( values );

        for( int j = 1; j < 9; ++j )
        {
            auto cell = b.cell( 0, j );
            EXPECT_FALSE( cell.hasVal() );
            EXPECT_EQ( cell.possibilities(), ( Nums{2,3,4,5,6,7,8,9} ) );

            cell = b.cell( j, 0 );
            EXPECT_FALSE( cell.hasVal() );
            EXPECT_EQ( cell.possibilities(), ( Nums{ 2,3,4,5,6,7,8,9 } ) );
        }
}


TEST( BoardTests, CTOR_4 )
{
    std::array<std::array<Num, 9>, 9> values{
        {
            {1,0,0,0,0,0,0,0,9},
            {0,4,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0},
            {2,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0},
        }
    };

    Board b( values );

    EXPECT_EQ( b.cell( 0, 1 ).possibilities(), ( Nums{ 2,3,5,6,7,8 } ) );
    EXPECT_EQ( b.cell( 0, 2 ).possibilities(), ( Nums{ 2,3,5,6,7,8 } ) );
    EXPECT_EQ( b.cell( 1, 0 ).possibilities(), ( Nums{ 3,5,6,7,8,9 } ) );
    EXPECT_EQ( b.cell( 1, 2 ).possibilities(), ( Nums{ 2,3,5,6,7,8,9 } ) );
    EXPECT_EQ( b.cell( 2, 0 ).possibilities(), ( Nums{ 3,5,6,7,8,9 } ) );
    EXPECT_EQ( b.cell( 2, 1 ).possibilities(), ( Nums{ 2,3,5,6,7,8,9 } ) );
    EXPECT_EQ( b.cell( 2, 2 ).possibilities(), ( Nums{ 2,3,5,6,7,8,9 } ) );

}

TEST( BoardTests, updateLoop1 )
{
    std::array<std::array<Num, 9>, 9> values{
        {
            {1,2,3,4,5,6,7,0,0},
            {0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,9,0},
            {0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0},
        }
    };

    Board b( values );

    EXPECT_EQ( b.at( 0, 7 ), 8 );
    EXPECT_EQ( b.at( 0, 8 ), 9 );
}

TEST( BoardTests, updateLoop2 )
{
    std::array<std::array<Num, 9>, 9> values{
        {
            {1,2,3,4,5,6,7,0,0},
            {0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,9,0},
            {0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0},
        }
    };

    Board b( values );

    EXPECT_EQ( b.at( 0, 7 ), 8 );
    EXPECT_EQ( b.at( 0, 8 ), 9 );
}

TEST( BoardTests, sortedPossibilities )
{
    std::array<std::array<Num, 9>, 9> values{
        {
            {1,2,3,4,5,6,0,0,0},
            {0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,7,0,0},
            {0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0},
        }
    };

    Board b( values );

    auto sorted = b.sortedPossibilities();

    EXPECT_EQ( std::get<ccROW>( sorted[0] ), 0 );
    EXPECT_EQ( std::get<ccCOL>( sorted[0] ), 6 );
    EXPECT_EQ( std::get<ccCELL>( sorted[0] ).possibilities().size(), 2 );
                        
    EXPECT_EQ( std::get<ccCELL>( sorted[1] ).possibilities().size(), 3 );
    EXPECT_EQ( std::get<ccCELL>( sorted[2] ).possibilities().size(), 3 );
}

TEST( BoardTests, invalidBoardRow )
{
    std::array<std::array<Num, 9>, 9> values{
        {
            {1,2,3,4,5,6,1,0,0},
            {0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,7,0,0},
            {0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0},
        }
    };
    
    std::shared_ptr<Board> board;

    ASSERT_THROW( board.reset( new Board( values ) ), std::invalid_argument ) << board;
}

TEST( BoardTests, invalidBoardCol )
{
    std::array<std::array<Num, 9>, 9> values{
        {
            {1,2,3,4,5,6,7,0,0},
            {0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0},
            {1,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0},
        }
    };
    
    std::shared_ptr<Board> board;

    ASSERT_THROW( board.reset( new Board( values ) ), std::invalid_argument ) << board;
}

TEST( BoardTests, invalidBoardQuadrant )
{
    std::array<std::array<Num, 9>, 9> values{
        {
            {1,2,3,4,5,6,7,0,0},
            {0,1,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0},
        }
    };

    std::shared_ptr<Board> board;

    ASSERT_THROW( board.reset( new Board( values ) ), std::invalid_argument ) << board;
}

TEST( BoardTests, solved )
{
    std::array<std::array<Num, 9>, 9> values{
        {
            {1,2,3,4,5,6,7,8,9},
            {4,5,6,7,8,9,1,2,3},
            {7,8,9,1,2,3,4,5,6},
            {2,3,4,5,6,7,8,9,1},
            {5,6,7,8,9,1,2,3,4},
            {8,9,1,2,3,4,5,6,7},
            {3,4,5,6,7,8,9,1,2},
            {6,7,8,9,1,2,3,4,5},
            {9,1,2,3,4,5,6,7,8},
        }
    };

    Board b( values );

    ASSERT_TRUE( b.sortedPossibilities().empty() );
}

TEST( BoardTests, simpleSolve )
{
    std::array<std::array<Num, 9>, 9> values{
        {
            {0,0,0,4,5,6,7,8,9},
            {4,5,6,7,8,9,1,2,3},
            {7,8,9,1,2,3,4,5,6},
            {2,3,4,0,6,7,8,9,1},
            {5,6,7,0,9,1,2,3,4},
            {8,9,1,0,3,4,5,6,7},
            {3,4,5,6,7,8,0,0,0},
            {6,7,8,9,1,2,3,4,5},
            {9,1,2,3,4,5,6,7,8},
        }
    };

    Board b( values );

    ASSERT_TRUE( b.sortedPossibilities().empty() );

    std::cout << "solved board:" << std::endl << b << std::endl;
}
