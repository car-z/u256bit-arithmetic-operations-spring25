/*
 * Testing functions which operate on uint256
 * CSF Assignment 1
 * Miranda Qing
 * mqing2@jhu.edu
 * Caroline Zhao
 * czhao67@jhu.edu
 */

#include <stdio.h>
#include <stdlib.h>
#include "tctest.h"

#include "uint256.h"

typedef struct {
  UInt256 zero; // the value equal to 0
  UInt256 one;  // the value equal to 1
  UInt256 max;  // the value equal to (2^256)-1
  UInt256 msb_set; // the value equal to 2^255
  UInt256 msb_of_ls32b; //value equal to 2^31 (most significant bit of a 32 bit number)
} TestObjs;

// Helper functions for implementing tests
void set_all( UInt256 *val, uint32_t wordval );

#define ASSERT_SAME( expected, actual ) \
do { \
  ASSERT( expected.data[0] == actual.data[0] ); \
  ASSERT( expected.data[1] == actual.data[1] ); \
  ASSERT( expected.data[2] == actual.data[2] ); \
  ASSERT( expected.data[3] == actual.data[3] ); \
  ASSERT( expected.data[4] == actual.data[4] ); \
  ASSERT( expected.data[5] == actual.data[5] ); \
  ASSERT( expected.data[6] == actual.data[6] ); \
  ASSERT( expected.data[7] == actual.data[7] ); \
} while ( 0 )

#define INIT_FROM_ARR( val, arr ) \
do { \
  for ( int i = 0; i < 8; ++i ) \
    val.data[i] = arr[i]; \
} while ( 0 )

// Functions to create and cleanup the test fixture object
TestObjs *setup( void );
void cleanup( TestObjs *objs );

// Declarations of test functions
void test_get_bits( TestObjs *objs );
void test_is_bit_set( TestObjs *objs );
void test_create_from_u32( TestObjs *objs );
void test_create( TestObjs *objs );
void test_create_from_hex( TestObjs *objs );
void test_create_from_hex_2 ( TestObjs *objs );
void test_format_as_hex( TestObjs *objs );
void test_format_as_hex_2( TestObjs *objs );
void test_add( TestObjs *objs );
void test_add_2(TestObjs *objs);
void test_sub( TestObjs *objs );
void test_negate( TestObjs *objs );
void test_negate_2( TestObjs *objs );
void test_neg_overflow( TestObjs *objs );
void test_mul( TestObjs *objs );
void test_lshift( TestObjs *objs );
void test_lshift_2( TestObjs *objs );

int main( int argc, char **argv ) {
  if ( argc > 1 )
    tctest_testname_to_execute = argv[1];

  TEST_INIT();

  TEST( test_get_bits );
  TEST( test_is_bit_set );
  TEST( test_create_from_u32 );
  TEST( test_create );
  TEST( test_create_from_hex );
  TEST( test_create_from_hex_2 );
  TEST( test_format_as_hex );
  TEST( test_format_as_hex_2 );
  TEST( test_add );
  TEST( test_add_2 );
  TEST( test_sub );
  TEST( test_negate );
  TEST( test_negate_2 );
  TEST( test_neg_overflow );
  TEST( test_mul );
  TEST( test_lshift );
  TEST( test_lshift_2 );

  TEST_FINI();
}

// Set all of the "words" of a UInt256 to a specific initial value
void set_all( UInt256 *val, uint32_t wordval ) {
  for ( unsigned i = 0; i < 8; ++i ) {
    val->data[i] = wordval;
  }
}

TestObjs *setup( void ) {
  TestObjs *objs = (TestObjs *) malloc( sizeof(TestObjs ) );

  // initialize several UInt256 values "manually"
  set_all( &objs->zero, 0 );
  set_all( &objs->one, 0 );
  objs->one.data[0] = 1U;
  set_all( &objs->max, 0xFFFFFFFFU );

  // create a value with only the most-significant bit set
  uint32_t msb_set_data[8] = { 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0x80000000U };
  INIT_FROM_ARR( objs->msb_set, msb_set_data );

  //create a value where only most-significant bit of first uint32 of UInt256 is set
  uint32_t msb_of_least_significant_32[8] = { 0x80000000U , 0U, 0U, 0U, 0U, 0U, 0U, 0U, };
  INIT_FROM_ARR( objs->msb_of_ls32b, msb_of_least_significant_32 );

  return objs;
}

