#ifndef TRACKVIEW_H
#define TRACKVIEW_H

#include <QAbstractScrollArea>
#include <QPaintEvent>
#include <QKeyEvent>
#include <QPainter>

#include "syncpage.h"

class QLineEdit;
class SyncPage;
class SyncTrack;

class TrackView : public QAbstractScrollArea
{
	Q_OBJECT
public:
	TrackView(SyncPage *page, QWidget *parent);

	void setRows(int rows);
	int getRows() const;

	SyncTrack *getTrack(int index);
	int getTrackCount() const;

	void editEnterValue();
	void editBiasValue(float amount);
	void editToggleInterpolationType();

	void setEditRow(int newEditRow, bool selecting = false);
	int  getEditRow() const { return editRow; }

	void setEditTrack(int newEditTrack, bool autoscroll = true, bool selecting = false);
	int  getEditTrack() const { return editTrack; }

	void selectNone()
	{
		setSelection(QRect(QPoint(editTrack, editRow),
		                   QPoint(editTrack, editRow)));
	}

	void dirtyCurrentValue()
	{
		emit currValDirty();
	}

	void dirtyPosition()
	{
		emit posChanged(editTrack, editRow);
	}

	void setReadOnly(bool readOnly)
	{
		this->readOnly = readOnly;
	}

signals:
	void posChanged(int col, int row);
	void currValDirty();

private slots:
	void onHScroll(int value);
	void onVScroll(int value);
	void onEditingFinished();
	void onTrackHeaderChanged(int trackIndex);
	void onTrackDataChanged(int trackIndex, int start, int stop);

public slots:
	void editUndo();
	void editRedo();
	void editCopy();
	void editCut();
	void editPaste();
	void editClear();

	void selectAll();
	void selectTrack();
	void selectRow();

private:

	/* paint helpers */
	void paintTopMargin(QPainter &painter, const QRegion &rcTracks);
	void paintLeftMargin(QPainter &painter, const QRegion &rcTracks);
	void paintTracks(QPainter &painter, const QRegion &rcTracks);
	void paintTrack(QPainter &painter, const QRegion &rcTracks, int track);

	void paintEvent(QPaintEvent *);
	void keyPressEvent(QKeyEvent *);
	void resizeEvent(QResizeEvent *);
	void mouseMoveEvent(QMouseEvent *);
	void mousePressEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent *);
	void changeEvent(QEvent *);

	void setupScrollBars();
	void setScrollPos(int newScrollPosX, int newScrollPosY);
	void scrollWindow(int newScrollPosX, int newScrollPosY);

	QRect getSelection() const
	{
		return QRect(selectionStart, selectionStart).united(QRect(selectionEnd, selectionEnd));
	}

	void setSelection(const QRect &rect);
	void updateSelection(const QPoint &pos, bool selecting);

	int getLogicalX(int track) const;
	int getLogicalY(int row) const;
	int getPhysicalX(int track) const;
	int getPhysicalY(int row) const;

	int getTrackFromLogicalX(int x) const;
	int getTrackFromPhysicalX(int x) const;

	SyncPage *page;

	QPoint selectionStart;
	QPoint selectionEnd;

	int rowHeight;
	int trackWidth;
	int topMarginHeight;
	int leftMarginWidth;
	void updateFont();

	QBrush bgBaseBrush, bgDarkBrush;
	QBrush selectBaseBrush, selectDarkBrush;
	QPen rowPen, rowSelectPen;
	QBrush editBrush, bookmarkBrush;
	QPen lerpPen, cosinePen, rampPen;
	QCursor handCursor;
	void updatePalette();

	/* cursor position */
	int editRow, editTrack;

	int scrollPosX,  scrollPosY;
	int windowRows;

	QLineEdit *lineEdit;

	bool readOnly;
	bool dragging;
	int anchorTrack;
};

#endif // !defined(TRACKVIEW_H)
