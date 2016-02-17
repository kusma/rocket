#include "syncpageview.h"
#include "trackview.h"
#include "syncpage.h"

#include <QApplication>
#include <QHBoxLayout>

SyncPageView::SyncPageView(SyncPage *syncPage, QWidget *parent) :
	QWidget(parent),
	syncPage(syncPage),
	currRow(-1),
	currCol(-1)
{
	QHBoxLayout *layout = new QHBoxLayout();
	layout->setContentsMargins(QMargins());
	layout->setSpacing(1);
	setLayout(layout);
	setRowCount(128);

	connect(syncPage, SIGNAL(trackAdded(SyncTrack *)), this, SLOT(trackAdded(SyncTrack *)));
	connect(syncPage, SIGNAL(trackRemoved(int)), this, SLOT(trackRemoved(int)));
}

void SyncPageView::trackAdded(SyncTrack *track)
{
	TrackView *trackView = new TrackView(track);
	trackViews.append(trackView);
	layout()->addWidget(trackView);
	trackView->show();
	adjustSize();
}

void SyncPageView::trackRemoved(int index)
{
	TrackView *trackView = trackViews[index];
	layout()->removeWidget(trackView);
	trackViews.removeAt(index);
	delete trackView;
	adjustSize();
}

QRect SyncPageView::getCurrentTrackRect() const
{
	if (currCol < 0)
		return QRect(0, 0, 0, 0);

	Q_ASSERT(trackViews.size());
	return trackViews[currCol]->geometry();
}

void SyncPageView::setRow(int row)
{
	Q_ASSERT(row >= 0 && row < rowCount);

	if (!trackViews.size())
		return;

	Q_ASSERT(currCol >= 0 && currCol < trackViews.size());
	trackViews[currCol]->setRowHilight(row);
	currRow = row;
}

void SyncPageView::setCol(int col)
{
	if (!trackViews.size())
		return;

	col = qMin(qMax(col, 0), trackViews.size() - 1);
	if (currCol != col) {
		if (currCol >= 0)
			trackViews[currCol]->setRowHilight(-1);

		trackViews[col]->setRowHilight(currRow);
		currCol = col;
	} else
		QApplication::beep();
}

void SyncPageView::setRowCount(int rows)
{
	setFixedHeight(fontMetrics().lineSpacing() * rows);
	rowCount = rows;
}

void SyncPageView::changeEvent(QEvent *event)
{
	if (event->type() == QEvent::FontChange) {
		Q_ASSERT(fontInfo().fixedPitch());
		setFixedHeight(fontMetrics().lineSpacing() * rowCount);
	}
}
