#include "scene.h"
#include "QGraphicsSceneEvent"
#include "QGraphicsRectItem"
#include "QWidget"
#include "QDebug"
#include "opencv2/highgui/highgui.hpp"
extern int currenttool;
scene::scene(QObject *parent)  : QGraphicsScene(parent)
{
	selecting=false;
	rect=NULL;
}
images* scene::getMainImage()
{
	return mainimage;
}

cv::Mat scene::getSelected()
{
	cv::Mat croppedImage;
	cv::Rect myRoi(rect->rect().x(),rect->rect().y(),rect->rect().width(),rect->rect().height());

	if(currenttool==0)
	{
		cv::Point center(rect->rect().x()+rect->rect().width()/2,rect->rect().y()+rect->rect().height()/2);
		cv::Size size(rect->rect().width()/2,rect->rect().height()/2);
		cv::Mat im1(mainimage->getCvMat().rows, mainimage->getCvMat().cols, CV_8UC1, cv::Scalar(255,255,255));
		cv::Mat im2(mainimage->getCvMat().rows, mainimage->getCvMat().cols, CV_8UC1, cv::Scalar(0,0,0));
		cv::ellipse( im2, center, size, 0, 0, 360, cv::Scalar( 255, 255, 255), -1, 8 );
		cv::ellipse( im1, center, size, 0, 0, 360, cv::Scalar( 0, 0, 0), -1, 8 );
		cv::bitwise_and(mainimage->getCvMat(),im2,croppedImage);
		cv::bitwise_xor(croppedImage,im1,croppedImage);
		croppedImage=croppedImage(myRoi);
	}
	if(currenttool==1)
	{
	}
	if(currenttool==2)
	{
		croppedImage=mainimage->getCvMat()(myRoi);
	}
	if(currenttool==3)
	{
	}

	return croppedImage;
}

void scene::setMainImage(images *image)
{
	mainimage=image;
	addPixmap(mainimage->getPixmap());
	setSceneRect(0,0,mainimage->getPixmap().size().width(),mainimage->getPixmap().size().height());
}
scene::~scene()
{
	delete mainimage;
}

void scene::drawRect(QPoint start,QPoint now)
{
	QPoint delta=now-start;
	QPoint topleft=start;
	if(delta.x()<0)
		topleft.setX(topleft.x()+delta.x());
	if(delta.y()<0)
		topleft.setY(topleft.y()+delta.y());
	rect->setRect(topleft.x(),topleft.y(),abs(delta.x()),abs(delta.y()));
	rect->childItems().at(0)->setPos(0,0);
	rect->childItems().at(1)->setPos(0,delta.y());
	rect->childItems().at(2)->setPos(delta.x(),0);
	rect->childItems().at(3)->setPos(delta.x(),delta.y());

}



void scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	//qDebug()<<"1";
	if(selecting)
	{
		switch (currenttool) {
		case 0:
			break;
		case 1:
			break;
		case 2:
			drawRect(start,event->scenePos().toPoint());
			break;
		case 3:
			break;
		default:
			break;
		}

	}
}

void scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	if(selecting and event->button()==Qt::LeftButton)
		selecting=false;
}


void scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

	if(event->button()==Qt::LeftButton)
	{
		if(rect!=NULL)
		{
			removeItem(rect);
		}
		QPen pen;
		pen.setWidth(0);
		//pen.setCosmetic(true);
		pen.setColor(Qt::red);
		QBrush brush;
		start=event->scenePos().toPoint();
		switch (currenttool) {
		case 0:
			break;
		case 1:
			break;
		case 2:
			rect=addRect(start.x(),start.y(),0,0,pen,brush);
			for(int i=0;i<4;i++)
			{
				QGraphicsRectItem *corner=new QGraphicsRectItem(start.x(),start.y(),rect->rect().width()/3,rect->rect().height()/3,rect);
				corner->setPen(pen);
			}
			selecting=true;
			break;
		case 3:
			break;
		default:
			break;
		}

	}
}
