#include <stdio.h>
#include <stdlib.h>
#include "fixedpoint.h"
#include "tctest.h"

// Test fixture object, has some useful values for testing
typedef struct
{
  Fixedpoint zero;
  Fixedpoint one;
  Fixedpoint one_half;
  Fixedpoint one_fourth;
  Fixedpoint large1;
  Fixedpoint large2;
  Fixedpoint min_magnitude;
  Fixedpoint max;
  Fixedpoint min;

  // TODO: add more objects to the test fixture
} TestObjs;

// functions to create and destroy the test fixture
TestObjs *setup(void);
void cleanup(TestObjs *objs);

// test functions
void test_whole_part(TestObjs *objs);
void test_frac_part(TestObjs *objs);
void test_create_from_hex(TestObjs *objs);
void test_format_as_hex(TestObjs *objs);
void test_negate(TestObjs *objs);
void test_add(TestObjs *objs);
void test_sub(TestObjs *objs);

void test_double(TestObjs *objs);
void test_halve(TestObjs *objs);

void test_is_overflow_pos(TestObjs *objs);

void test_is_overflow_neg(TestObjs *objs);
void test_is_underflow_pos(TestObjs *objs);
void test_is_underflow_neg(TestObjs *objs);

void test_is_err(TestObjs *objs);

void test_fixedpoint_create();
void test_fixedpiont_create2();
void test_is_zero(TestObjs *objs);
void test_compare(TestObjs *objs);
// TODO: add more test functions

int main(int argc, char **argv)
{

  // if a testname was specified on the command line, only that
  // test function will be executed
  if (argc > 1)
  {
    tctest_testname_to_execute = argv[1];
  }

  TEST_INIT();

  TEST(test_whole_part);
  TEST(test_frac_part);
  TEST(test_create_from_hex);
  TEST(test_format_as_hex);
  TEST(test_negate);
  TEST(test_add);
  TEST(test_sub);

  TEST(test_double);
  TEST(test_halve);

  TEST(test_is_overflow_pos);

  TEST(test_is_overflow_neg);
  TEST(test_is_underflow_pos);
  TEST(test_is_underflow_neg);

  TEST(test_is_err);

  TEST(test_fixedpoint_create);
  TEST(test_fixedpiont_create2);
  TEST(test_is_zero);
  TEST(test_compare);

  // IMPORTANT: if you add additional test functions (which you should!),
  // make sure they are included here.  E.g., if you add a test function
  // "my_awesome_tests", you should add
  //
  //   TEST(my_awesome_tests);
  //
  // here. This ensures that your test function will actually be executed.

  TEST_FINI();
}

TestObjs *setup(void)
{
  TestObjs *objs = malloc(sizeof(TestObjs));

  objs->zero = fixedpoint_create(0UL);
  objs->one = fixedpoint_create(1UL);
  objs->one_half = fixedpoint_create2(0UL, 0x8000000000000000UL);
  objs->one_fourth = fixedpoint_create2(0UL, 0x4000000000000000UL);
  objs->large1 = fixedpoint_create2(0x4b19efceaUL, 0xec9a1e2418UL);
  objs->large2 = fixedpoint_create2(0xfcbf3d5UL, 0x4d1a23c24fafUL);
  objs->min_magnitude = fixedpoint_create2(0UL, 0x0000000000000001UL);
  objs->max = fixedpoint_create2(0xffffffffffffffffUL, 0xffffffffffffffffUL);
  objs->min = fixedpoint_create2(0x0000000000000000UL, 0x0000000000000001UL);

  return objs;
}

void cleanup(TestObjs *objs)
{
  free(objs);
}

void test_whole_part(TestObjs *objs)
{
  ASSERT(0UL == fixedpoint_whole_part(objs->zero));
  ASSERT(1UL == fixedpoint_whole_part(objs->one));
  ASSERT(0UL == fixedpoint_whole_part(objs->one_half));
  ASSERT(0UL == fixedpoint_whole_part(objs->one_fourth));
  ASSERT(0x4b19efceaUL == fixedpoint_whole_part(objs->large1));
  ASSERT(0xfcbf3d5UL == fixedpoint_whole_part(objs->large2));
}

void test_frac_part(TestObjs *objs)
{
  ASSERT(0UL == fixedpoint_frac_part(objs->zero));
  ASSERT(0UL == fixedpoint_frac_part(objs->one));
  ASSERT(0x8000000000000000UL == fixedpoint_frac_part(objs->one_half));
  ASSERT(0x4000000000000000UL == fixedpoint_frac_part(objs->one_fourth));
  ASSERT(0xec9a1e2418UL == fixedpoint_frac_part(objs->large1));
  ASSERT(0x4d1a23c24fafUL == fixedpoint_frac_part(objs->large2));
}

