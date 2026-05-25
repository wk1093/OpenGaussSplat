#include "ogs_test_support.h"

#include <stdio.h>

void ogs_test_start(const char *suite_name) {
    printf("[ RUN      ] %s\n", suite_name);
}

void ogs_test_finish(const char *suite_name) {
    printf("[      OK ] %s\n", suite_name);
}

int ogs_test_fail(const char *suite_name, const char *file, int line,
                  const char *expression) {
    fprintf(stderr, "[  FAILED  ] %s @ %s:%d (%s)\n", suite_name, file, line,
            expression);
    return 1;
}