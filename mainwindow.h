#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QList"
#include "images.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private slots:
	void on_actionOpen_triggered();

private:
	Ui::MainWindow *ui;
	QList<images> imagelist;
	QTabWidget *tabs;
};

#endif // MAINWINDOW_H
