#include "syncpage.h"

#include <syncdocument.h>

SyncPage::SyncPage(SyncDocument *document, const QString &name) :
    QObject(document),
    document(document),
    name(name)
{
}

SyncTrack *SyncPage::getTrack(int index)
{
	Q_ASSERT(index >= 0 && index < tracks.size());
	return tracks[index];
}

const SyncTrack *SyncPage::getTrack(int index) const
{
	Q_ASSERT(index >= 0 && index < tracks.size());
	return tracks[index];
}

void SyncPage::addTrack(SyncTrack *track)
{
	tracks.push_back(track);
	QObject::connect(track, SIGNAL(keyFrameAdded(const SyncTrack &, int)),
	                 this,  SLOT(onKeyFrameAdded(const SyncTrack &, int)));
	QObject::connect(track, SIGNAL(keyFrameChanged(const SyncTrack &, int, const SyncTrack::TrackKey &)),
	                 this,  SLOT(onKeyFrameChanged(const SyncTrack &, int, const SyncTrack::TrackKey &)));
	QObject::connect(track, SIGNAL(keyFrameRemoved(const SyncTrack &, int)),
	                 this,  SLOT(onKeyFrameRemoved(const SyncTrack &, int)));
}

void SyncPage::swapTrackOrder(int t1, int t2)
{
	Q_ASSERT(0 <= t1 && t1 < tracks.size());
	Q_ASSERT(0 <= t2 && t2 < tracks.size());
	std::swap(tracks[t1], tracks[t2]);
	invalidateTrack(*tracks[t1]);
	invalidateTrack(*tracks[t2]);
}

void SyncPage::invalidateTrack(const SyncTrack &track)
{
	int trackIndex = tracks.indexOf((SyncTrack*)&track);
	Q_ASSERT(trackIndex >= 0);
	emit trackHeaderChanged(trackIndex);
	emit trackDataChanged(trackIndex, 0, document->getRows());
}

void SyncPage::invalidateTrackData(const SyncTrack &track, int start, int stop)
{
	int trackIndex = tracks.indexOf((SyncTrack*)&track);
	Q_ASSERT(trackIndex >= 0);
	emit trackDataChanged(trackIndex, start, stop);
}
