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
	void resizeEvent(QResizeEvent *);
	~MainWindow();

private slots:
	void on_actionOpen_triggered();

	void on_actionClose_triggered();

	void on_actionExport_triggered();

	void on_actionImport_triggered();

	void onTabClose(int);

private:
	Ui::MainWindow *ui;
	QList<images*> imagelist;
	QTabWidget *tabs;
	void setActionsDisabled();
	void setActionsEnabled();
};

#endif // MAINWINDOW_H
