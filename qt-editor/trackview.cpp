#include "trackview.h"

#include <QApplication>
#include <QPainter>
#include <QMenuBar>
#include <QStatusBar>
#include <QKeyEvent>
#include <QScrollBar>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QLabel>
#include <QDebug>

#include "synctrack.h"
#include "syncpageview.h"

static QColor lerp(const QColor &a, const QColor &b, float t)
{
	QColor result;
	result.setRedF(a.redF() + (b.redF() - a.redF()) * t);
	result.setGreenF(a.greenF() + (b.greenF() - a.greenF()) * t);
	result.setBlueF(a.blueF() + (b.blueF() - a.blueF()) * t);
	result.setAlphaF(a.alphaF() + (b.alphaF() - a.alphaF()) * t);
	return result;
}

QBrush TrackView::getBackgroundBrush(int row) const
{
	if (isHighlit(row))
		return palette().highlight().color();
	else {
		QColor baseColor = row % 8 ?
		                   palette().base().color() :
		                   palette().alternateBase().color();

		const SyncTrack::TrackKey *prevKey = track->getPrevKeyFrame(row);
		if (!prevKey)
			return QBrush(baseColor);

		QColor typeColor;
		switch (prevKey->type) {
		case SyncTrack::TrackKey::LINEAR:
			typeColor = Qt::red;
			break;
		case SyncTrack::TrackKey::RAMP:
			typeColor = Qt::green;
			break;
		case SyncTrack::TrackKey::SMOOTH:
			typeColor = Qt::blue;
			break;

		default:
			return QBrush(baseColor);
		}

		return QBrush(lerp(baseColor, typeColor, 0.125f));
	}
}

void TrackView::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);

	QRect rect;
	rect.setLeft(0);
	rect.setRight(width());

	int lineSpacing = painter.fontMetrics().lineSpacing();
	int startRow = event->rect().top() / lineSpacing;
	int stopRow = (event->rect().bottom() + lineSpacing - 1) / lineSpacing;
	for (int r = startRow; r < stopRow; ++r) {
		rect.setTop(r * lineSpacing);
		rect.setBottom((r + 1) * lineSpacing);

		if (!event->region().intersects(rect))
			continue;

		QRect clipRect = rect.intersected(event->rect());
		painter.fillRect(clipRect, getBackgroundBrush(r));
		painter.setPen(getTextColor(r));

		QString temp = QString("---");
		if (track->isKeyFrame(r))
			temp.setNum(track->getKeyFrame(r).value);
		painter.drawText(rect, 0, temp, &clipRect);
	}
}

void TrackView::changeEvent(QEvent *event)
{
	if (event->type() == QEvent::FontChange) {
		Q_ASSERT(fontInfo().fixedPitch());
		setFixedWidth(fontMetrics().width(' ') * 16);
	}
}
