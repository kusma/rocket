#include "trackarea.h"
#include "syncpage.h"
#include "syncpageview.h"
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
	SyncPage *syncPage = new SyncPage;
	syncPageView = new SyncPageView(syncPage, this);
	setWidget(syncPageView);

	setAlignment(Qt::AlignCenter);

	syncPageNameView = new SyncPageNameView(syncPage, this);
	rowNumberView = new RowNumberView(this);
	rowNumberView->setFont(this->font());

	setFrameShape(QFrame::NoFrame);
	setBackgroundRole(QPalette::Dark);
	setAutoFillBackground(true);

	// HACK: add some data!
	for (int i = 0; i < 10; ++i) {
		QString name = QString("track %1").arg(i + 1);
		SyncTrack  *track = new SyncTrack(name, name);
		syncPage->addTrack(track);
		for (int i = 0; i < 20; ++i) {
			SyncTrack::TrackKey key;
			key.row = qrand() % 128;
			key.value = qrand() / (RAND_MAX * 0.5f);
			key.type = SyncTrack::TrackKey::STEP;
			track->setKey(key);
		}
	}
	syncPageView->setCol(0);
	syncPageView->setRow(0);

	syncPageNameView->adjustSize();
	setViewportMargins(fontMetrics().width(' ') * 8, syncPageNameView->height(), 0, 0);
}

void TrackArea::setRow(int row)
{
	row = qMin(qMax(row, 0), getRowCount() - 1);
	if (syncPageView->getRow() != row) {
		syncPageView->setRow(row);
		rowNumberView->setRowHilight(row);
	} else
		QApplication::beep();

	updateVScrollbar();
}

void TrackArea::setCol(int col)
{
	syncPageView->setCol(col);
	updateHScrollbar();
}

int TrackArea::getRowCount() const
{
	return syncPageView->getRowCount();
}

void TrackArea::setRowCount(int rows)
{
	// propagate row-count to both widgets
	rowNumberView->setRowCount(rows);
	syncPageView->setRowCount(rows);
}

void TrackArea::updateHScrollbar()
{
	// make sure current track is visible
	const QRect trackRect = syncPageView->getCurrentTrackRect();
	int x = trackRect.x() + trackRect.width() / 2;
	int y = verticalScrollBar()->value() + viewport()->height() / 2;
	ensureVisible(x, y, trackRect.width() / 2, 0);
}

void TrackArea::updateVScrollbar()
{
	// vertically center current row
	QFontMetrics fm(font());
	int y = syncPageView->getRow() * fm.lineSpacing() + fm.lineSpacing() / 2;
	verticalScrollBar()->setValue(y - viewport()->height() / 2);
}

void TrackArea::keyPressEvent(QKeyEvent *event)
{
	switch (event->key()) {
	case Qt::Key_Up:
		setRow(syncPageView->getRow() - 1);
		break;

	case Qt::Key_Down:
		setRow(syncPageView->getRow() + 1);
		break;

	case Qt::Key_PageUp:
		setRow(syncPageView->getRow() - 16);
		break;

	case Qt::Key_PageDown:
		setRow(syncPageView->getRow() + 16);
		break;

	case Qt::Key_Left:
		setCol(syncPageView->getCol() - 1);
		break;

	case Qt::Key_Right:
		setCol(syncPageView->getCol() + 1);
		break;

	case Qt::Key_Home:
		if (event->modifiers() & Qt::ControlModifier)
			setCol(0);
		else
			setRow(0);
		break;

	case Qt::Key_End:
		if (event->modifiers() & Qt::ControlModifier)
			setCol(syncPageView->getColCount() - 1);
		else
			setRow(syncPageView->getRowCount() - 1);
		break;
	}
}

void TrackArea::resizeEvent(QResizeEvent *event)
{
	updateHScrollbar();
	updateVScrollbar();
	QScrollArea::resizeEvent(event);
	rowNumberView->move(0, syncPageNameView->height() + widget()->y());
	rowNumberView->resize(viewport()->x(), widget()->height());
	syncPageNameView->move(viewport()->x() + widget()->x(), 0);
}

void TrackArea::scrollContentsBy(int dx, int dy)
{
	// syncronize left margin
	if (dy)
		rowNumberView->move(rowNumberView->x(), rowNumberView->y() + dy);
	if (dx)
		syncPageNameView->move(syncPageNameView->x() + dx, syncPageNameView->y());

	QScrollArea::scrollContentsBy(dx, dy);
}
