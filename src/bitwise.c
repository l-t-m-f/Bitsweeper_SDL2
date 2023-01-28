//
// Created by LTMF on 1/27/2023.
//

#include "../include/bitwise.h"

bool bitwise_check_bits_at(uint8_t bits, uint8_t bitmask);
void bitwise_plop_bit_at(uint8_t *bits, uint8_t bitmask);
void bitwise_clear_bit_at(uint8_t *bits, uint8_t bitmask);

enum bitmask {
   BIT_MINE,
   BIT_REVEAL,
   BIT_FLAG_1,
   BIT_FLAG_2,
   //
   BIT_NEIGHBOR_N = 7,
   BIT_NEIGHBOR_NW,
   BIT_NEIGHBOR_W,
   BIT_NEIGHBOR_SW,
   BIT_NEIGHBOR_S,
   BIT_NEIGHBOR_SE,
   BIT_NEIGHBOR_E,
   BIT_NEIGHBOR_NE,
};


bool bitwise_check_bits_at(uint8_t bits, uint8_t bitmask)
{
   if ((bits & bitmask) == bitmask)
      {
         return true;
      }
   return false;
}

void bitwise_plop_bit_at(uint8_t *bits, uint8_t bitmask)
{
  *bits |= bitmask;
}

void bitwise_clear_bit_at(uint8_t *bits, uint8_t bitmask)
{
  *bits &= ~bitmask;
}