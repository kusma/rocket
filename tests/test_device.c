#include "test.h"
#include "../lib/device.h"
#include "../lib/sync.h"

static int test_device_basics(void)
{
	struct sync_device *d = sync_create_device("foo");

	ASSERT_IS_TRUE(d != NULL);

	sync_destroy_device(d);

	return 0;
}

int main(void)
{
	TEST_RUN(test_device_basics);

	return test_report();
}