void cleanup( TestObjs *objs ) {
  free( objs );
}

void test_get_bits( TestObjs *objs ) {
  ASSERT( 0U == uint256_get_bits( objs->zero, 0 ) );
  ASSERT( 0U == uint256_get_bits( objs->zero, 1 ) );
  ASSERT( 0U == uint256_get_bits( objs->zero, 2 ) );
  ASSERT( 0U == uint256_get_bits( objs->zero, 3 ) );
  ASSERT( 0U == uint256_get_bits( objs->zero, 4 ) );
  ASSERT( 0U == uint256_get_bits( objs->zero, 5 ) );
  ASSERT( 0U == uint256_get_bits( objs->zero, 6 ) );
  ASSERT( 0U == uint256_get_bits( objs->zero, 7 ) );

  ASSERT( 1U == uint256_get_bits( objs->one, 0 ) );
  ASSERT( 0U == uint256_get_bits( objs->one, 1 ) );
  ASSERT( 0U == uint256_get_bits( objs->one, 2 ) );
  ASSERT( 0U == uint256_get_bits( objs->one, 3 ) );
  ASSERT( 0U == uint256_get_bits( objs->one, 4 ) );
  ASSERT( 0U == uint256_get_bits( objs->one, 5 ) );
  ASSERT( 0U == uint256_get_bits( objs->one, 6 ) );
  ASSERT( 0U == uint256_get_bits( objs->one, 7 ) );

  ASSERT( 0xFFFFFFFFU == uint256_get_bits( objs->max, 0 ) );
  ASSERT( 0xFFFFFFFFU == uint256_get_bits( objs->max, 1 ) );
  ASSERT( 0xFFFFFFFFU == uint256_get_bits( objs->max, 2 ) );
  ASSERT( 0xFFFFFFFFU == uint256_get_bits( objs->max, 3 ) );
  ASSERT( 0xFFFFFFFFU == uint256_get_bits( objs->max, 4 ) );
  ASSERT( 0xFFFFFFFFU == uint256_get_bits( objs->max, 5 ) );
  ASSERT( 0xFFFFFFFFU == uint256_get_bits( objs->max, 6 ) );
  ASSERT( 0xFFFFFFFFU == uint256_get_bits( objs->max, 7 ) );

  ASSERT( 0x80000000U == uint256_get_bits( objs->msb_of_ls32b, 0 ) );
  ASSERT( 0U == uint256_get_bits( objs->msb_of_ls32b, 1 ) );
  ASSERT( 0U == uint256_get_bits( objs->msb_of_ls32b, 2 ) );
  ASSERT( 0U == uint256_get_bits( objs->msb_of_ls32b, 3 ) );
  ASSERT( 0U == uint256_get_bits( objs->msb_of_ls32b, 4 ) );
  ASSERT( 0U == uint256_get_bits( objs->msb_of_ls32b, 5 ) );
  ASSERT( 0U == uint256_get_bits( objs->msb_of_ls32b, 6 ) );
  ASSERT( 0U == uint256_get_bits( objs->msb_of_ls32b, 7 ) );
}

void test_is_bit_set( TestObjs *objs ) {
  // no bits should be set in the zero value
  for ( int i = 0; i < 256; ++i )
    ASSERT( !uint256_is_bit_set( objs->zero, i ) );

  // only bit 0 should be set in the one value
  ASSERT( uint256_is_bit_set( objs->one, 0 ) );
  for ( int i = 1; i < 256; ++i )
    ASSERT( !uint256_is_bit_set( objs->one, i ) );

  // only bit 255 should be set in the msb_set value
  for ( int i = 0; i < 255; ++i )
  ASSERT( !uint256_is_bit_set( objs->msb_set, i ) );
  ASSERT( uint256_is_bit_set( objs->msb_set, 255 ) );

  // all bits should be set in the max value
  for ( int i = 0; i < 256; ++i )
    ASSERT( uint256_is_bit_set( objs->max, i ) );
}

