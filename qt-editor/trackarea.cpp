#include "trackarea.h"
#include "trackgroup.h"
#include "trackgroupview.h"
#include "trackview.h"
#include "rownumberview.h"
#include "track.h"
#include "trackgroupnameview.h"

#include <QScrollBar>
#include <QKeyEvent>
#include <QApplication>

TrackArea::TrackArea(QWidget *parent) :
	QScrollArea(parent)
{
#ifdef Q_OS_WIN
	setFont(QFont("Fixedsys"));
#else
	QFont font("Monospace");
	font.setStyleHint(QFont::TypeWriter);
	setFont(font);
#endif

	// HACK: create a dummy-trackgroup
	TrackGroup *trackGroup = new TrackGroup;
	trackGroupView = new TrackGroupView(trackGroup, this);
	setWidget(trackGroupView);

	setAlignment(Qt::AlignCenter);

	rowNumberView = new RowNumberView(this);
	rowNumberView->setFont(this->font());
	trackGroupNameView = new TrackGroupNameView(trackGroup, this);

	setFrameShape(QFrame::NoFrame);
	setBackgroundRole(QPalette::Dark);
	setAutoFillBackground(true);

	// HACK: add some data!
	for (int i = 0; i < 10; ++i) {
		Track *track = new Track(QString("track %1").arg(i + 1));
		trackGroup->addTrack(track);
		for (int i = 0; i < 20; ++i) {
			Track::KeyFrame key;
			key.value = qrand() / (RAND_MAX * 0.5f);
			key.type = Track::KeyFrame::Step;
			track->setKeyFrame(qrand() % 128, key);
		}
	}
	trackGroupView->setCol(0);
	trackGroupView->setRow(0);

	trackGroupNameView->adjustSize();
	setViewportMargins(fontMetrics().width(' ') * 8, trackGroupNameView->height(), 0, 0);
}

void TrackArea::setRow(int row)
{
	row = qMin(qMax(row, 0), getRowCount() - 1);
	if (trackGroupView->getRow() != row) {
		trackGroupView->setRow(row);
		rowNumberView->setRowHilight(row);
	} else
		QApplication::beep();

	updateVScrollbar();
}

void TrackArea::setCol(int col)
{
	trackGroupView->setCol(col);
	updateHScrollbar();
}

int TrackArea::getRowCount() const
{
	return trackGroupView->getRowCount();
}

void TrackArea::setRowCount(int rows)
{
	// propagate row-count to both widgets
	rowNumberView->setRowCount(rows);
	trackGroupView->setRowCount(rows);
}

void TrackArea::updateHScrollbar()
{
	// make sure current track is visible
	const QRect trackRect = trackGroupView->getCurrentTrackRect();
	int x = trackRect.x() + trackRect.width() / 2;
	int y = verticalScrollBar()->value() + viewport()->height() / 2;
	ensureVisible(x, y, trackRect.width() / 2, 0);
}

void TrackArea::updateVScrollbar()
{
	// vertically center current row
	QFontMetrics fm(font());
	int y = trackGroupView->getRow() * fm.lineSpacing() + fm.lineSpacing() / 2;
	verticalScrollBar()->setValue(y - viewport()->height() / 2);
}

void TrackArea::keyPressEvent(QKeyEvent *event)
{
	switch (event->key()) {
	case Qt::Key_Up:
		setRow(trackGroupView->getRow() - 1);
		break;

	case Qt::Key_Down:
		setRow(trackGroupView->getRow() + 1);
		break;

	case Qt::Key_PageUp:
		setRow(trackGroupView->getRow() - 16);
		break;

	case Qt::Key_PageDown:
		setRow(trackGroupView->getRow() + 16);
		break;

	case Qt::Key_Left:
		setCol(trackGroupView->getCol() - 1);
		break;

	case Qt::Key_Right:
		setCol(trackGroupView->getCol() + 1);
		break;

	case Qt::Key_Home:
		if (event->modifiers() & Qt::ControlModifier)
			setCol(0);
		else
			setRow(0);
		break;

	case Qt::Key_End:
		if (event->modifiers() & Qt::ControlModifier)
			setCol(trackGroupView->getColCount() - 1);
		else
			setRow(trackGroupView->getRowCount() - 1);
		break;
	}
}

void TrackArea::resizeEvent(QResizeEvent *event)
{
	updateHScrollbar();
	updateVScrollbar();
	QScrollArea::resizeEvent(event);
	rowNumberView->move(0, trackGroupNameView->height() + widget()->y());
	rowNumberView->resize(viewport()->x(), widget()->height());
	trackGroupNameView->move(viewport()->x() + widget()->x(), 0);
}

void TrackArea::scrollContentsBy(int dx, int dy)
{
	// syncronize left margin
	if (dy)
		rowNumberView->move(rowNumberView->x(), rowNumberView->y() + dy);
	if (dx)
		trackGroupNameView->move(trackGroupNameView->x() + dx, trackGroupNameView->y());

	QScrollArea::scrollContentsBy(dx, dy);
}
