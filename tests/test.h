#include <math.h>

int test_run(const char *name, int (*test_fn)(void));
int test_fail(const char *file, int line, const char *fmt, ...);
int test_log(const char *fmt, ...);
int test_report(void);

#define TEST_RUN(test) test_run(#test, test)
#define TEST_FAIL(reason) return test_fail(__FILE__, __LINE__, "%s", reason)

#define ASSERT_INT_EQUAL(actual, expected) do { \
		if ((actual) != (expected)) \
			return test_fail(__FILE__, __LINE__, "expected: %d, got: %d\n", expected, actual); \
	} while (0)

#define ASSERT_FLOAT_EQUAL(actual, expected, error) do { \
		if (fabs((actual) - (expected)) >= error) \
			return test_fail(__FILE__, __LINE__, "expected: %f, got: %f (error: %f)\n", expected, actual, error); \
	} while (0)
