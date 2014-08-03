#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "scene.h"
#include "QFileDialog"
#include "QLabel"
#include "QMouseEvent"
#include "QGraphicsView"
#include "QToolBar"
#include "settingsdialog.h"
#include "QDebug"
#include "opencv2/highgui/highgui.hpp"

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
	addToolBar(Qt::LeftToolBarArea, createToolBar());
}

QToolBar* MainWindow::createToolBar()
{
	lefttoolbar=new QToolBar("test");
	lefttoolbar->setMovable(0);
	//lefttoolbar->setMaximumWidth(20);
	lefttoolbar->addAction(QPixmap(":/circ.png"),"circle");
	lefttoolbar->addAction(QPixmap(":/line.png"),"line");
	lefttoolbar->addAction(QPixmap(":/rect.png"),"rect");
	lefttoolbar->addAction(QPixmap(":/marker.png"),"marker");
	return lefttoolbar;
}

void MainWindow::loadSettings()
{
	curentSettings=new QSettings;
}

void MainWindow::saveSettings()
{

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
	tabs->resize(this->size().width()-this->lefttoolbar->width(),this->height()-ui->menuBar->height()+1);
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

void MainWindow::on_actionOpen_triggered()
{

	QString filename=QFileDialog::getOpenFileName(this,"Open File","","TIFF Images(*.tiff *.tif);;PNG Images(*.png)");
	cv::Mat image=cv::imread(filename.toUtf8().data(),CV_LOAD_IMAGE_GRAYSCALE);
	if(!image.data)
	{
		return;
	}
	setActionsEnabled();
	images *newimage=new images(image);
	scene *pixmap=new scene;
	scenelist<<pixmap;
	pixmap->setMainImage(newimage);
	QGraphicsView *view=new QGraphicsView;
	view->setScene(pixmap);
	view->show();
	tabs->addTab(view,filename);


}

void MainWindow::on_actionClose_triggered()
{
	delete scenelist[tabs->currentIndex()];
	scenelist.removeAt(tabs->currentIndex());
	tabs->removeTab(tabs->currentIndex());
	if(scenelist.size()==0)
		setActionsDisabled();
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
	setActionsEnabled();
	images *newimage=new images(image);
	scene *pixmap=new scene;
	pixmap->setMainImage(newimage);
	scenelist<<pixmap;
	QGraphicsView *view=new QGraphicsView;
	view->setScene(pixmap);
	view->show();
	tabs->addTab(view,filename);
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
