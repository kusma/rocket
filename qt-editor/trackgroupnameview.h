#ifndef TRACKNAMEVIEW_H
#define TRACKNAMEVIEW_H

#include <QWidget>

class TrackGroup;
class Track;
class QToolButton;

class TrackGroupNameView : public QWidget {
	Q_OBJECT
public:
	explicit TrackGroupNameView(TrackGroup *trackGroup, QWidget *parent = 0);

private slots:
	void trackAdded(Track *track);
	void trackRemoved(int index);

private:
	TrackGroup *trackGroup;
	QList<QWidget *> trackNameViews;
};

#endif // TRACKNAMEVIEW_H
