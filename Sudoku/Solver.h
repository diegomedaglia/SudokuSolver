#pragma once
#include "Board.h"
#include <unordered_set>

#ifdef DEBUG
#undef DEBUG
#endif

#ifdef NDEBUG
#define DEBUG( X ) 
#else
#include <iostream>
#define DEBUG( message ) { std::cout << message << std::endl; }
#endif

namespace Sudoku
{

/**
* @brief Solves the given board using backtracking.
* @param board The board to solve.
* @return The solved board, or 'board' if no solution was found.
*/
template<class BoardType>
BoardType solve( BoardType board )
{
    std::unordered_set<size_t> visitedStates;
    BoardType solution{};

    if( solve( board, visitedStates, solution ) )
    {
        DEBUG( "states visited: " << visitedStates.size() );
        return solution;
    }
    return board;
}

/**
* Solve a board using recursion backtracking.
* @param b The board to solve in this recursion
* @param visitedStates A set of hashes of boards to check if we already visited a given state
* @param solution The board to copy the solution to in case we solve
* @return True if the board was solved, false otherwise
*/
template<class BoardType>
bool solve( BoardType b, std::unordered_set<size_t>& visitedStates, BoardType& solution )
{
    static BoardHasher<BoardType> boardHasher;

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
        BoardType current( b );
        auto row = vals.row;
        auto col = vals.col;

        current.set( row, col, n );

        auto hash = boardHasher( current );

        if( visitedStates.find( hash ) != visitedStates.end() )
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
#ifndef NDEBUG
        else
        {
            auto offending = current.offendingVal();
            DEBUG( "NOT valid: " << std::get<0>( offending ) << ", " << std::get<1>( offending ) << ", "
                << std::get<2>( offending ) << ", " << std::get<3>( offending ) << ", " << std::get<4>( offending ) );
        }
#endif
    }

    return false;
}
}

#undef DEBUG
