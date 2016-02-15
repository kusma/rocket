#include "mainwindow.h"

#include <QMenuBar>
#include <QStatusBar>
#include <QInputDialog>
#include <QString>

#include "trackarea.h"

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);

	trackArea = new TrackArea(this);
	setCentralWidget(trackArea);
}

MainWindow::~MainWindow()
{

}

void MainWindow::on_actionSet_rows_triggered()
{
	bool ok = false;
	int newRowCount = QInputDialog::getInt(this,
					       "Set rows", "",
					       trackArea->getRowCount(),
					       0, INT_MAX, 1, &ok);
	if (ok)
		trackArea->setRowCount(newRowCount);
	else
		QApplication::beep();
}
