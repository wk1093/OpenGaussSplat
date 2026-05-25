#include <ogs.h>

#include "ogs_test_support.h"

int main(void) {
    const char *suite_name = "ogs_test_error_strings";
    ogs_test_start(suite_name);

    OGS_TEST_ASSERT(suite_name, ogsGetErrorString(OGS_SUCCESS) != NULL);
    OGS_TEST_ASSERT(suite_name,
                    ogsGetErrorString(OGS_ERROR_INVALID_ARGUMENT) != NULL);
    OGS_TEST_ASSERT(suite_name,
                    ogsGetErrorString(OGS_ERROR_OUT_OF_MEMORY) != NULL);
    OGS_TEST_ASSERT(suite_name,
                    ogsGetErrorString(OGS_ERROR_VULKAN_INIT_FAILED) != NULL);
    OGS_TEST_ASSERT(suite_name,
                    ogsGetErrorString(OGS_ERROR_NO_SUITABLE_DEVICE) != NULL);

    ogs_test_finish(suite_name);
    return 0;
}