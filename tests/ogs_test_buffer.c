#include <ogs.h>

#include "ogs_test_support.h"

int main(void) {
    const char *suite_name = "ogs_test_buffer_smoke";
    ogs_test_start(suite_name);

    OgsContext *ctx = ogsInit();
    OGS_TEST_ASSERT(suite_name, ctx != NULL);

    OgsResult result = OGS_ERROR_INVALID_ARGUMENT;
    OgsBuffer *buffer = ogsCreateBuffer(ctx, 256, &result);
    OGS_TEST_ASSERT(suite_name, buffer != NULL);
    OGS_TEST_ASSERT(suite_name, result == OGS_SUCCESS);

    ogsDestroyBuffer(ctx, buffer);
    ogsShutdown(ctx);

    ogs_test_finish(suite_name);
    return 0;
}