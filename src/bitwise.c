//
// Created by LTMF on 1/27/2023.
//

#include "bitwise.h"

bool bitwise_check_bits_at (uint8_t *bits, uint8_t bitmask);
void bitwise_plop_bit_at (uint8_t *bits, uint8_t bitmask);
void bitwise_clear_bit_at (uint8_t *bits, uint8_t bitmask);

bool
bitwise_check_bits_at (uint8_t *bits, uint8_t bitmask)
{
  if ((*bits & bitmask) == bitmask)
    {
      return true;
    }
  return false;
}

void
bitwise_plop_bit_at (uint8_t *bits, uint8_t bitmask)
{
  *bits |= bitmask;
}

void
bitwise_clear_bit_at (uint8_t *bits, uint8_t bitmask)
{
  *bits &= ~bitmask;
}