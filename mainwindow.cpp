#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "scene.h"
#include "QFileDialog"
#include "QLabel"
#include "QMouseEvent"
#include "QToolBar"
#include "settingsdialog.h"
#include "view.h"
#include "QDebug"
#include "QActionGroup"
#include "opencv2/highgui/highgui.hpp"
int currenttool=-1;
MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	initGui();
	loaded=0;
	loadSettings();
	flag=0;
}

void MainWindow::initGui()
{
	ui->setupUi(this);
	this->setWindowTitle("Image");
	tabs=new QTabWidget(ui->centralWidget);
	tabs->setTabsClosable(1);
	connect(tabs,SIGNAL(tabCloseRequested(int)),this,SLOT(onTabClose(int)));
	setActionsDisabled();
	tabs->setStyleSheet("border:0px solid black;");
	QAction *settings=ui->menuBar->addAction("Настройки");
	dialog=new settingsDialog;
	dialog->setSettings(curentSettings);
	connect(settings,SIGNAL(triggered()),this,SLOT(openSettingsDialog()));
	createToolBar();
	addToolBar(Qt::LeftToolBarArea, lefttoolbar);


}

void MainWindow::createToolBar()
{
	lefttoolbar = new QToolBar("lefttoolbar");
	lefttoolbar->setMovable(0);
	actiongroup = new QActionGroup(lefttoolbar);
	actiongroup->addAction(QIcon(":/circ.png"),"Line");
	actiongroup->addAction(QIcon(":/line.png"),"Circle");
	actiongroup->addAction(QIcon(":/rect.png"),"Rectangle");
	actiongroup->addAction(QIcon(":/marker.png"),"Marker");
	lefttoolbar->addActions(actiongroup->actions());
	foreach(QAction *action, actiongroup->actions())
	{
		action->setCheckable(true);
	}
	connect(actiongroup,SIGNAL(triggered(QAction*)),this,SLOT(actionGroupChecked(QAction*)));
}

void MainWindow::actionGroupChecked(QAction * activeAction)
{
	currenttool=actiongroup->actions().indexOf(activeAction);
}

void MainWindow::loadSettings()
{
	curentSettings=new QSettings;
}

void MainWindow::saveSettings()
{

}

void MainWindow::exportSelected()
{
	QString filename=QFileDialog::getSaveFileName(this,"Export to matrix","","YAML Files(*.yaml);;XML Files(*.xml);;All Files(*)");
	QFile checkfile(filename);
	if(!checkfile.open(QIODevice::WriteOnly | QIODevice::Text))
		return;
	checkfile.close();
	cv::FileStorage file(filename.toUtf8().data(), cv::FileStorage::WRITE);
	file<<"Image"<<scenelist.at(tabs->currentIndex())->getSelected();
	file.release();

}

void MainWindow::buildProfile()
{
	scenelist.at(tabs->currentIndex())->getProfile();

}

void MainWindow::openSettingsDialog()
{
	connect(dialog,SIGNAL(accepted()),this,SLOT(saveSettings()));
	dialog->show();
}

void MainWindow::setActionsDisabled()
{
	ui->actionSave->setDisabled(1);
	ui->actionSaveAs->setDisabled(1);
	ui->actionExport->setDisabled(1);
	ui->actionClose->setDisabled(1);
}
void MainWindow::setActionsEnabled()
{
	ui->actionSave->setEnabled(1);
	ui->actionSaveAs->setEnabled(1);
	ui->actionExport->setEnabled(1);
	ui->actionClose->setEnabled(1);
}

void MainWindow::resizeEvent(QResizeEvent *)
{
	tabs->resize(this->size().width()-this->lefttoolbar->width(),this->size().height()-ui->menuBar->height()+1);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::onTabClose(int index)
{
	delete scenelist[tabs->currentIndex()];
	scenelist.removeAt(index);
	tabs->removeTab(index);
	if(scenelist.size()==0)
		setActionsDisabled();
}

void MainWindow::on_actionClose_triggered()
{
	onTabClose(tabs->currentIndex());
}

void MainWindow::on_actionOpen_triggered()
{

	QString filename=QFileDialog::getOpenFileName(this,"Open File","","TIFF Images(*.tiff *.tif);;PNG Images(*.png)");
	QFile checkfile(filename);
	if(!checkfile.exists())
		return;
	cv::Mat image=cv::imread(filename.toUtf8().data(),CV_LOAD_IMAGE_GRAYSCALE);
	if(!image.data)
	{
		return;
	}
	displayLoaded(image, filename);
}

void MainWindow::on_actionImport_triggered()
{
	QString filename=QFileDialog::getOpenFileName(this,"Import File","","OpenCV Filestorages(*.yaml *.xml);;All Files(*)");
	QFile checkfile(filename);
	if(!checkfile.exists())
		return;
	cv::FileStorage file(filename.toUtf8().data(), cv::FileStorage::READ);
	cv::Mat image;
	file["Image"]>>image;
	file.release();
	if(!image.data)
	{
		return;
	}
	displayLoaded(image,filename);
}

void MainWindow::displayLoaded(cv::Mat image,QString filename)
{
	setActionsEnabled();
	images *newimage=new images(image);
	scene *pixmap=new scene;
	scenelist<<pixmap;
	pixmap->setMainImage(newimage);
	view *newview=new view;
	connect(newview,SIGNAL(scaleChanged(double)),pixmap,SLOT(setScale(double)));
	newview->setScene(pixmap);
	newview->show();
	connect(newview,SIGNAL(export_triggered()),this,SLOT(exportSelected()));
	connect(newview,SIGNAL(profile_triggered()),this,SLOT(buildProfile()));
	tabs->addTab(newview,filename);
}

void MainWindow::on_actionSave_triggered()
{
	QString filename=tabs->tabText(tabs->currentIndex());
	QFile checkfile(filename);
	if((filename.endsWith("tiff",Qt::CaseInsensitive) or filename.endsWith("tif",Qt::CaseInsensitive)) and checkfile.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		checkfile.close();
		cv::imwrite(filename.toUtf8().data(),scenelist.at(tabs->currentIndex())->getMainImage()->getCvMat());
	}
	else
		on_actionSaveAs_triggered();
}

void MainWindow::on_actionSaveAs_triggered()
{
	QString filename=QFileDialog::getSaveFileName(this,"Save Image as","","TIFF Files(*.tiff *.tif);;PNG Files(*.png);;All Files(*)");
	QFile checkfile(filename);
	if(!checkfile.open(QIODevice::WriteOnly | QIODevice::Text))
		return;
	checkfile.close();
	cv::imwrite(filename.toUtf8().data(),scenelist.at(tabs->currentIndex())->getMainImage()->getCvMat());
}


void MainWindow::on_actionExport_triggered()
{
	QString filename=QFileDialog::getSaveFileName(this,"Export to matrix","","YAML Files(*.yaml);;XML Files(*.xml);;All Files(*)");
	QFile checkfile(filename);
	if(!checkfile.open(QIODevice::WriteOnly | QIODevice::Text))
		return;
	checkfile.close();
	cv::FileStorage file(filename.toUtf8().data(), cv::FileStorage::WRITE);
	file<<"Image"<<scenelist.at(tabs->currentIndex())->getMainImage()->getCvMat();
	file.release();
}
