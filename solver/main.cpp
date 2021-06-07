#include <iostream>
#include <unordered_set>
#include <iomanip>
#include <chrono>
#include "Board.h"
#include "Utils.h"
#include "FileParser.h"

#define DEBUG( message ) // { std::cout << message << std::endl; }

bool solve( Board b, std::unordered_set<size_t>& visitedStates, Board& solution )
{
    static BoardHasher boardHasher;

    DEBUG( "Board is " << std::endl << b );

    if( b.isSolved() )
    {
        DEBUG( "Solved!!!" << std::endl<< b );
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
        Board current (b);
        auto row = vals.row;
        auto col = vals.col;

        current.set( row, col, n );

        auto hash = boardHasher( current );

        if( std::find( visitedStates.begin(), visitedStates.end(), hash ) != visitedStates.end() )
            continue;

        DEBUG( "not yet visited" );

        visitedStates.insert( hash );

        DEBUG( "Trying (" << row << "," << col << ") set to " << static_cast<int>( n ) );
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
                << std::get<2>( offending ) << ", " << std::get<3>( offending ) << ", " << std::get<4>( offending ));
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

int main(int argc, char* argv[] )
{
    if( argc < 2 )
    {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl << std::endl;
        return 1;
    }

    Board board;
    try
    {
        board = parseFile( argv[1] );
    }
    catch( std::exception& ex )
    {
        std::cerr << "Failed to read file: " << ex.what() << std::endl << std::endl;
        return 2;
    }

    const auto start = std::chrono::steady_clock::now();

    Board s = solve( board );

    const auto end = std::chrono::steady_clock::now();

    if( s.isSolved() )
    {
        std::cout << "solved sudoku!" << std::endl <<
            s << std::endl;
    }
    else
    {
        std::cout << "Could not solve!" << std::endl;
    }
    
    std::chrono::duration<double> elapsed_seconds = end - start;
    
    auto hours = std::chrono::duration_cast< std::chrono::hours >( elapsed_seconds ).count();
    auto minutes = std::chrono::duration_cast< std::chrono::minutes >( elapsed_seconds ).count() % 60;
    auto seconds = std::chrono::duration_cast< std::chrono::seconds >( elapsed_seconds ).count() % 60;

    std::cout << "Took " << hours << "h " << minutes << "m " << seconds << "s" << std::endl;

    return 0;
}
