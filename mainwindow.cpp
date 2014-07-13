#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFileDialog"
#include "QLabel"
#include "opencv2/highgui/highgui.hpp"
#include "QDebug"
MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	tabs=new QTabWidget(ui->centralWidget);
	tabs->resize(this->size().width(),this->height()-ui->menuBar->height());
	ui->actionSave->setDisabled(1);
	ui->actionSaveAs->setDisabled(1);
	ui->actionExport->setDisabled(1);
	ui->actionClose->setDisabled(1);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_actionOpen_triggered()
{

	QString filename=QFileDialog::getOpenFileName(this,"Open File","","TIFF Images(*.tiff *.tif);;PNG Images(*.png)");
	cv::Mat image=cv::imread(filename.toUtf8().data(),CV_LOAD_IMAGE_GRAYSCALE);
	if(!image.data)
	{
		return;
	}
	ui->actionSave->setEnabled(1);
	ui->actionSaveAs->setEnabled(1);
	ui->actionExport->setEnabled(1);
	ui->actionClose->setEnabled(1);
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
	{
		ui->actionSave->setDisabled(1);
		ui->actionSaveAs->setDisabled(1);
		ui->actionExport->setDisabled(1);
		ui->actionClose->setDisabled(1);
	}
}

void MainWindow::on_actionExport_triggered()
{
	QString filename=QFileDialog::getSaveFileName(this,"Export to matrix","","YAML File(*.yaml);;XML File(*.xml)");
	cv::FileStorage file(filename.toUtf8().data(), cv::FileStorage::WRITE);
	file<<"Test"<<imagelist.at(tabs->currentIndex())->getCvMat();
	file.release();
}
