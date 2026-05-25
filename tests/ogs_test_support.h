#ifndef OGS_TEST_SUPPORT_H
#define OGS_TEST_SUPPORT_H

void ogs_test_start(const char *suite_name);
void ogs_test_finish(const char *suite_name);
int ogs_test_fail(const char *suite_name, const char *file, int line,
                  const char *expression);

#define OGS_TEST_ASSERT(suite_name, expression)                               \
    do {                                                                      \
        if (!(expression)) {                                                  \
            return ogs_test_fail((suite_name), __FILE__, __LINE__,            \
                                 #expression);                                \
        }                                                                     \
    } while (0)

#endif /* OGS_TEST_SUPPORT_H */