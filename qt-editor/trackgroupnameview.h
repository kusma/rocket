#ifndef TRACKNAMEVIEW_H
#define TRACKNAMEVIEW_H

#include <QWidget>

class SyncPage;
class Track;
class QToolButton;

class SyncPageNameView : public QWidget {
	Q_OBJECT
public:
	explicit SyncPageNameView(SyncPage *syncPage, QWidget *parent = 0);

private slots:
	void trackAdded(Track *track);
	void trackRemoved(int index);

private:
	SyncPage *syncPage;
	QList<QWidget *> trackNameViews;
};

#endif // TRACKNAMEVIEW_H
