#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"

class TrackArea;

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~MainWindow();

private slots:
	void on_actionSet_rows_triggered();

private:
	Ui::MainWindowClass ui;

	TrackArea *trackArea;
};

#endif // MAINWINDOW_H
