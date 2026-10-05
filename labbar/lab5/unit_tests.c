#include <CUnit/Basic.h>
#include "utils.h"

int init_suite(void) {
    return 0;
}

int clean_suite(void) {
    return 0;
}

// 1. Skriv testfunktionen för is_number
void test_is_number(void) {
    // Positiva fall (ska returnera sant / icke-noll)
    CU_ASSERT_TRUE(is_number("42"));
    CU_ASSERT_TRUE(is_number("-10"));
    CU_ASSERT_TRUE(is_number("0"));

    // Negativa fall (ska returnera falskt / 0)
    CU_ASSERT_FALSE(is_number("foo"));
    CU_ASSERT_FALSE(is_number("123foo"));
    CU_ASSERT_FALSE(is_number(""));
    CU_ASSERT_FALSE(is_number("-"));
    CU_ASSERT_FALSE(is_number("--10"));
}

int main(void) {
    // Initiera testregistret
    if (CU_initialize_registry() != CUE_SUCCESS) {
        return CU_get_error();
    }

    // Skapa en testsvit
    CU_pSuite my_test_suite = CU_add_suite("is_number test suite", init_suite, clean_suite);
    if (my_test_suite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Registrera testfunktionen till sviten
    if (
        (CU_add_test(my_test_suite, "Testing is_number with valid and invalid inputs", test_is_number) == NULL)
    ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Kör testerna i verbose-läge (visar alla tester och assertions)
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    // Rensa upp efter CUnit
    CU_cleanup_registry();
    return CU_get_error();
}