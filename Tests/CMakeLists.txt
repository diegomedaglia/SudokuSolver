cmake_minimum_required (VERSION 3.8)

enable_testing()
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.10.0.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


add_executable(SudokuTests  "CellTests.cpp" "BoardTests.cpp" "FreeFunctions.cpp" "FileParserTests.cpp")
target_link_libraries(SudokuTests Sudoku gmock gtest gtest_main)
add_test(SudokuTests SudokuTests)

add_custom_command(
        TARGET SudokuTests POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_CURRENT_SOURCE_DIR}/FileParserTestInput
                ${CMAKE_CURRENT_BINARY_DIR}/)