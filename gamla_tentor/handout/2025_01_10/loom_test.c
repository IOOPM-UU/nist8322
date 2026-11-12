#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <CUnit/Basic.h>
#include "loom.h"

int init_suite(void)
{
  return 0;
}

int clean_suite(void)
{
  return 0;
}

void test_create_destroy(void)
{
  loom_t *loom = loom_create(10, 5);
  CU_ASSERT_PTR_NOT_NULL_FATAL(loom);
  loom_destroy(loom);
}

void test_bad_looms(void)
{
  loom_t *loom = loom_create(0, 0);
  CU_ASSERT_PTR_NULL(loom);

  loom = loom_create(1, 0);
  CU_ASSERT_PTR_NULL(loom);

  loom = loom_create(0, 1);
  CU_ASSERT_PTR_NULL(loom);
}

void test_add_single_instruction(void)
{
  loom_t *loom = loom_create(10, 5);
  CU_ASSERT_PTR_NOT_NULL_FATAL(loom);
  CU_ASSERT_TRUE(loom_add_instruction(loom, "abcde"));
  loom_destroy(loom);
}

void test_add_max_instructions(void)
{
  loom_t *loom = loom_create(10, 3);
  CU_ASSERT_PTR_NOT_NULL_FATAL(loom);
  CU_ASSERT_TRUE(loom_add_instruction(loom, "abc"));
  CU_ASSERT_TRUE(loom_add_instruction(loom, "def"));
  CU_ASSERT_TRUE(loom_add_instruction(loom, "ghi"));
  CU_ASSERT_TRUE(loom_add_instruction(loom, "jkl"));
  CU_ASSERT_TRUE(loom_add_instruction(loom, "mno"));
  CU_ASSERT_TRUE(loom_add_instruction(loom, "pqr"));
  CU_ASSERT_TRUE(loom_add_instruction(loom, "stu"));
  CU_ASSERT_TRUE(loom_add_instruction(loom, "vwx"));
  CU_ASSERT_TRUE(loom_add_instruction(loom, "yz "));
  CU_ASSERT_TRUE(loom_add_instruction(loom, "ABC"));
  loom_destroy(loom);
}

void test_add_too_many_instructions(void)
{
  loom_t *loom = loom_create(10, 3);
  CU_ASSERT_PTR_NOT_NULL_FATAL(loom);
  CU_ASSERT_TRUE(loom_add_instruction(loom, "abc"));
  CU_ASSERT_TRUE(loom_add_instruction(loom, "def"));
  CU_ASSERT_TRUE(loom_add_instruction(loom, "ghi"));
  CU_ASSERT_TRUE(loom_add_instruction(loom, "jkl"));
  CU_ASSERT_TRUE(loom_add_instruction(loom, "mno"));
  CU_ASSERT_TRUE(loom_add_instruction(loom, "pqr"));
  CU_ASSERT_TRUE(loom_add_instruction(loom, "stu"));
  CU_ASSERT_TRUE(loom_add_instruction(loom, "vwx"));
  CU_ASSERT_TRUE(loom_add_instruction(loom, "yz "));
  CU_ASSERT_TRUE(loom_add_instruction(loom, "ABC"));
  CU_ASSERT_FALSE(loom_add_instruction(loom, "no!"));
  loom_destroy(loom);
}

void test_add_multipliers(void)
{
  loom_t *loom = loom_create(5, 5);
  CU_ASSERT_PTR_NOT_NULL_FATAL(loom);
  CU_ASSERT_TRUE(loom_add_instruction(loom, "5x"));
  CU_ASSERT_TRUE(loom_add_instruction(loom, "2x3y"));
  CU_ASSERT_TRUE(loom_add_instruction(loom, "1x2y2z"));
  CU_ASSERT_TRUE(loom_add_instruction(loom, "3xyz"));
  loom_destroy(loom);
}

void test_add_null(void)
{
  loom_t *loom = loom_create(5, 10);
  CU_ASSERT_PTR_NOT_NULL_FATAL(loom);
  CU_ASSERT_TRUE(loom_add_instruction(loom, "Something!"));
  CU_ASSERT_FALSE(loom_add_instruction(loom, NULL));
  CU_ASSERT_TRUE(loom_add_instruction(loom, "MoreThings"));
  loom_destroy(loom);
}

