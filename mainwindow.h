#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QList"
#include "QSettings"
#include "scene.h"
#include "images.h"
#include "settingsdialog.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	//void loadSettings();
	~MainWindow();

private slots:
	void on_actionOpen_triggered();

	void on_actionClose_triggered();

	void on_actionExport_triggered();

	void on_actionImport_triggered();

	void onTabClose(int);

	void on_actionSave_triggered();

	void on_actionSaveAs_triggered();

	void openSettingsDialog();

	void saveSettings();
private:
	Ui::MainWindow *ui;
	QList<images*> imagelist;
	QList<scene *> scenelist;
	QTabWidget *tabs;
	settingsDialog *dialog;
	QToolBar* lefttoolbar;
	void setActionsDisabled();
	void setActionsEnabled();
	void initGui();
	QToolBar* createToolBar();
	void loadSettings();
	void resizeEvent(QResizeEvent *);
	int x,y,flag,loaded;
	QSettings *curentSettings;
};

#endif // MAINWINDOW_H
