#include "Solver.h"
#include <iostream>
#include <unordered_set>

#define DEBUG( message ) // { std::cout << message << std::endl; }

static bool solve( Board b, std::unordered_set<size_t>& visitedStates, Board& solution )
{
    static BoardHasher boardHasher;

    DEBUG( "Board is " << std::endl << b );

    if( b.isSolved() )
    {
        DEBUG( "Solved!!!" << std::endl << b );
        solution = b;
        return true;
    }
    DEBUG( "not solved" );

    auto list = b.sortedPossibilities();

    if( list.empty() )
        return false;

    auto& vals = list.front();

    for( auto& n : vals.possibilities )
    {
        Board current( b );
        auto row = vals.row;
        auto col = vals.col;

        current.set( row, col, n );

        auto hash = boardHasher( current );

        if( std::find( visitedStates.begin(), visitedStates.end(), hash ) != visitedStates.end() )
            continue;

        DEBUG( "not yet visited" );

        visitedStates.insert( hash );

        DEBUG( "Trying (" << row << "," << col << ") set to " << static_cast< int >( n ) );
        DEBUG( "board after update: " << std::endl << current << std::endl );

        if( current.isValid() )
        {
            if( solve( current, visitedStates, solution ) )
            {
                return true;
            }
        }
        else
        {
            auto offending = current.offendingVal();
            DEBUG( "NOT valid: " << std::get<0>( offending ) << ", " << std::get<1>( offending ) << ", "
                << std::get<2>( offending ) << ", " << std::get<3>( offending ) << ", " << std::get<4>( offending ) );
        }
    }

    return false;
}

Board solve( Board board )
{
    std::unordered_set<size_t> visitedStates;
    Board solution;

    if( solve( board, visitedStates, solution ) )
    {
        DEBUG( "states visited: " << visitedStates.size() );
        return solution;
    }
    return {};
}