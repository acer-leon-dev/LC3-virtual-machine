#ifndef B4287C87_D586_4153_841C_83F90DCA0FAB
#define B4287C87_D586_4153_841C_83F90DCA0FAB

#include "LC3VM/lc3_types.h"


namespace lc3::bit::label
{

typedef uint32_t bit_label;
inline constexpr bit_label msb = -1;
    
}


namespace lc3::bit {

lc3_size_t get_arg(lc3_size_t instruction, uint32_t left, uint32_t right);

lc3_size_t get_arg(lc3_size_t instruction, uint32_t bit);

///
/// @brief Extends the given signed integer to 16 bits.
///
/// @param x            The value to extend
/// @param bit_count    The bit width of `x`
/// @return             `x` extended to 16 bits
///
lc3_size_t sext(lc3_size_t x, uint32_t bit_count);

// ///
// /// @brief Extends the given integer to 16 bits.
// ///
// /// @param x            The value to extend
// /// @param bit_count    The bit width of `x`
// /// @return             `x` extended to 16 bits
// ///
// lc3_size_t zext(lc3_size_t x, uint32_t bit_count);

} // lc3::bit

#endif /* B4287C87_D586_4153_841C_83F90DCA0FAB */
