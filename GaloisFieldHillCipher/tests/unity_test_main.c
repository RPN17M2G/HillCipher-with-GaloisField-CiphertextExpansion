#include "unity.h"
#include "Math/test_FieldBasicOperations.h"
#include "Math/test_MathUtils.h"

int main()
{
    UNITY_BEGIN();

    run_all_FieldBasicOperations_tests();
    run_all_MathUtils_tests();

    UNITY_END();

    return 0;
}
