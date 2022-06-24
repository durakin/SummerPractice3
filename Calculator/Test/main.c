#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "../calc.h"

void test_to_postfix_notation_SimpleSum() {
  char result_buffer[20];
  to_postfix_notation("1+2", result_buffer);
  CU_ASSERT(strcmp(result_buffer, "1 2 +") == 0);
}

void test_to_postfix_notation_SumWithTailingMult() {
  char result_buffer[20];
  to_postfix_notation("1+2*3", result_buffer);
  CU_ASSERT(strcmp(result_buffer, "1 2 3 *+"));
}

void test_to_postfix_notation_SumWithBraces() {
  char result_buffer[30];
  to_postfix_notation("1+2*3*(4-5)", result_buffer);
  CU_ASSERT(strcmp(result_buffer, "1 2 3 *4 5 -*+"));
}

void test_calculate_postfix_notation_SimpleSum() {
  CU_ASSERT(calculate_postfix_notation("1 2 +") == 3.0);
}

void test_calculate_postfix_notation_SumWithTailingMult() {
  CU_ASSERT(calculate_postfix_notation("1 2 3 *+") == 7.0);
}

void test_calculate_postfix_notation_SumWithBraces() {
  CU_ASSERT(calculate_postfix_notation("1 2 3 *4 5 -*+") == -5.0);
}

int init_suite() { return 0; }

int clean_suite() { return 0; }

int main() {
  CU_pSuite pSuite = NULL;

  if (CUE_SUCCESS != CU_initialize_registry()) {
    return CU_get_error();
  }

  pSuite = CU_add_suite("Suite", init_suite, clean_suite);

  if (NULL == pSuite) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if ((NULL == CU_add_test(pSuite, "Simple sum to postfix: ",
                           test_to_postfix_notation_SimpleSum)) ||
      NULL == CU_add_test(pSuite, "Sum with tailing multiplication to postfix: ",
                          test_to_postfix_notation_SumWithTailingMult) ||
      NULL == CU_add_test(pSuite, "Sum with multiplication and braces to postfix: ",
                          test_to_postfix_notation_SumWithBraces) ||
      NULL == CU_add_test(pSuite, "Calculate simple sum from postfix: ",
                          test_calculate_postfix_notation_SimpleSum) ||
      NULL == CU_add_test(pSuite, "Calculate sum with tailing multiplication from postfix: ",
                      test_calculate_postfix_notation_SumWithTailingMult) ||
      NULL == CU_add_test(pSuite, "Calculate sum with multiplication and braces from postfix: ",
                          test_calculate_postfix_notation_SumWithBraces)
      ) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();

  return CU_get_error();
}