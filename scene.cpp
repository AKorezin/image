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
	cv::Rect myRoi(rect->rect().x(),rect->rect().y(),rect->rect().width(),rect->rect().height());
	cv::Mat croppedImage=mainimage->getCvMat()(myRoi);
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
			removeItem(rect);

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
			selecting=true;
			break;
		case 3:
			break;
		default:
			break;
		}

	}
}
