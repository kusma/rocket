#ifndef ROWNUMBERVIEW_H
#define ROWNUMBERVIEW_H

#include <QWidget>

class RowNumberView : public QWidget {
	Q_OBJECT
public:
	RowNumberView(QWidget *parent = 0) :
		QWidget(parent),
		rowHilight(0)
	{
		setAttribute(Qt::WA_OpaquePaintEvent, true);
	}

	void setRowHilight(int row)
	{
		int oldRow = rowHilight;
		rowHilight = row;
		updateRow(oldRow);
		updateRow(row);
	}

	void updateRow(int row)
	{
		update(0, row * fontMetrics().lineSpacing(), width(), fontMetrics().lineSpacing());
	}

	void setRowCount(int rows)
	{
		setFixedHeight(fontMetrics().lineSpacing() * rows);
	}

protected:
	void paintEvent(QPaintEvent *event);

private:
	int rowHilight;
};

#endif // ROWNUMBERVIEW_H