void test_create_from_u32( TestObjs *objs ) {
  UInt256 zero = uint256_create_from_u32( 0U );
  UInt256 one = uint256_create_from_u32( 1U );
  UInt256 max32 = uint256_create_from_u32( 2147483648U );

  ASSERT_SAME( objs->zero, zero );
  ASSERT_SAME( objs->one, one );
  ASSERT_SAME( objs->msb_of_ls32b, max32);
}

void test_create( TestObjs *objs ) {
  (void) objs;

  uint32_t data1[8] = { 1U, 2U, 3U, 4U, 5U, 6U, 7U, 8U };
  UInt256 val1 = uint256_create( data1 );
  ASSERT( 1U == val1.data[0] );
  ASSERT( 2U == val1.data[1] );
  ASSERT( 3U == val1.data[2] );
  ASSERT( 4U == val1.data[3] );
  ASSERT( 5U == val1.data[4] );
  ASSERT( 6U == val1.data[5] );
  ASSERT( 7U == val1.data[6] );
  ASSERT( 8U == val1.data[7] );

  uint32_t data2[8] = { 8U, 7U, 6U, 5U, 4U, 3U, 2U, 1U };
  UInt256 val2 = uint256_create( data2 );
  ASSERT( 1U == val2.data[7] );
  ASSERT( 2U == val2.data[6] );
  ASSERT( 3U == val2.data[5] );
  ASSERT( 4U == val2.data[4] );
  ASSERT( 5U == val2.data[3] );
  ASSERT( 6U == val2.data[2] );
  ASSERT( 7U == val2.data[1] );
  ASSERT( 8U == val2.data[0] );
}

void test_create_from_hex( TestObjs *objs ) {
  //testing when hex string has length < 8
  UInt256 zero = uint256_create_from_hex( "0" );
  ASSERT_SAME( objs->zero, zero );

  UInt256 one = uint256_create_from_hex( "1" );
  ASSERT_SAME( objs->one, one );

  //testing when hex string has length 64
  UInt256 max = uint256_create_from_hex( "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff" );
  ASSERT_SAME( objs->max, max );
}

void test_create_from_hex_2( TestObjs *objs ) {
  //testing when hex string has length > 64
  UInt256 moreThan64 = uint256_create_from_hex( "aaaaaaaaaaffffff23ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff" );
  ASSERT_SAME( objs->max, moreThan64);  

  //testing when hex string has length >8 and <64
  UInt256 moreThan8LessThan64 = uint256_create_from_hex("ffffffffffffffffffffffff");
  uint32_t data[8] = { 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0U, 0U, 0U, 0U, 0U };
  UInt256 val = uint256_create( data );
  ASSERT_SAME( val, moreThan8LessThan64);

  UInt256 zerosInMiddle = uint256_create_from_hex("ffffffff00000000ffffffff");
  uint32_t data2[8] = { 0xFFFFFFFFU, 0U, 0xFFFFFFFFU, 0U, 0U, 0U, 0U, 0U };
  UInt256 val2 = uint256_create( data2 );
  ASSERT_SAME( val2, zerosInMiddle);

  UInt256 notPerfectGroupsOf8 = uint256_create_from_hex("64ffffffff");
  uint32_t data3[8] = { 0xFFFFFFFFU, 100U, 0U, 0U, 0U, 0U, 0U, 0U };
  UInt256 val3 = uint256_create( data3 );
  ASSERT_SAME( val3, notPerfectGroupsOf8);
}

void test_format_as_hex( TestObjs *objs ) {
  char *s;

  //testing formatting hex string of less than 8 bits
  s = uint256_format_as_hex( objs->zero );
  ASSERT( 0 == strcmp( "0", s ) );
  free( s );

  s = uint256_format_as_hex( objs->one );
  ASSERT( 0 == strcmp( "1", s ) );
  free( s );

  //testing formatting hex string of 64 bits (which is the max)
  s = uint256_format_as_hex( objs->max );
  ASSERT( 0 == strcmp( "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff", s ) );
  free( s );

  //testing hex string for max-1
  UInt256 max_minus_one = uint256_sub(objs->max, objs->one);
  s = uint256_format_as_hex( max_minus_one);
  ASSERT( 0 == strcmp( "fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffe", s ) );
  free( s );
}

