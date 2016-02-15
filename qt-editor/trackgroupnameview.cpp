#include "trackgroupnameview.h"
#include "trackgroup.h"
#include "track.h"

#include <QToolButton>
#include <QHBoxLayout>
#include <QPaintEvent>
#include <QStylePainter>

TrackGroupNameView::TrackGroupNameView(TrackGroup *trackGroup, QWidget *parent) :
	QWidget(parent)
{
	QHBoxLayout *layout = new QHBoxLayout();
	layout->setContentsMargins(QMargins());
	layout->setSpacing(1);
	setLayout(layout);

	connect(trackGroup, SIGNAL(trackAdded(Track *)), this, SLOT(trackAdded(Track *)));
	connect(trackGroup, SIGNAL(trackRemoved(int)), this, SLOT(trackRemoved(int)));
}

void TrackGroupNameView::trackAdded(Track *track)
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

void TrackGroupNameView::trackRemoved(int index)
{
	QWidget *trackNameView = trackNameViews[index];
	layout()->removeWidget(trackNameView);
	trackNameViews.removeAt(index);
	delete trackNameView;
	adjustSize();
}
