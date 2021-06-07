cmake_minimum_required (VERSION 3.8)

set( SOURCES 
    "Cell.cpp" 
    "Cell.h" 
    "Board.h" 
    "Board.cpp"  
    "Common.h"
    "Utils.h"  
    "Utils.cpp" 
    "FileParser.h" 
    "FileParser.cpp"
    "Solver.h"
    "Solver.cpp"
    )

# Add source to this project's executable.
add_library ( Sudoku ${SOURCES} )

target_include_directories( Sudoku 
                PUBLIC
                "${CMAKE_CURRENT_SOURCE_DIR}"
)