void test_format_as_hex_2( TestObjs *objs ) {
  char *s;

  //testing formatting hex string of exactly 8 bits
  s = uint256_format_as_hex( objs->msb_of_ls32b );
  ASSERT( 0 == strcmp("80000000", s ) );
  free ( s );

  //testing formatting hex string of more than 8 bits, less than 64 bits
  uint32_t data[8] = { 0xFFFFFFFFU, 100U, 0U, 0U, 0U, 0U, 0U, 0U };
  UInt256 val = uint256_create( data );
  s = uint256_format_as_hex( val );
  ASSERT( 0 == strcmp("64ffffffff", s ) );
  free ( s );

  //testing formatting hex string with zeros in middle
  uint32_t data2[8] = { 0xFFFFFFFFU, 0U, 0xFFFFFFFFU, 0U, 0U, 0U, 0U, 0U };
  UInt256 val2 = uint256_create( data2 );
  s = uint256_format_as_hex( val2 );
  ASSERT( 0 == strcmp("ffffffff00000000ffffffff", s ) );
  free ( s );
}

void test_add( TestObjs *objs ) {
  UInt256 result;

  //asserting that 0 + 0 = 0
  result = uint256_add( objs->zero, objs->zero );
  ASSERT_SAME( objs->zero, result );

  //asserting that 0 + 1 = 1
  result = uint256_add( objs->zero, objs->one );
  ASSERT_SAME( objs->one, result );

  //asserting that 1 + 1 = 2
  uint32_t two_data[8] = { 2U };
  UInt256 two;
  INIT_FROM_ARR( two, two_data );
  result = uint256_add( objs->one, objs->one );
  ASSERT_SAME( two, result );

  //asserting that adding one to max representable integer causes result to be zero
  result = uint256_add( objs->max, objs->one );
  ASSERT_SAME( objs->zero, result );

  //asserting that adding max+max results in max-1
  result = uint256_add(objs->max, objs->max );
  ASSERT_SAME( uint256_sub(objs->max, objs->one), result );

}

void test_add_2( TestObjs *objs ) {
  UInt256 result;

  //testing commutative property
  result = uint256_add( objs->one, objs->max );
  ASSERT_SAME( objs->zero, result );

  result = uint256_add( objs->one, objs->zero );
  ASSERT_SAME( objs->one, result );

  //asserting that overflow works as intended

  //carry from index 0 to index 1
  uint32_t data1[8] = { 0xFFFFFFFFU, 0U, 0U, 0U, 0U, 0U, 0U, 0U };
  UInt256 val1 = uint256_create( data1 );
  uint32_t data2[8] = { 0U, 1U, 0U, 0U, 0U, 0U, 0U, 0U };
  UInt256 expectedVal1 = uint256_create( data2 );
  result = uint256_add( val1, objs->one );
  ASSERT_SAME( expectedVal1, result);

  //carry from index 0 to index 2
  uint32_t data3[8] = { 0xFFFFFFFFU, 0xFFFFFFFFU, 0U, 0U, 0U, 0U, 0U, 0U };
  UInt256 val3 = uint256_create( data3 );
  uint32_t data4[8] = { 0U, 0U, 1U, 0U, 0U, 0U, 0U, 0U };
  UInt256 expectedVal2 = uint256_create( data4 );
  result = uint256_add( val3, objs->one );
  ASSERT_SAME( expectedVal2, result);

  //carry from index 0 to index 5
  uint32_t data5[8] = { 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0U, 0U, 0U };
  UInt256 val4 = uint256_create( data5 );
  uint32_t data6[8] = { 0U, 0U, 0U, 0U, 0U, 1U, 0U, 0U };
  UInt256 expectedVal3 = uint256_create( data6 );
  result = uint256_add( val4, objs->one );
  ASSERT_SAME( expectedVal3, result);

  //adding 1 to value with 0 in LSB
  uint32_t data7[8] = { 0U, 0U, 0xFFFFFFFFU, 0U, 0U, 0U, 0U, 0U };
  UInt256 val5 = uint256_create( data7 );
  uint32_t data8[8] = { 1U, 0U, 0xFFFFFFFFU, 0U, 0U, 0U, 0U, 0U };
  UInt256 expectedVal4 = uint256_create( data8 );
  result = uint256_add( val5, objs->one );
  ASSERT_SAME( expectedVal4, result);

  //adding two UInt256 values with bits only in 7th index of data array
  uint32_t data9[8] = { 0U, 0U, 0U, 0U, 0U, 0U, 0U, 1U };
  UInt256 val6 = uint256_create( data9 );
  uint32_t data10[8] = { 0U, 0U, 0U, 0U, 0U, 0U, 0U, 2U };
  UInt256 expectedVal5 = uint256_create( data10 );
  result = uint256_add( val6, val6 );
  ASSERT_SAME( expectedVal5, result);
}

