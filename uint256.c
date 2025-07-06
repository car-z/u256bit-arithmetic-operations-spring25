/*
 * Defining functions which operate on uint256
 * CSF Assignment 1
 * Miranda Qing
 * mqing2@jhu.edu
 * Caroline Zhao
 * czhao67@jhu.edu
 */
 
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "uint256.h"

// Create a UInt256 value from a single uint32_t value.
// Only the least-significant 32 bits are initialized directly,
// all other bits are set to 0.
UInt256 uint256_create_from_u32( uint32_t val ) {
  UInt256 result;

  result.data[0] = val;
  for (int i = 1; i < 8; i++) {
    result.data[i] = 0;
  }

  return result;
}

// Create a UInt256 value from an array of 8 uint32_t values.
// The element at index 0 is the least significant, and the element
// at index 7 is the most significant.
UInt256 uint256_create( const uint32_t data[8] ) {
  UInt256 result;

  for (int i = 0; i < 8; i++) {
    result.data[i]=data[i];
  }

  return result;
}

// Create a UInt256 value from a string of hexadecimal digits.
UInt256 uint256_create_from_hex( const char *hex ) {
  UInt256 result;
  int last = strlen(hex) - 1;
  for (int i = 0; i < 8; i++) {
    char curr[9];
    memset(curr, '0', 8);
    curr[8] = '\0';
    for (int j = 0; j < 8; j++) {
      int index = last - (i * 8 + j);
      if (index >= 0) {
        curr[7-j] = hex[index];
      }
    }
    uint32_t bits32 = strtoul(curr, NULL, 16);
    result.data[i] = bits32;
  }
  return result;
}

// Return a dynamically-allocated string of hex digits representing the
// given UInt256 value.
char *uint256_format_as_hex( UInt256 val ) {
  char *hex = NULL;
  bool first = true;
  int hexIndex = 0;
  int stopIndex = 0;

  for (int i = 7; i >=0; i--) {
    uint32_t bits32 = val.data[i];
    if (first && bits32 == 0 && i!= 0) {
      //unnecessary leading zeros (MSB not reached)
      continue;
    }
    char curr[9];
    if (first) {
      //formatting without leading zeros
      sprintf(curr,"%x",bits32);
      //getting actual length of most significant hex chunk
      int len = (int) strlen(curr);
      stopIndex = len;
      
      //allocating sufficient amount of char elements to hold hex value
      hex = malloc((i*8+len+1)*sizeof(char));
      if (hex == NULL) {
        printf("failed to allocate memory");
        return NULL;
      }

      first = false;

    } else {
      //formatting with leading zeros
      sprintf(curr, "%08x", bits32);  
      stopIndex = 8;    
    }
    for (int n = 0; n < stopIndex; n++) {
        hex[hexIndex] = curr[n];
        hexIndex++;
    }
  }

  hex[hexIndex] = '\0';
  return hex;
}

// Get 32 bits of data from a UInt256 value.
// Index 0 is the least significant 32 bits, index 7 is the most
// significant 32 bits.
uint32_t uint256_get_bits( UInt256 val, unsigned index ) {
  uint32_t bits;
 
  bits = val.data[index];

  return bits;
}

// Return 1 if bit at given index is set, 0 otherwise.
int uint256_is_bit_set( UInt256 val, unsigned index ) {
  uint32_t bits32 = val.data[index / 32];
  if (bits32 & (1 << (index % 32))) {
    return 1;
  }
  return 0;
}

// Compute the sum of two UInt256 values.
UInt256 uint256_add( UInt256 left, UInt256 right ) {
  UInt256 sum;
  uint64_t carry = 0;

  for (int i = 0; i < 8; i++) {
    uint64_t temp = (uint64_t) left.data[i] + (uint64_t) right.data[i] + carry;
    sum.data[i] = (uint32_t) temp;

    carry = temp >> 32 ;
  }
  return sum;
}

// Compute the difference of two UInt256 values.
UInt256 uint256_sub( UInt256 left, UInt256 right ) {
  UInt256 result;
  
  UInt256 negateRight = uint256_negate(right);

  result = uint256_add(left, negateRight);

  return result;
}

// Return the two's-complement negation of the given UInt256 value.
UInt256 uint256_negate( UInt256 val ) {
  UInt256 result;
  
  //flipping each bit in val
  for (int i = 0; i < 8; i++) {
    uint32_t bits32 = val.data[i];
    result.data[i] = ~bits32;
  }

  //adding one to result
  UInt256 one = {{1, 0, 0, 0, 0, 0, 0, 0} };
  result = uint256_add(result, one);

  return result;
}

// Compute the product of two UInt256 values.
UInt256 uint256_mul( UInt256 left, UInt256 right ) {
  UInt256 product = {{ 0, 0, 0, 0, 0, 0, 0, 0}};

  for (int i = 0; i < 8; i++) {
    uint32_t bits32 = left.data[i];
    
    for (int n = 0; n < 32; n++) {
      if (bits32 & ( 1 << n )) {
        UInt256 result = uint256_lshift(right, 32*i + n);
        product = uint256_add(product, result);
      }
    }
  }

  return product;
}

UInt256 uint256_lshift( UInt256 val, unsigned shift ) {
  assert( shift < 256 );
  UInt256 result;

  int shiftBy32 = shift / 32;
  if (shiftBy32 != 0) {
    for (int i = 7; i >= shiftBy32; i--) {
      result.data[i] = val.data[i-shiftBy32];
    }
    for (int i = 0; i < shiftBy32; i++) {
      result.data[i] = 0;
    }
  } else {
    for (int i = 0; i < 8; i++) {
      result.data[i] = val.data[i];
    }
  }

  int shiftIn32 = shift % 32;
  if (shiftIn32 != 0) {
    uint64_t temp;
    uint32_t shiftedNum = 0;
    for (int i = 0; i < 8; i++) {
      temp = (uint64_t) result.data[i];
      temp = temp << shiftIn32;
      temp += shiftedNum;
      shiftedNum = (uint32_t) (temp >> 32);
      result.data[i] = (uint32_t) temp;
    }
  }

  return result;
}

//HELPER FUNCTION TO PRINT ALL BITS
void uint256_print_bits ( UInt256 val ) {
  printf("\nprinting bits: ");

  for ( int i = 0; i < 8; i++ ) {
    uint32_t bits32 = val.data[i];
    for (int k = 0; k < 32; k++) {
      if (bits32 & ( 1 << k )) {
        printf("%u", 1);
      } else {
        printf("%u", 0);
      }
    }
    printf(",");
  }
  printf("\n");
}
