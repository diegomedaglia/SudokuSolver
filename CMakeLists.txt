
cmake_minimum_required (VERSION 3.11 )
project ("Sudoku")

set(CMAKE_CXX_STANDARD 14)
if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

if ( NOT DEFINED CMAKE_BUILD_TYPE OR NOT "${CMAKE_BUILD_TYPE}" STREQUAL "Debug" )
	add_compile_definitions(NDEBUG)
endif()

enable_testing()
add_subdirectory ("Sudoku")
add_subdirectory ("solver")
add_subdirectory ("Tests")