void test_sub( TestObjs *objs ) {
  UInt256 result;

  result = uint256_sub( objs->zero, objs->zero );
  ASSERT_SAME( objs->zero, result );

  result = uint256_sub( objs->one, objs->one );
  ASSERT_SAME( objs->zero, result );

  result = uint256_sub( objs->zero, objs->one );
  ASSERT_SAME( objs->max, result );

  result = uint256_sub( objs->one, objs->zero );
  ASSERT_SAME( objs->one, result );

  //general cases
  UInt256 two = { { 2,0,0,0,0,0,0,0 } }, four = { { 4,0,0,0,0,0,0,0 } };
  result = uint256_sub(four, two);
  ASSERT_SAME(two, result);

  result = uint256_sub( objs->msb_of_ls32b, objs->one);
  UInt256 expectedResult = { { 0x7FFFFFFF, 0, 0, 0, 0, 0, 0, 0} };
  ASSERT_SAME(expectedResult, result);

  //since tested addition and negation, can assume subtraction works since built off addition and negation
}

void test_negate( TestObjs *objs ) {
  UInt256 result;

  result = uint256_negate( objs->zero );
  ASSERT_SAME( objs->zero, result );

  result = uint256_negate( objs->one );
  ASSERT_SAME( objs->max, result );

  result = uint256_negate( objs->max );
  ASSERT_SAME( objs->one, result );
}

void test_negate_2( TestObjs *objs ) {
  UInt256 result;

  //general case
  result = uint256_negate( objs->msb_of_ls32b );
  uint32_t data[8] = { 0x80000000, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU };
  UInt256 val = uint256_create( data );
  ASSERT_SAME( val, result ); 

  //double negation
  result = uint256_negate(val);
  ASSERT_SAME(objs->msb_of_ls32b, result);
}

void test_neg_overflow( TestObjs *objs ) {
  UInt256 result;

  uint32_t two_data[8] = { 2U };
  UInt256 two;
  INIT_FROM_ARR( two, two_data );

  result = uint256_sub( objs->one, objs->max );
  ASSERT_SAME( two, result );
}