void test_create_from_hex(TestObjs *objs)
{
  (void)objs;

  Fixedpoint val1 = fixedpoint_create_from_hex("f6a5865.00f2");

  ASSERT(fixedpoint_is_valid(val1));

  ASSERT(0xf6a5865UL == fixedpoint_whole_part(val1));

  ASSERT(0x00f2000000000000UL == fixedpoint_frac_part(val1));

  val1 = fixedpoint_create_from_hex("-7fbd739120cafdc3.aa9b4253817c5dc5");
  ASSERT(fixedpoint_is_valid(val1));
  ASSERT(fixedpoint_is_neg(val1));
  ASSERT(0x7fbd739120cafdc3UL == fixedpoint_whole_part(val1));
  ASSERT(0xaa9b4253817c5dc5UL == fixedpoint_frac_part(val1));

  val1 = fixedpoint_create_from_hex("-b2e415bd0c.c42b457426");
  ASSERT(fixedpoint_is_valid(val1));
  ASSERT(fixedpoint_is_neg(val1));
  ASSERT(0xb2e415bd0cUL == fixedpoint_whole_part(val1));
  ASSERT(0xc42b457426000000UL == fixedpoint_frac_part(val1));

  val1 = fixedpoint_create_from_hex("545084.9f5ad0");
  ASSERT(fixedpoint_is_valid(val1));
  ASSERT(0x545084UL == fixedpoint_whole_part(val1));
  ASSERT(0x9f5ad00000000000UL == fixedpoint_frac_part(val1));
}

void test_format_as_hex(TestObjs *objs)
{
  char *s;

  s = fixedpoint_format_as_hex(objs->zero);
  ASSERT(0 == strcmp(s, "0"));
  free(s);

  s = fixedpoint_format_as_hex(objs->one);
  ASSERT(0 == strcmp(s, "1"));
  free(s);

  s = fixedpoint_format_as_hex(objs->one_half);
  ASSERT(0 == strcmp(s, "0.8"));
  free(s);

  s = fixedpoint_format_as_hex(objs->one_fourth);
  ASSERT(0 == strcmp(s, "0.4"));
  free(s);

  s = fixedpoint_format_as_hex(objs->large1);
  ASSERT(0 == strcmp(s, "4b19efcea.000000ec9a1e2418"));
  free(s);

  s = fixedpoint_format_as_hex(objs->large2);
  ASSERT(0 == strcmp(s, "fcbf3d5.00004d1a23c24faf"));
  free(s);
}

void test_negate(TestObjs *objs)
{
  // none of the test fixture objects are negative
  ASSERT(!fixedpoint_is_neg(objs->zero));
  ASSERT(!fixedpoint_is_neg(objs->one));
  ASSERT(!fixedpoint_is_neg(objs->one_half));
  ASSERT(!fixedpoint_is_neg(objs->one_fourth));
  ASSERT(!fixedpoint_is_neg(objs->large1));
  ASSERT(!fixedpoint_is_neg(objs->large2));

  // negate the test fixture values
  Fixedpoint zero_neg = fixedpoint_negate(objs->zero);
  Fixedpoint one_neg = fixedpoint_negate(objs->one);
  Fixedpoint one_half_neg = fixedpoint_negate(objs->one_half);
  Fixedpoint one_fourth_neg = fixedpoint_negate(objs->one_fourth);
  Fixedpoint large1_neg = fixedpoint_negate(objs->large1);
  Fixedpoint large2_neg = fixedpoint_negate(objs->large2);

  // zero does not become negative when negated
  ASSERT(!fixedpoint_is_neg(zero_neg));

  // all of the other values should have become negative when negated
  ASSERT(fixedpoint_is_neg(one_neg));
  ASSERT(fixedpoint_is_neg(one_half_neg));
  ASSERT(fixedpoint_is_neg(one_fourth_neg));
  ASSERT(fixedpoint_is_neg(large1_neg));
  ASSERT(fixedpoint_is_neg(large2_neg));

  // magnitudes should stay the same
  ASSERT(0UL == fixedpoint_whole_part(objs->zero));
  ASSERT(1UL == fixedpoint_whole_part(objs->one));
  ASSERT(0UL == fixedpoint_whole_part(objs->one_half));
  ASSERT(0UL == fixedpoint_whole_part(objs->one_fourth));
  ASSERT(0x4b19efceaUL == fixedpoint_whole_part(objs->large1));
  ASSERT(0xfcbf3d5UL == fixedpoint_whole_part(objs->large2));
  ASSERT(0UL == fixedpoint_frac_part(objs->zero));
  ASSERT(0UL == fixedpoint_frac_part(objs->one));
  ASSERT(0x8000000000000000UL == fixedpoint_frac_part(objs->one_half));
  ASSERT(0x4000000000000000UL == fixedpoint_frac_part(objs->one_fourth));
  ASSERT(0xec9a1e2418UL == fixedpoint_frac_part(objs->large1));
  ASSERT(0x4d1a23c24fafUL == fixedpoint_frac_part(objs->large2));
}

