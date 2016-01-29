#ifndef SYNCPAGE_H
#define SYNCPAGE_H

#include <QObject>
#include <QString>
#include <QVector>
#include "synctrack.h"

class SyncDocument;

class SyncPage : public QObject {
	Q_OBJECT
public:
	SyncPage(SyncDocument *document, const QString &name);

	SyncTrack *getTrack(int index);
	const SyncTrack *getTrack(int index) const;

	int getTrackCount() const
	{
		return tracks.size();
	}

	void addTrack(SyncTrack *);

	void swapTrackOrder(int t1, int t2);

	SyncDocument *getDocument()
	{
		return document; // TODO: try to encapsulate without exposing
	}

	const QString &getName() { return name; }

public slots:
	// TODO: tighter invalidation on all of these!
	void onKeyFrameAdded(const SyncTrack &track, int)
	{
		invalidateTrack(track);
	}

	void onKeyFrameChanged(const SyncTrack &track, int, const SyncTrack::TrackKey &)
	{
		invalidateTrack(track);
	}

	void onKeyFrameRemoved(const SyncTrack &track, int)
	{
		invalidateTrack(track);
	}

private:
	void invalidateTrack(const SyncTrack &track);
	void invalidateTrackData(const SyncTrack &track, int start, int stop);

	SyncDocument *document;
	QString name;
	QVector<SyncTrack *> tracks;

signals:
	void trackHeaderChanged(int trackIndex);
	void trackDataChanged(int trackIndex, int start, int stop);
};

#endif // SYNCPAGE_H
