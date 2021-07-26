#include "gtest/gtest.h"

#include "Board.h"
#include "Utils.h"
#include "BoardHasher.h"

using namespace Sudoku;

using TestBoard = Board;

TEST( BoardTests, CTOR )
{
    for( Num dim = 2; dim < 6; ++dim )
    {
        TestBoard b( dim );

        for( Num i = 0; i < b.dimension(); ++i )
        {
            for( Num j = 0; j < b.dimension(); ++j )
            {
                auto cell = b.cell( i, j );
                size_t size = dim;
                Nums possibilities( size * size );
                std::iota( possibilities.begin(), possibilities.end(), static_cast<Num>( 1u ) );
                ASSERT_EQ( cell.possibilities(), possibilities );
            }
        }
    }
    
}

TEST( BoardTests, updateInRow )
{
    TestBoard b(3);

    b.set( 0, 0, 1 );
    auto cell = b.cell( 0, 0 );
    ASSERT_TRUE( cell.hasVal() );
    ASSERT_EQ( cell.getVal(), 1 );


    EXPECT_FALSE( contains( b.cell( 0, 1 ).possibilities(), 1 ) );
    EXPECT_FALSE( contains( b.cell( 0, 2 ).possibilities(), 1 ) );
    EXPECT_FALSE( contains( b.cell( 0, 3 ).possibilities(), 1 ) );
    EXPECT_FALSE( contains( b.cell( 0, 4 ).possibilities(), 1 ) );
    EXPECT_FALSE( contains( b.cell( 0, 5 ).possibilities(), 1 ) );
    EXPECT_FALSE( contains( b.cell( 0, 6 ).possibilities(), 1 ) );
    EXPECT_FALSE( contains( b.cell( 0, 7 ).possibilities(), 1 ) );
    EXPECT_FALSE( contains( b.cell( 0, 8 ).possibilities(), 1 ) );
}

