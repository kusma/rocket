#ifndef TRACKGROUP_H
#define TRACKGROUP_H

#include <QObject>
#include <QList>
class SyncTrack;

class SyncPage : public QObject {
	Q_OBJECT

public:
	void addTrack(SyncTrack  *track)
	{
		tracks.push_back(track);
		emit trackAdded(track);
	}

	void removeTrack(int index)
	{
		emit trackRemoved(index);
		tracks.removeAt(index);
	}

signals:
	void trackAdded(SyncTrack  *track);
	void trackRemoved(int index);

private:
	QList<SyncTrack  *> tracks;
};

#endif // TRACKGROUP_H
