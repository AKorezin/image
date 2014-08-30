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
	ellipse=NULL;
	line=NULL;
}
images* scene::getMainImage()
{
	return mainimage;
}

cv::Mat scene::getSelected()
{
	cv::Mat croppedImage;
	cv::Rect myRoi(rect->rect().x(),rect->rect().y(),rect->rect().width(),rect->rect().height());
	switch (currenttool) {
	case 0:
		croppedImage=mainimage->ellipseCrop(rect);
		break;
	case 1:
		break;
	case 2:
		croppedImage=mainimage->getCvMat()(myRoi);
		break;
	case 3:
		break;
	default:
		break;
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

	handles.at(1)->setPos(0,delta.y());
	handles.at(2)->setPos(delta.x(),0);
	handles.at(3)->setPos(delta);
}

void scene::drawEllipse(QPoint start, QPoint now)
{
	QPoint delta=now-start;
	QPoint topleft=start;
	if(delta.x()<0)
		topleft.setX(topleft.x()+delta.x());
	if(delta.y()<0)
		topleft.setY(topleft.y()+delta.y());
	rect->setRect(topleft.x(),topleft.y(),abs(delta.x()),abs(delta.y()));
	ellipse->setRect(rect->rect());
	handles.at(1)->setPos(0,delta.y());
	handles.at(2)->setPos(delta.x(),0);
	handles.at(3)->setPos(delta);
}

void scene::drawLine(QPoint start, QPoint now)
{
	line->setLine(start.x(),start.y(),now.x(),now.y());
	handles.at(1)->setPos(line->line().dx(),line->line().dy());
}

void scene::drawMarker(QPoint now)
{
	rect->setPos(now-start);
}



void scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if(selecting)
	{
		switch (tool) {
		case 0:
			drawEllipse(start,event->scenePos().toPoint());
			break;
		case 1:
			drawLine(start,event->scenePos().toPoint());
			break;
		case 2:
			drawRect(start,event->scenePos().toPoint());
			break;
		case 3:
			drawMarker(event->scenePos().toPoint());
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

	if(event->button()==Qt::LeftButton and currenttool!=-1)
	{
		int check=-1;
		if(!handles.empty())
			for(int i=0;handles.size();i++)
				if(handles.at(i)->contains(event->pos()))
					check=i;
		if(check==-1)
			prepareItem();
		else
		{

		}

	}
}

void scene::prepareItem()
{
	selecting=true;
	start=event->scenePos().toPoint();
	tool=currenttool;
	handles.clear();
	if(rect!=NULL)
		removeItem(rect);
	if(ellipse!=NULL)
		removeItem(ellipse);
	if(line!=NULL)
		removeItem(line);
	QPen pen;
	pen.setWidth(0);
	pen.setColor(Qt::red);
	QBrush brush;
	switch (tool) {
	case 0:
	{
		rect=addRect(start.x(),start.y(),0,0,pen,brush);
		for(int i=0;i<4;i++)
		{
			QGraphicsRectItem* handle=new QGraphicsRectItem(start.x()-3,start.y()-3,6,6,rect);
			handle->setPen(pen);
			handles<<handle;
		}
		ellipse=addEllipse(rect->rect(),pen,brush);
		break;
	}
	case 1:
	{
		line=addLine(start.x(),start.y(),start.x(),start.y(),pen);
		for(int i=0;i<2;i++)
		{
			QGraphicsRectItem* handle=new QGraphicsRectItem(start.x()-3,start.y()-3,6,6,line);
			handle->setPen(pen);
			handles<<handle;
		}
		break;
	}
	case 2:
	{
		rect=addRect(start.x(),start.y(),0,0,pen,brush);
		for(int i=0;i<4;i++)
		{
			QGraphicsRectItem* handle=new QGraphicsRectItem(start.x()-3,start.y()-3,6,6,rect);
			handle->setPen(pen);
			handles<<handle;
		}
		break;
	}
	case 3:
	{
		rect=addRect(start.x()-3,start.y()-3,6,6,pen,brush);
		QGraphicsRectItem* handle=new QGraphicsRectItem(start.x()-3,start.y()-3,6,6,rect);
		QGraphicsLineItem* line=new QGraphicsLineItem(start.x()-10,start.y(),start.x()+10,start.y(),rect);
		line->setPen(pen);
		line=new QGraphicsLineItem(start.x(),start.y()-10,start.x(),start.y()+10,rect);
		line->setPen(pen);
		handles<<handle;
	}
		break;
	default:
		break;
	}
}
