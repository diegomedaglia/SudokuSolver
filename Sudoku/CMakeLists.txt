cmake_minimum_required (VERSION 3.11)

set( SOURCES 
    "Board.cpp"
    "Board.h"
    "BoardHasher.cpp"
    "BoardHasher.h"
    "Cell.cpp"
    "Cell.h"
    "Common.h"
    "FileParser.cpp"
    "FileParser.h"
    "Solver.cpp"
    "Solver.h"
    "Utils.cpp"
    "Utils.h"
    )
    
add_library ( Sudoku ${SOURCES} )

target_include_directories( Sudoku 
                PUBLIC
                "${CMAKE_CURRENT_SOURCE_DIR}"
)
