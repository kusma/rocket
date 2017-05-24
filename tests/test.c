#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>

static int passed_tests, failed_tests;

int test_log(const char *fmt, ...)
{
	int ret;
	va_list va;
	va_start(va, fmt);
	ret = vfprintf(stderr, fmt, va);
	va_end(va);
	return ret;
}

void test_run(const char *name, int (*test_fn)(void))
{
	int ret = test_fn();

	if (!ret) {
		passed_tests++;
		test_log("ok: %s\n", name);
	} else {
		failed_tests++;
		test_log("not ok: %s\n", name);
	}
}

int test_fail(const char *file, int line, const char *fmt, ...)
{
	va_list va;
	fprintf(stderr, "%s:%d - ", file, line);
	va_start(va, fmt);
	vfprintf(stderr, fmt, va);
	va_end(va);
	return 1;
}

int test_report(void)
{
	test_log("PASSED: %d\n", passed_tests);
	test_log("FAILED: %d\n", failed_tests);
	return failed_tests != 0;
}
