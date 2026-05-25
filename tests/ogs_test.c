#include "ogs_test_support.h"
#include <ogs.h>
#include <stdio.h>

int main(void) {
    const char *suite_name = "ogs_test_context_smoke";
    ogs_test_start(suite_name);

    OgsContext *ctx = ogsInit();
    OGS_TEST_ASSERT(suite_name, ctx != NULL);

    ogsShutdown(ctx);
    ogs_test_finish(suite_name);
    return 0;
}
