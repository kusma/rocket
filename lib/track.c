#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "sync.h"
#include "track.h"
#include "base.h"

void sync_update_poly(struct sync_track *t)
{
	for (int i = 0; i < t->num_keys - 1; ++i) {
		struct track_key *k = t->keys + i;
		double mag = t->keys[i + 1].a - t->keys[i].a;

		/* set up curve-coefficients according to key-type */
		double b = 0.0, c = 0.0, d = 0.0;
		switch (k[0].type) {
		case KEY_STEP:
			break;

		case KEY_LINEAR:
			b = mag;
			break;

		case KEY_SMOOTH:
			c =  3 * mag;
			d = -2 * mag;
			break;

		case KEY_RAMP:
			c = mag;
			break;

		default:
			assert(0);
		}
		k->b = b;
		k->c = c;
		k->d = d;
	}
	t->keys[t->num_keys - 1].b = 0.0f;
	t->keys[t->num_keys - 1].c = 0.0f;
	t->keys[t->num_keys - 1].d = 0.0f;
}

double sync_get_val(const struct sync_track *t, double row)
{
	int idx, irow;
	struct track_key *k;
	double x;

	/* If we have no keys at all, return a constant 0 */
	if (!t->num_keys)
		return 0.0f;

	irow = (int)floor(row);
	idx = key_idx_floor(t, irow);

	/* at the edges, return the first/last value */
	if (idx < 0)
		return t->keys[0].a;
	if (idx > (int)t->num_keys - 2)
		return t->keys[t->num_keys - 1].a;

	k = t->keys + idx;
	x = (row - t->keys[idx].row) / (t->keys[idx + 1].row - t->keys[idx].row);
	return k->a + (k->b + (k->c + k->d * x) * x) * x;
}

int sync_find_key(const struct sync_track *t, int row)
{
	int lo = 0, hi = t->num_keys;

	/* binary search, t->keys is sorted by row */
	while (lo < hi) {
		int mi = (lo + hi) / 2;
		assert(mi != hi);

		if (t->keys[mi].row < row)
			lo = mi + 1;
		else if (t->keys[mi].row > row)
			hi = mi;
		else
			return mi; /* exact hit */
	}
	assert(lo == hi);

	/* return first key after row, negated and biased (to allow -0) */
	return -lo - 1;
}

#ifndef SYNC_PLAYER
int sync_set_key(struct sync_track *t, const struct track_key *k)
{
	int idx = sync_find_key(t, k->row);
	if (idx < 0) {
		/* no exact hit, we need to allocate a new key */
		void *tmp;
		idx = -idx - 1;
		tmp = realloc(t->keys, sizeof(struct track_key) *
		    (t->num_keys + 1));
		if (!tmp)
			return -1;
		t->num_keys++;
		t->keys = tmp;
		memmove(t->keys + idx + 1, t->keys + idx,
		    sizeof(struct track_key) * (t->num_keys - idx - 1));
	}
	t->keys[idx] = *k;
	sync_update_poly(t);
	return 0;
}

int sync_del_key(struct sync_track *t, int pos)
{
	void *tmp;
	int idx = sync_find_key(t, pos);
	assert(idx >= 0);
	memmove(t->keys + idx, t->keys + idx + 1,
	    sizeof(struct track_key) * (t->num_keys - idx - 1));
	assert(t->keys);
	tmp = realloc(t->keys, sizeof(struct track_key) *
	    (t->num_keys - 1));
	if (t->num_keys != 1 && !tmp)
		return -1;
	t->num_keys--;
	t->keys = tmp;
	return 0;
}
#endif
