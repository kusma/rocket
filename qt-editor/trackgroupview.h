#ifndef TRACKGROUPVIEW_H
#define TRACKGROUPVIEW_H

#include <QWidget>

class TrackGroup;
class TrackView;
class QHBoxLayout;
class Track;

class TrackGroupView : public QWidget {
	Q_OBJECT

public:
	TrackGroupView(TrackGroup *trackGroup, QWidget *parent = 0);

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

	TrackGroup *trackGroup;
	QList<TrackView *> trackViews;
	int currRow, currCol;
	int rowCount;
};

#endif // TRACKGROUPVIEW_H