void test_add(TestObjs *objs)
{
  (void)objs;

  Fixedpoint lhs, rhs, sum;

  lhs = fixedpoint_create_from_hex("-c7252a193ae07.7a51de9ea0538c5");
  rhs = fixedpoint_create_from_hex("d09079.1e6d601");
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(fixedpoint_is_neg(sum));
  ASSERT(0xc7252a0c31d8eUL == fixedpoint_whole_part(sum));
  ASSERT(0x5be47e8ea0538c50UL == fixedpoint_frac_part(sum));

  lhs = fixedpoint_create_from_hex("466bfa42.51a43");
  rhs = fixedpoint_create_from_hex("-1654413951649.08cae0213b7");
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(fixedpoint_is_neg(sum));
  ASSERT(0x16543cd291c06UL == fixedpoint_whole_part(sum));
  ASSERT(0xb726b0213b700000UL == fixedpoint_frac_part(sum));

  lhs = fixedpoint_create_from_hex("5842f839c.5e");
  rhs = fixedpoint_create_from_hex("d.74acb60845375a");
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(0x5842f83a9UL == fixedpoint_whole_part(sum));
  ASSERT(0xd2acb60845375a00UL == fixedpoint_frac_part(sum));

  lhs = fixedpoint_create_from_hex("-43dc8c.0c");
  rhs = fixedpoint_create_from_hex("-76f5dc9211c18.950b1da227a76e1");
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(fixedpoint_is_neg(sum));
  ASSERT(0x76f5dc964f8a4UL == fixedpoint_whole_part(sum));
  ASSERT(0xa10b1da227a76e10UL == fixedpoint_frac_part(sum));

  lhs = fixedpoint_create_from_hex("-5af045790ab3b7.a0f622e");
  rhs = fixedpoint_create_from_hex("f1d289ad5.1470bb52406");
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(fixedpoint_is_neg(sum));
  ASSERT(0x5af0365be218e2UL == fixedpoint_whole_part(sum));
  ASSERT(0x8c85678dbfa00000UL == fixedpoint_frac_part(sum));

  lhs = objs->zero;
  rhs = objs->zero;
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(0UL == fixedpoint_whole_part(sum));
  ASSERT(0UL == fixedpoint_frac_part(sum));

  lhs = objs->max;
  rhs = objs->one;
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(fixedpoint_is_overflow_pos(sum));

  lhs = fixedpoint_negate(objs->max);
  rhs = fixedpoint_negate(objs->one);
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(fixedpoint_is_overflow_neg(sum));

  lhs = objs->one;
  rhs = objs->zero;
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(1UL == fixedpoint_whole_part(sum));

  lhs = objs->zero;
  rhs = objs->one;
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(1UL == fixedpoint_whole_part(sum));

  lhs = objs->one_fourth;
  rhs = objs->one_fourth;
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(0UL == fixedpoint_whole_part(sum));
  ASSERT(0x8000000000000000UL == fixedpoint_frac_part(sum));

  lhs = objs->one_fourth;
  rhs = fixedpoint_negate(objs->one_fourth);
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(0UL == fixedpoint_whole_part(sum));
  ASSERT(0UL == fixedpoint_frac_part(sum));

  lhs = objs->one_half;
  rhs = objs->one_half;
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(1UL == fixedpoint_whole_part(sum));
  ASSERT(0UL == fixedpoint_frac_part(sum));

  lhs = objs->one;
  rhs = objs->one_half;
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(1UL == fixedpoint_whole_part(sum));
  ASSERT(0x8000000000000000UL == fixedpoint_frac_part(sum));
}

