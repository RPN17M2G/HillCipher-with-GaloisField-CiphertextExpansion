#include "unity.h"
#include "Cipher/test_CipherUtils.h"
#include "Math/test_FieldBasicOperations.h"
#include "Math/test_MathUtils.h"

void setUp() {}
void tearDown() {}

int main()
{
    UNITY_BEGIN();

    //run_all_FieldBasicOperations_tests();
    run_all_MathUtils_tests();
    //run_all_CipherUtils_tests();

    return UNITY_END();
}
