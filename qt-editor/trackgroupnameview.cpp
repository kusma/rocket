#include "trackgroupnameview.h"
#include "syncpage.h"
#include "synctrack.h"

#include <QToolButton>
#include <QHBoxLayout>
#include <QPaintEvent>
#include <QStylePainter>

SyncPageNameView::SyncPageNameView(SyncPage *syncPage, QWidget *parent) :
	QWidget(parent)
{
	QHBoxLayout *layout = new QHBoxLayout();
	layout->setContentsMargins(QMargins());
	layout->setSpacing(1);
	setLayout(layout);

	connect(syncPage, SIGNAL(trackAdded(SyncTrack *)), this, SLOT(trackAdded(SyncTrack *)));
	connect(syncPage, SIGNAL(trackRemoved(int)), this, SLOT(trackRemoved(int)));
}

void SyncPageNameView::trackAdded(SyncTrack *track)
{
	QToolButton *trackNameView = new QToolButton();
	trackNameView->setText(track->getName());
	trackNameView->setFixedWidth(fontMetrics().width(' ') * 16);
	trackNameView->setEnabled(false);
//	trackNameView->setStyleSheet("background-color: red");

	trackNameViews.append(trackNameView);
	layout()->addWidget(trackNameView);
	trackNameView->show();
	adjustSize();
}

void SyncPageNameView::trackRemoved(int index)
{
	QWidget *trackNameView = trackNameViews[index];
	layout()->removeWidget(trackNameView);
	trackNameViews.removeAt(index);
	delete trackNameView;
	adjustSize();
}