void test_sub(TestObjs *objs)
{
  (void)objs;

  Fixedpoint lhs, rhs, diff;

  lhs = fixedpoint_create_from_hex("-ccf35aa3a04a3b.b105");
  rhs = fixedpoint_create_from_hex("f676e8.58");
  diff = fixedpoint_sub(lhs, rhs);
  ASSERT(fixedpoint_is_neg(diff));
  ASSERT(0xccf35aa496c124UL == fixedpoint_whole_part(diff));
  ASSERT(0x0905000000000000UL == fixedpoint_frac_part(diff));

  lhs = fixedpoint_create_from_hex("5.3");
  rhs = fixedpoint_create_from_hex("3.8");
  diff = fixedpoint_sub(lhs, rhs);
  ASSERT(0x1UL == fixedpoint_whole_part(diff));
  printf("%lx\n", fixedpoint_frac_part(diff));
  ASSERT(0xb000000000000000UL == fixedpoint_frac_part(diff));

  lhs = fixedpoint_create_from_hex("-ffffffffffffffff");
  rhs = fixedpoint_create_from_hex("f");
  diff = fixedpoint_sub(lhs, rhs);
  ASSERT(fixedpoint_is_overflow_neg(diff));

  lhs = fixedpoint_create_from_hex("ffffffffffffffff");
  rhs = fixedpoint_create_from_hex("-f");
  diff = fixedpoint_sub(lhs, rhs);
  ASSERT(fixedpoint_is_overflow_pos(diff));
}

void test_double(TestObjs *objs)
{
  Fixedpoint doubled;

  doubled = fixedpoint_double(objs->one_half);
  ASSERT(1UL == fixedpoint_whole_part(doubled));

  doubled = fixedpoint_double(objs->one);
  ASSERT(2UL == fixedpoint_whole_part(doubled));
}

void test_halve(TestObjs *objs)
{
  Fixedpoint half;

  half = fixedpoint_halve(objs->one);
  ASSERT(0UL == fixedpoint_whole_part(half));
  ASSERT(0x8000000000000000UL == fixedpoint_frac_part(half));
}

void test_is_overflow_pos(TestObjs *objs)
{
  Fixedpoint sum;

  sum = fixedpoint_add(objs->max, objs->one);
  ASSERT(fixedpoint_is_overflow_pos(sum));

  sum = fixedpoint_add(objs->one, objs->max);
  ASSERT(fixedpoint_is_overflow_pos(sum));

  Fixedpoint negative_one = fixedpoint_negate(objs->one);

  sum = fixedpoint_sub(objs->max, negative_one);
  ASSERT(fixedpoint_is_overflow_pos(sum));
}

void test_is_overflow_neg(TestObjs *objs)
{
  Fixedpoint sum;

  sum = fixedpoint_add(fixedpoint_negate(objs->max), fixedpoint_negate(objs->one));
  ASSERT(fixedpoint_is_overflow_neg(sum));

  sum = fixedpoint_add(fixedpoint_negate(objs->one), fixedpoint_negate(objs->max));
  ASSERT(fixedpoint_is_overflow_neg(sum));

  sum = fixedpoint_sub(fixedpoint_negate(objs->max), objs->one);
  ASSERT(fixedpoint_is_overflow_neg(sum));
}

void test_is_underflow_pos(TestObjs *objs)
{
  Fixedpoint half;

  half = fixedpoint_halve(objs->min_magnitude);
  ASSERT(fixedpoint_is_underflow_pos(half));
}

void test_is_underflow_neg(TestObjs *objs)
{
  Fixedpoint half;

  half = fixedpoint_halve(fixedpoint_negate(objs->min_magnitude));
  ASSERT(fixedpoint_is_underflow_neg(half));
}

void test_is_err(TestObjs *objs)
{
  (void)objs;

  // too many characters
  Fixedpoint err1 = fixedpoint_create_from_hex("88888888888888889.6666666666666666");
  ASSERT(fixedpoint_is_err(err1));

  // too many characters
  Fixedpoint err2 = fixedpoint_create_from_hex("6666666666666666.88888888888888889");
  ASSERT(fixedpoint_is_err(err2));

  // this one is actually fine
  Fixedpoint err3 = fixedpoint_create_from_hex("-6666666666666666.8888888888888888");
  ASSERT(fixedpoint_is_valid(err3));
  ASSERT(!fixedpoint_is_err(err3));

  // whole part is too large
  Fixedpoint err4 = fixedpoint_create_from_hex("88888888888888889");
  ASSERT(fixedpoint_is_err(err4));

  // fractional part is too large
  Fixedpoint err5 = fixedpoint_create_from_hex("7.88888888888888889");
  ASSERT(fixedpoint_is_err(err5));

  // invalid hex digits in whole part
  Fixedpoint err6 = fixedpoint_create_from_hex("123xabc.4");
  ASSERT(fixedpoint_is_err(err6));

  // invalid hex digits in fractional part
  Fixedpoint err7 = fixedpoint_create_from_hex("7.0?4");
  ASSERT(fixedpoint_is_err(err7));
}

