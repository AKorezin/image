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

	void on_actionClose_triggered();

	void on_actionExport_triggered();

	void on_actionImport_triggered();

	void onTabClose(int);

	void on_actionSave_triggered();

	void on_actionSaveAs_triggered();

private:
	void resizeEvent(QResizeEvent *);
	void mouseMoveEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent *);
	Ui::MainWindow *ui;
	QList<images*> imagelist;
	QTabWidget *tabs;
	void setActionsDisabled();
	void setActionsEnabled();
	int x,y,flag;
};

#endif // MAINWINDOW_H
