#include "test.h"
#include "../lib/track.h"
#include "../lib/sync.h"

int test_track_basics()
{
	struct sync_track t;
	t.name = "dummy";
	t.keys = NULL;
	t.num_keys = 0;

	struct track_key k;
	k.row = 10;
	k.value = 1337.0f;
	k.type = KEY_LINEAR;

	ASSERT_INT_EQUAL(sync_set_key(&t, &k), 0);
	ASSERT_INT_EQUAL(is_key_frame(&t, k.row), 1);
	ASSERT_INT_EQUAL(sync_del_key(&t, k.row), 0);
	ASSERT_INT_EQUAL(is_key_frame(&t, k.row), 0);

	return 0;
}

int test_sync_get_val_linear()
{
	int i;
	struct track_key keys[] = {
		{ 100, 10.0f, KEY_LINEAR },
		{ 110, 20.0f, KEY_LINEAR },
		{ 120, 25.0f, KEY_LINEAR }
	};

	struct sync_track t;
	t.name = "dummy";
	t.keys = NULL;
	t.num_keys = 0;

	for (i = 0; i < sizeof(keys) / sizeof(*keys); ++i)
		ASSERT_INT_EQUAL(sync_set_key(&t, &keys[i]), 0);

	ASSERT_FLOAT_EQUAL(sync_get_val(&t,   0.0f), 10.0f, 1e-5);
	ASSERT_FLOAT_EQUAL(sync_get_val(&t, 100.0f), 10.0f, 1e-5);
	ASSERT_FLOAT_EQUAL(sync_get_val(&t, 105.0f), 15.0f, 1e-5);
	ASSERT_FLOAT_EQUAL(sync_get_val(&t, 110.0f), 20.0f, 1e-5);
	ASSERT_FLOAT_EQUAL(sync_get_val(&t, 115.0f), 22.5f, 1e-5);
	ASSERT_FLOAT_EQUAL(sync_get_val(&t, 120.0f), 25.0f, 1e-5);
	ASSERT_FLOAT_EQUAL(sync_get_val(&t, 150.0f), 25.0f, 1e-5);

	return 0;
}

int main(void)
{
	TEST_RUN(test_track_basics);
	TEST_RUN(test_sync_get_val_linear);

	return test_report();
}
