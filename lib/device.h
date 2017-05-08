#ifndef SYNC_DEVICE_H
#define SYNC_DEVICE_H

#include "base.h"
#include "sync.h"
#include "client.h"

struct sync_device {
	char *base;
	struct sync_track **tracks;
	size_t num_tracks;

#ifndef SYNC_PLAYER
	int row;
	SOCKET sock;
#endif
	struct sync_io_cb io_cb;
};

#endif /* SYNC_DEVICE_H */
