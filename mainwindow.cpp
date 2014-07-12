#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFileDialog"
#include "opencv2/highgui/highgui.hpp"
MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

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
	cv::namedWindow("Display window", cv::WINDOW_AUTOSIZE );
	cv::imshow( "Display window", image );
}
