#include <unordered_set>
#include <string>

#include "Solver.h"
#include "BoardHasher.h"
#include "Utils.h"


#ifdef DEBUG
#undef DEBUG
#endif

#ifdef NDEBUG
#define DEBUG( X ) 
#else
#include <iostream>
#define DEBUG( message ) { std::cout << message << std::endl; }
#endif

using namespace Sudoku;

namespace Sudoku{
    bool solve( Board b, std::unordered_set<size_t>& visitedStates, Board& solution );
}

/**
* Solve a board using recursion backtracking.
* @param b The board to solve in this recursion
* @param visitedStates A set of hashes of boards to check if we already visited a given state
* @param solution The board to copy the solution to in case we solve
* @return True if the board was solved, false otherwise
*/
bool Sudoku::solve( Board b, std::unordered_set<size_t>& visitedStates, Board& solution )
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

        if( visitedStates.find( hash ) != visitedStates.end() )
            continue;

        DEBUG( "not yet visited" );

        visitedStates.insert( hash );

        DEBUG( "Trying (" << std::to_string( row ) << "," << std::to_string( col ) << ") set to " << static_cast< int >( n ) );
        DEBUG( "board after update: " << std::endl << current << std::endl );

        if( current.isValid() )
        {
            if( solve( current, visitedStates, solution ) )
            {
                return true;
            }
        }
#ifndef NDEBUG
        else
        {
            auto offending = current.offendingVal();
            DEBUG( "NOT valid: " << std::to_string( std::get<0>( offending ) )<< ", " << std::to_string( std::get<1>( offending ) )<< ", "
                << std::to_string( std::get<2>( offending ) ) << ", " << std::to_string( std::get<3>( offending ) ) << ", " << std::to_string( std::get<4>( offending ) ) );
        }
#endif
    }

    return false;
}


/**
* @brief Solves the given board using backtracking.
* @param board The board to solve.
* @return The solved board, or 'board' if no solution was found.
*/
Board Sudoku::solve( Board board )
{
    std::unordered_set<size_t> visitedStates;
    Board solution{ board.blockSize() };

    if( solve( board, visitedStates, solution ) )
    {
        DEBUG( "states visited: " << visitedStates.size() );
        return solution;
    }
    return board;
}
