#ifndef TRACKGROUP_H
#define TRACKGROUP_H

#include <QObject>
#include <QList>
class Track;

class TrackGroup : public QObject {
	Q_OBJECT

public:
	void addTrack(Track *track)
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
	void trackAdded(Track *track);
	void trackRemoved(int index);

private:
	QList<Track *> tracks;
};

#endif // TRACKGROUP_H