void test_add_bad(void)
{
  loom_t *loom = loom_create(4, 4);
  CU_ASSERT_PTR_NOT_NULL_FATAL(loom);

  CU_ASSERT_FALSE(loom_add_instruction(loom, ""));
  CU_ASSERT_FALSE(loom_add_instruction(loom, "a"));
  CU_ASSERT_FALSE(loom_add_instruction(loom, "ab"));
  CU_ASSERT_FALSE(loom_add_instruction(loom, "abc"));
  CU_ASSERT_FALSE(loom_add_instruction(loom, "abcde"));
  CU_ASSERT_FALSE(loom_add_instruction(loom, "3x2y"));

  // Loom cannot have NULL instruction
  CU_ASSERT_FALSE(loom_add_instruction(loom, NULL));

  // Instructions cannot end with a digit
  CU_ASSERT_FALSE(loom_add_instruction(loom, "abc3"));

  // Digits cannot be followed by digits
  CU_ASSERT_FALSE(loom_add_instruction(loom, "a23b"));

  loom_destroy(loom);
}

void test_weave_simple(void)
{
  loom_t *loom = loom_create(5, 5);
  CU_ASSERT_PTR_NOT_NULL_FATAL(loom);

  CU_ASSERT_TRUE(loom_add_instruction(loom, "abcde"));

  char **tapestry = loom_weave(loom);
  CU_ASSERT_PTR_NOT_NULL_FATAL(tapestry);

  CU_ASSERT_STRING_EQUAL(tapestry[0], "abcde");
  CU_ASSERT_PTR_NULL(tapestry[1]);

  free(tapestry[0]);
  free(tapestry);

  loom_destroy(loom);
}

void test_weave_simple_change(void)
{
  loom_t *loom = loom_create(5, 5);
  CU_ASSERT_PTR_NOT_NULL_FATAL(loom);

  char instruction[6] = "abcde";

  CU_ASSERT_TRUE(loom_add_instruction(loom, instruction));

  instruction[0] = '\0';

  char **tapestry = loom_weave(loom);
  CU_ASSERT_PTR_NOT_NULL_FATAL(tapestry);

  CU_ASSERT_STRING_EQUAL(tapestry[0], "abcde");
  CU_ASSERT_PTR_NULL(tapestry[1]);

  free(tapestry[0]);
  free(tapestry);

  loom_destroy(loom);
}

void test_weave_many(void)
{
  loom_t *loom = loom_create(5, 5);
  CU_ASSERT_PTR_NOT_NULL_FATAL(loom);

  CU_ASSERT_TRUE(loom_add_instruction(loom, "  A  "));
  CU_ASSERT_TRUE(loom_add_instruction(loom, " XXX "));
  CU_ASSERT_TRUE(loom_add_instruction(loom, " XXX "));
  CU_ASSERT_TRUE(loom_add_instruction(loom, "VVVVV"));

  char **tapestry = loom_weave(loom);
  CU_ASSERT_PTR_NOT_NULL_FATAL(tapestry);

  CU_ASSERT_STRING_EQUAL(tapestry[0], "  A  ");
  CU_ASSERT_STRING_EQUAL(tapestry[1], " XXX ");
  CU_ASSERT_STRING_EQUAL(tapestry[2], " XXX ");
  CU_ASSERT_STRING_EQUAL(tapestry[3], "VVVVV");
  CU_ASSERT_PTR_NULL(tapestry[4]);

  free(tapestry[0]);
  free(tapestry[1]);
  free(tapestry[2]);
  free(tapestry[3]);
  free(tapestry);

  // Weave the same thing again:
  tapestry = loom_weave(loom);
  CU_ASSERT_PTR_NOT_NULL_FATAL(tapestry);

  CU_ASSERT_STRING_EQUAL(tapestry[0], "  A  ");
  CU_ASSERT_STRING_EQUAL(tapestry[1], " XXX ");
  CU_ASSERT_STRING_EQUAL(tapestry[2], " XXX ");
  CU_ASSERT_STRING_EQUAL(tapestry[3], "VVVVV");
  CU_ASSERT_PTR_NULL(tapestry[4]);

  free(tapestry[0]);
  free(tapestry[1]);
  free(tapestry[2]);
  free(tapestry[3]);
  free(tapestry);

  loom_destroy(loom);
}

void test_weave_single_multiplier(void)
{
  loom_t *loom = loom_create(5, 5);
  CU_ASSERT_PTR_NOT_NULL_FATAL(loom);

  CU_ASSERT_TRUE(loom_add_instruction(loom, "a3bc"));

  char **tapestry = loom_weave(loom);
  CU_ASSERT_PTR_NOT_NULL_FATAL(tapestry);

  CU_ASSERT_STRING_EQUAL(tapestry[0], "abbbc");
  CU_ASSERT_PTR_NULL(tapestry[1]);

  free(tapestry[0]);
  free(tapestry);

  loom_destroy(loom);
}

