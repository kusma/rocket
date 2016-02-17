#ifndef TRACKGROUPVIEW_H
#define TRACKGROUPVIEW_H

#include <QWidget>

class SyncPage;
class TrackView;
class QHBoxLayout;
class Track;

class SyncPageView : public QWidget {
	Q_OBJECT

public:
	SyncPageView(SyncPage *syncPage, QWidget *parent = 0);

	int getRow() { return currRow; }
	void setRow(int row);
	int getCol() { return currCol; }
	void setCol(int col);

	void setRowCount(int rows);
	int getRowCount() const { return rowCount; }
	int getColCount() const { return trackViews.size(); }

	QRect getCurrentTrackRect() const;

private slots:
	void trackAdded(Track *track);
	void trackRemoved(int index);

protected:
	void changeEvent(QEvent *event);

	SyncPage *syncPage;
	QList<TrackView *> trackViews;
	int currRow, currCol;
	int rowCount;
};

#endif // TRACKGROUPVIEW_H
