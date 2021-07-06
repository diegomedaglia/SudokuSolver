#include <iostream>
#include <chrono>
#include "FileParser.h"
#include "Solver.h"

int main(int argc, char* argv[] )
{
    if( argc < 2 )
    {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl << std::endl;
        return 1;
    }

    Sudoku::Board<3> board;
    try
    {
        board = Sudoku::parseFile<Sudoku::Board<3>>( argv[1] );
    }
    catch( std::exception& ex )
    {
        std::cerr << "Failed to read file: " << ex.what() << std::endl << std::endl;
        return 2;
    }

    const auto start = std::chrono::steady_clock::now();

    auto s = Sudoku::solve( board );

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

