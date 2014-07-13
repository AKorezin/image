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
	qDebug()<<"test";
	tabs->resize(ui->centralWidget->geometry().size());
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
	images *newimage=new images(image);
	imagelist<<*newimage;
	QLabel *mylabel=new QLabel;
	mylabel->setPixmap(newimage->getPixmap());
	mylabel->show();

	tabs->addTab(mylabel,filename);
}