void test_mul( TestObjs *objs ) {
  UInt256 left, right, result;

  // some very basic tests

  result = uint256_mul( objs->one, objs->one );
  ASSERT_SAME( objs->one, result );

  result = uint256_mul( objs->one, objs->zero );
  ASSERT_SAME( objs->zero, result );

  result = uint256_mul( objs->zero, objs->one );
  ASSERT_SAME( objs->zero, result );

  result = uint256_mul( objs->zero, objs->max );
  ASSERT_SAME( objs->zero, result );

  result = uint256_mul( objs->zero, objs->zero );
  ASSERT_SAME( objs->zero, result );

  result = uint256_mul( objs->max, objs->one );
  ASSERT_SAME( objs->max, result );

  UInt256 two = { { 2,0,0,0,0,0,0,0 } }, four = { { 4,0,0,0,0,0,0,0 } };
  result = uint256_mul( two, two );
  ASSERT_SAME( four, result );

  result = uint256_mul( objs->max, two );
  ASSERT_SAME( uint256_add(objs->max, objs->max), result );

  UInt256 twenty = { { 20,0,0,0,0,0,0,0 } }, forty = {{ 40,0,0,0,0,0,0,0 }};
  result = uint256_mul( twenty, two );
  ASSERT_SAME(forty, result);


  // a more complicated test

  // fc42c691d6284761fb49dd54f3a13eb * 4cfd2c7d8790c50c280ff0ff77617a8e = 4bdd4cc8b6067f7617c05917f828d17a26046ba5f436cb7df595f6c68c00a5a
  left.data[0] = 0x4f3a13ebU;
  left.data[1] = 0x1fb49dd5U;
  left.data[2] = 0x1d628476U;
  left.data[3] = 0xfc42c69U;
  left.data[4] = 0x0U;
  left.data[5] = 0x0U;
  left.data[6] = 0x0U;
  left.data[7] = 0x0U;
  right.data[0] = 0x77617a8eU;
  right.data[1] = 0x280ff0ffU;
  right.data[2] = 0x8790c50cU;
  right.data[3] = 0x4cfd2c7dU;
  right.data[4] = 0x0U;
  right.data[5] = 0x0U;
  right.data[6] = 0x0U;
  right.data[7] = 0x0U;
  result = uint256_mul(left, right);
  ASSERT(0x68c00a5aU == result.data[0]);
  ASSERT(0xdf595f6cU == result.data[1]);
  ASSERT(0x5f436cb7U == result.data[2]);
  ASSERT(0xa26046baU == result.data[3]);
  ASSERT(0x7f828d17U == result.data[4]);
  ASSERT(0x617c0591U == result.data[5]);
  ASSERT(0x8b6067f7U == result.data[6]);
  ASSERT(0x4bdd4ccU == result.data[7]);
}

void test_lshift( TestObjs *objs ) {
  UInt256 result;

  UInt256 two = { { 2,0,0,0,0,0,0,0 } }, four = { { 4,0,0,0,0,0,0,0 } };

  // some very basic tests

  result = uint256_lshift( objs->one, 0 );
  ASSERT_SAME( objs->one, result );

  result = uint256_lshift( objs->one, 1 );
  ASSERT_SAME( two, result );

  result = uint256_lshift( objs->one, 2 );
  ASSERT_SAME( four, result );

  // a more complicated test
  {
    // Test shifting 727767d07ccff5fe25cd125b4523e8c7db1b8d1a2c8a2830284d72bb872c33a5 left by 50 bit(s)
    uint32_t arr[8] = {0x872c33a5U, 0x284d72bbU, 0x2c8a2830U, 0xdb1b8d1aU, 0x4523e8c7U, 0x25cd125bU, 0x7ccff5feU, 0x727767d0U};
    UInt256 val;
    INIT_FROM_ARR( val, arr );
    uint32_t expected_arr[8] = {0x00000000U, 0xce940000U, 0xcaee1cb0U, 0xa0c0a135U, 0x3468b228U, 0xa31f6c6eU, 0x496d148fU, 0xd7f89734U};
    UInt256 expected;
    INIT_FROM_ARR( expected, expected_arr );
    result = uint256_lshift( val, 50U );
    ASSERT_SAME( expected, result );
  }
}

void test_lshift_2 ( TestObjs *objs ) {
  UInt256 result;

  //left shifting by 32
  UInt256 expectedResult = { { 0,0xFFFFFFFFU,0xFFFFFFFFU,0xFFFFFFFFU,0xFFFFFFFFU,0xFFFFFFFFU,0xFFFFFFFFU,0xFFFFFFFFU } };
  result = uint256_lshift( objs->max, 32 );
  ASSERT_SAME( expectedResult, result );

  //left shifting by over 32, but less than 256
  UInt256 expectedResult2 = { { 0,0,0,0xFFFFFFFFU,0xFFFFFFFFU,0xFFFFFFFFU,0xFFFFFFFFU,0xFFFFFFFFU } };
  result = uint256_lshift( objs->max, 96 );
  ASSERT_SAME( expectedResult2, result );

  UInt256 expectedResult3 = { { 0,0,0,0,0,0,0xFFFFFFFFU,0xFFFFFFFFU } };
  result = uint256_lshift( objs->max, 192 );
  ASSERT_SAME( expectedResult3, result );

  //left shifting 33
  UInt256 expectedResult4 = { { 0,2,0,0,0,0,0,0 } };
  result = uint256_lshift( objs->one, 33 );
  ASSERT_SAME( expectedResult4, result );
}
