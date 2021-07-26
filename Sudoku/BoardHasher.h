#pragma once
#include <functional>
#include "Board.h"

namespace Sudoku
{

/**
* @brief Hasher for a board.
*/
class BoardHasher
{
public:
    /**
    * @brief Calculates the hash of the board.
    * @param b the board to calculate the hash for
    * @return the board's hash.
    */
    std::size_t operator()( Board const& b ) const noexcept;

private:
    std::hash<std::uint64_t> u64Hasher;
    /**
    * @brief boost::hash_combine
    */
    static void combineHash( size_t& seed, const size_t& hash ) noexcept;
};

} // namespace