void test_weave_with_multipliers(void)
{
  loom_t *loom = loom_create(5, 5);
  CU_ASSERT_PTR_NOT_NULL_FATAL(loom);

  CU_ASSERT_TRUE(loom_add_instruction(loom, "2 A2 "));
  CU_ASSERT_TRUE(loom_add_instruction(loom, " 3X "));
  CU_ASSERT_TRUE(loom_add_instruction(loom, " 2XX "));
  CU_ASSERT_TRUE(loom_add_instruction(loom, "5V"));

  char **tapestry = loom_weave(loom);
  CU_ASSERT_PTR_NOT_NULL_FATAL(tapestry);

  CU_ASSERT_STRING_EQUAL(tapestry[0], "  A  ");
  CU_ASSERT_STRING_EQUAL(tapestry[1], " XXX ");
  CU_ASSERT_STRING_EQUAL(tapestry[2], " XXX ");
  CU_ASSERT_STRING_EQUAL(tapestry[3], "VVVVV");
  CU_ASSERT_PTR_NULL(tapestry[4]);

  free(tapestry[0]);
  free(tapestry[1]);
  free(tapestry[2]);
  free(tapestry[3]);
  free(tapestry);

  loom_destroy(loom);
}

void test_weave_large(void)
{
  loom_t *loom = loom_create(1000, 100);
  CU_ASSERT_PTR_NOT_NULL_FATAL(loom);

  bool result = true;
  for (int i = 0; i < 1000; ++i){
    result = result && loom_add_instruction(loom, "This string is really quite long. Funnily, it just happens to be exactly one hundred characters wide");
  }
  CU_ASSERT_TRUE(result);

  char **tapestry = loom_weave(loom);
  CU_ASSERT_PTR_NOT_NULL_FATAL(tapestry);

  result = true;
  for (int i = 0; i < 1000; ++i){
    result = result && strcmp(tapestry[i], "This string is really quite long. Funnily, it just happens to be exactly one hundred characters wide") == 0;
    free(tapestry[i]);
  }

  CU_ASSERT_TRUE(result);

  free(tapestry);

  loom_destroy(loom);
}

int main()
{
  CU_pSuite loom_test_suite = NULL;
  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();

  loom_test_suite = CU_add_suite("Loom Test Suite", init_suite, clean_suite);
  if (NULL == loom_test_suite)
    {
      CU_cleanup_registry();
      return CU_get_error();
    }

  if (
    (NULL == CU_add_test(loom_test_suite, "Test creating a loom", test_create_destroy)) ||
    (NULL == CU_add_test(loom_test_suite, "Test creating bad looms", test_bad_looms)) ||
    (NULL == CU_add_test(loom_test_suite, "Test adding a single instruction", test_add_single_instruction)) ||
    (NULL == CU_add_test(loom_test_suite, "Test filling up instructions", test_add_max_instructions)) ||
    (NULL == CU_add_test(loom_test_suite, "Test adding too many instructions", test_add_too_many_instructions)) ||
    (NULL == CU_add_test(loom_test_suite, "Test adding instructions with multipliers", test_add_multipliers)) ||
    (NULL == CU_add_test(loom_test_suite, "Test adding NULL instructions", test_add_null)) ||
    (NULL == CU_add_test(loom_test_suite, "Test adding bad instructions", test_add_bad)) ||
    (NULL == CU_add_test(loom_test_suite, "Test weaving a single row", test_weave_simple)) ||
    (NULL == CU_add_test(loom_test_suite, "Test that instructions are copied", test_weave_simple_change)) ||
    (NULL == CU_add_test(loom_test_suite, "Test weaving many rows", test_weave_many)) ||
    (NULL == CU_add_test(loom_test_suite, "Test weaving a single row with a multiplier", test_weave_single_multiplier)) ||
    (NULL == CU_add_test(loom_test_suite, "Test weaving with advanced instructions", test_weave_with_multipliers)) ||
    (NULL == CU_add_test(loom_test_suite, "Test weaving lots of large lines", test_weave_large)) ||
    false
  )
    {
      CU_cleanup_registry();
      return CU_get_error();
    }

  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();
  return CU_get_error();
}