//--fixedpoint_create
void test_fixedpoint_create()
{
  //create a zero
  Fixedpoint fixedZero = fixedpoint_create(0UL);
  ASSERT(0UL == fixedZero.whole);
  //create a regular number
  Fixedpoint fixedOne = fixedpoint_create(1UL);
  ASSERT(1UL == fixedOne.whole);
  //create large number
  Fixedpoint fixedLarge = fixedpoint_create(0x4b19efceaUL);
  ASSERT(0x4b19efceaUL == fixedLarge.whole);
}

//--fixedpoint_create2
void test_fixedpiont_create2()
{
  //create a zero
  Fixedpoint fixedZero = fixedpoint_create2(0UL, 0UL);
  ASSERT(0UL == fixedZero.whole);
  ASSERT(0UL == fixedZero.frac);
  //create a number w/ frac
  Fixedpoint fixedNum = fixedpoint_create2(1UL, 0x4000000000000000UL);
  ASSERT(1UL == fixedNum.whole);
  ASSERT(0x4000000000000000UL == fixedNum.frac);
  //create a large number
  Fixedpoint fixedLarge = fixedpoint_create2(0x4b19efceaUL, 0xec9a1e2418UL);
  ASSERT(0x4b19efceaUL == fixedLarge.whole);
  ASSERT(0xec9a1e2418UL == fixedLarge.frac);
}

//--fixedpoint_whole_part
//get the whole part of reg number
//get the whole part of 0
//get the whole part of whole = 0 and a frac
//get the whole part of a fixedpoint w/ a frac = 0

//--fixedpoint_frac_part
//get the frac part of reg number
//get the frac part of 0
//get the frac part of frac = 0 and a whole
//get the frac part of a fixedpoint w/ a whole = 0

//--fixedpoint_is_zero
void test_is_zero(TestObjs *objs)
{
  //check actual zero
  ASSERT(1 == fixedpoint_is_zero(objs->zero));
  //check non zero
  ASSERT(0 == fixedpoint_is_zero(objs->one));
  //check whole = 0 and frac != 0
  ASSERT(0 == fixedpoint_is_zero(objs->one_half));
}

void test_compare(TestObjs *objs)
{
  ASSERT(0 == fixedpoint_compare(objs->one, objs->one));
  ASSERT(0 == fixedpoint_compare(objs->zero, objs->zero));

  Fixedpoint left, right;

  left = fixedpoint_create_from_hex("-5af045790ab3b7.a0f622e");
  right = fixedpoint_create_from_hex("f1d289ad5.1470bb52406");
  ASSERT(-1 == fixedpoint_compare(left, right));
  ASSERT(1 == fixedpoint_compare(right, left));

  left = fixedpoint_create_from_hex("5842f839c.5e");
  right = fixedpoint_create_from_hex("76f5dc9211c18.5e");
  ASSERT(-1 == fixedpoint_compare(left, right));
  ASSERT(1 == fixedpoint_compare(right, left));

  left = fixedpoint_create_from_hex("-5af045790ab3b7.a");
  right = fixedpoint_create_from_hex("-f1d289ad5.a");
  ASSERT(-1 == fixedpoint_compare(left, right));
  ASSERT(1 == fixedpoint_compare(right, left));

  left = fixedpoint_create_from_hex("5842f839c.5e123");
  right = fixedpoint_create_from_hex("5842f839c.0000005e");
  ASSERT(1 == fixedpoint_compare(left, right));
  ASSERT(-1 == fixedpoint_compare(right, left));

  left = fixedpoint_create_from_hex("-5842f839c.5e123");
  right = fixedpoint_create_from_hex("-5842f839c.00005e123");
  ASSERT(-1 == fixedpoint_compare(left, right));
  ASSERT(1 == fixedpoint_compare(right, left));

  left = fixedpoint_create_from_hex("5842f839c.5e123");
  right = fixedpoint_create_from_hex("5842f839c.5e123");
  ASSERT(0 == fixedpoint_compare(left, right));

  left = fixedpoint_create_from_hex("-5842f839c.5e123");
  right = fixedpoint_create_from_hex("-5842f839c.5e123");
  ASSERT(0 == fixedpoint_compare(left, right));
}
// TODO: implement more test functions