TEST( BoardTests, updateInCol )
{
    TestBoard b( 3 );

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
    TestBoard b( 3 );

    for( Num row = 0; row < 7; row += 3 )
    {
        for( Num col = 0; col < 7; col += 3 )
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
    TestBoard::InputArray values{
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
    
    TestBoard b( 3, values );

    for( Num i = 0; i < b.dimension(); ++i )
    {
        for( Num j = 0; j < b.dimension(); ++j )
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
    TestBoard::InputArray values{
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

    TestBoard b( 3, values );

        for( Num j = 1; j < 9; ++j )
        {
            auto cell = b.cell( 0, j );
            EXPECT_FALSE( cell.hasVal() );
            EXPECT_EQ( cell.possibilities(), ( Nums{2,3,4,5,6,7,8,9} ) );

            auto otherCell = b.cell( j, 0 );
            EXPECT_FALSE( cell.hasVal() );
            EXPECT_EQ( cell.possibilities(), ( Nums{ 2,3,4,5,6,7,8,9 } ) );
        }
}


TEST( BoardTests, CTOR_4 )
{
    TestBoard::InputArray values{
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

    TestBoard b( 3, values );

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
    TestBoard::InputArray values{
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

    TestBoard b( 3, values );

    EXPECT_EQ( b.at( 0, 7 ), 8 );
    EXPECT_EQ( b.at( 0, 8 ), 9 );
}

TEST( BoardTests, updateLoop2 )
{
    TestBoard::InputArray values{
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

    TestBoard b( 3, values );

    EXPECT_EQ( b.at( 0, 7 ), 8 );
    EXPECT_EQ( b.at( 0, 8 ), 9 );
}

TEST( BoardTests, sortedPossibilities )
{
    TestBoard::InputArray values{
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

    TestBoard b( 3, values );

    auto sorted = b.sortedPossibilities();

    EXPECT_EQ( sorted[0].row, 0 );
    EXPECT_EQ( sorted[0].col, 6 );
    EXPECT_EQ( sorted[0].possibilities.size(), 2 );
                        
    EXPECT_EQ( sorted[1].possibilities.size(), 3 );
    EXPECT_EQ( sorted[2].possibilities.size(), 3 );
}

TEST( BoardTests, invalidBoardRow )
{
    TestBoard::InputArray values{
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
    
    std::shared_ptr<TestBoard> board;

    ASSERT_THROW( board.reset( new TestBoard( 3, values ) ), std::invalid_argument ) << board;
}

TEST( BoardTests, invalidBoardCol )
{
    TestBoard::InputArray values{
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
    
    std::shared_ptr<TestBoard> board;

    ASSERT_THROW( board.reset( new TestBoard( 3, values ) ), std::invalid_argument ) << board;
}

TEST( BoardTests, invalidBoardQuadrant )
{
    TestBoard::InputArray values{
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

    std::shared_ptr<TestBoard> board;

    ASSERT_THROW( board.reset( new TestBoard( 3, values ) ), std::invalid_argument ) << board;
}

TEST( BoardTests, solved )
{
    TestBoard::InputArray values{
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

    TestBoard b( 3, values );

    ASSERT_TRUE( b.sortedPossibilities().empty() );
    ASSERT_TRUE( b.isSolved() );
}

TEST( BoardTests, simpleSolve )
{
    TestBoard::InputArray values{
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

    TestBoard b( 3, values );

    ASSERT_TRUE( b.sortedPossibilities().empty() );
    ASSERT_TRUE( b.isSolved() );

    std::cout << "solved board:" << std::endl << b << std::endl;
}

TEST( BoardTests, compare1 )
{
    TestBoard b1( 3 );
    TestBoard b2( 3 );

    ASSERT_EQ( b1, b2 );
}

TEST( BoardTests, compare2 )
{
    TestBoard b1( 3 );
    TestBoard b2( 3,
        {
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
        } 
    );

    ASSERT_NE( b1, b2 );
}

TEST( BoardTests, compare3 )
{
    TestBoard::InputArray values{
        {
            {0, 0, 0, 4, 5, 6, 7, 8, 9},
            { 4,5,6,7,8,9,1,2,3 },
            { 7,8,9,1,2,3,4,5,6 },
            { 2,3,4,0,6,7,8,9,1 },
            { 5,6,7,0,9,1,2,3,4 },
            { 8,9,1,0,3,4,5,6,7 },
            { 3,4,5,6,7,8,0,0,0 },
            { 6,7,8,9,1,2,3,4,5 },
            { 9,1,2,3,4,5,6,7,8 },
        }
    };
    
    TestBoard b1( 3, values );
    TestBoard b2( 3, values );

    ASSERT_EQ( b1, b2 );
}

TEST( BoardTests, compare4 )
{
    TestBoard::InputArray values{
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

    TestBoard b1( 3, values );
    TestBoard b2( 3, values );

    b2.set( 8, 8, 1 );

    ASSERT_NE( b1, b2 );
}

TEST( BoardTests, getRow )
{
    TestBoard::InputArray values{
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

    TestBoard b1( 3, values );
    
    auto row = b1.getRowCells( 1 );

    EXPECT_EQ( row[0]->getVal(), 5 );
    EXPECT_EQ( row[6]->getVal(), 6 );
}

TEST( BoardTests, getCol )
{
    TestBoard::InputArray values{
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

    TestBoard b1( 3, values );

    auto col = b1.getColCells( 1 );

    EXPECT_EQ( col[1]->getVal(), 9 );
    EXPECT_EQ( col[2]->getVal(), 6 );
}

TEST( BoardTests, getQuadrantCells )
{
    TestBoard::InputArray values{
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

    TestBoard b1( 3, values );

    auto q = b1.getQuadrantCells( 2 );

    EXPECT_EQ( q[3]->getVal(), 6 );
    EXPECT_EQ( q[7]->getVal(), 8 );
}

TEST( BoardTests, updateGroup1 )
{
    TestBoard::InputArray values{
        {
            {0,0,0,0,0,0,0,0,0},
            {1,7,0,0,0,0,0,0,0},
            {2,6,0,0,0,0,0,0,0},
            {3,2,0,0,0,0,0,0,0},
            {4,1,0,0,0,0,0,0,0},
            {5,0,0,0,0,0,0,0,0},
            {6,5,0,0,0,0,0,0,0},
            {7,4,0,0,0,0,0,0,0},
            {0,3,0,0,0,0,0,0,0},
        }
    };

    TestBoard b1( 3, values );

    ASSERT_EQ( b1.cell( 0, 0 ).possibilities(), ( Nums{ 8,9 } ) );
    ASSERT_EQ( b1.cell( 0, 1 ).possibilities(), ( Nums{ 8,9 } ) );

    for( Num i = 2; i < b1.dimension(); ++i )
    {
        auto possibilities = b1.cell( 0, i ).possibilities();
        EXPECT_FALSE( contains( possibilities, 8 ) );
        EXPECT_FALSE( contains( possibilities, 9 ) );
    }

    EXPECT_FALSE( contains( b1.cell( 1, 2 ).possibilities(), 8 ) );
    EXPECT_FALSE( contains( b1.cell( 1, 2 ).possibilities(), 9 ) );
                                        
    EXPECT_FALSE( contains( b1.cell( 2, 2 ).possibilities(), 8 ) );
    EXPECT_FALSE( contains( b1.cell( 2, 2 ).possibilities(), 9 ) );
}

TEST( BoardTests, updateGroup2 )
{
    TestBoard::InputArray values{
        {
            {0,1,2,3,4,5,6,7,0},
            {0,7,6,2,1,0,5,4,3},
            {0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0},

        }
    };

    TestBoard b1( 3, values );

    ASSERT_EQ( b1.cell( 0, 0 ).possibilities(), ( Nums{ 8,9 } ) );
    ASSERT_EQ( b1.cell( 1, 0 ).possibilities(), ( Nums{ 8,9 } ) );

    for( Num i = 2; i < b1.dimension(); ++i )
    {
        auto possibilities = b1.cell( i, 0 ).possibilities();
        EXPECT_FALSE( contains( possibilities, 8 ) );
        EXPECT_FALSE( contains( possibilities, 9 ) );
    }
    EXPECT_FALSE( contains( b1.cell( 2, 1 ).possibilities(), 8 ) );
    EXPECT_FALSE( contains( b1.cell( 2, 2 ).possibilities(), 9 ) );
                                     
    EXPECT_FALSE( contains( b1.cell( 2, 1 ).possibilities(), 8 ) );
    EXPECT_FALSE( contains( b1.cell( 2, 2 ).possibilities(), 9 ) );
}

TEST( BoardTests, hash )
{
    BoardHasher boardHasher{};
    TestBoard b( 3 );
    TestBoard b2( 3 );

    EXPECT_EQ( boardHasher( b ), boardHasher( b2 ) );

    TestBoard::InputArray values{
        {
            {0,1,2,3,4,5,6,7,0},
            {0,7,6,2,1,0,5,4,3},
            {0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0},

        }
    };

    TestBoard b1( 3, values );

    EXPECT_NE( boardHasher( b1 ), boardHasher( b2 ) );

    TestBoard b3( 3, values );

    EXPECT_EQ( boardHasher( b1 ), boardHasher( b3 ) );

    b3.set( 8, 8, 1 );
    EXPECT_NE( boardHasher( b1 ), boardHasher( b3 ) );
}

TEST( BoardTests, printBoard )
{
    TestBoard::InputArray values{
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

    TestBoard b( 3, values );

    ASSERT_NO_THROW( std::cout << b << std::endl );
}
