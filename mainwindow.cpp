#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFileDialog"
#include "QLabel"
#include "opencv2/highgui/highgui.hpp"
MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	tabs=new QTabWidget(ui->centralWidget);
	tabs->setTabsClosable(1);
	connect(tabs,SIGNAL(tabCloseRequested(int)),this,SLOT(onTabClose(int)));
	setActionsDisabled();
}

void MainWindow::resizeEvent(QResizeEvent *)
{
	tabs->resize(this->size().width(),this->height()-ui->menuBar->height()+1);
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

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::onTabClose(int index)
{
	imagelist.removeAt(index);
	tabs->removeTab(index);
	if(imagelist.size()==0)
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
	imagelist<<newimage;
	QLabel *mylabel=new QLabel;
	mylabel->setPixmap(newimage->getPixmap());
	mylabel->show();
	tabs->addTab(mylabel,filename);

}

void MainWindow::on_actionClose_triggered()
{
	imagelist.removeAt(tabs->currentIndex());
	tabs->removeTab(tabs->currentIndex());
	if(imagelist.size()==0)
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
	file<<"Image"<<imagelist.at(tabs->currentIndex())->getCvMat();
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
	imagelist<<newimage;
	QLabel *mylabel=new QLabel;
	mylabel->setPixmap(newimage->getPixmap());
	mylabel->show();
	tabs->addTab(mylabel,filename);
}