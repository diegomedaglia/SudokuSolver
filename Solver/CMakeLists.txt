cmake_minimum_required (VERSION 3.11 )


if ( NOT DEFINED CMAKE_BUILD_TYPE OR NOT ${CMAKE_BUILD_TYPE} STREQUAL "Debug" )
	add_compile_definitions(NDEBUG)
endif()

set( SOURCES 
    main.cpp
)

add_executable( Solver ${SOURCES} )
target_link_libraries(Solver Sudoku)
