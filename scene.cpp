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
	translate=false;
	rect=NULL;
	ellipse=NULL;
	line=NULL;
	handlew=8;
	handleh=8;
	penwidth=2;
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

QList<int> scene::getProfile()
{
	QList<int> range;
	cv::Point p1,p2;
	p1.x=line->line().x1();
	p1.y=line->line().y1();
	p2.x=line->line().x2();
	p2.y=line->line().y2();
	cv::LineIterator it(mainimage->getCvMat(), p1, p2, 8,true);
	for(int i = 0; i < it.count; i++, it++)
		range<<it.ptr[0];
	return range;
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
	handles.at(0)->setRect(topleft.x()-handlew/2,topleft.y()-handleh/2,handlew,handleh);
	handles.at(1)->setRect(topleft.x()-handlew/2,topleft.y()+abs(delta.y())-handleh/2,handlew,handleh);
	handles.at(2)->setRect(topleft.x()+abs(delta.x())-handlew/2,topleft.y()+abs(delta.y())-handleh/2,handlew,handleh);
	handles.at(3)->setRect(topleft.x()+abs(delta.x())-handlew/2,topleft.y()-handleh/2,handlew,handleh);
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
	handles.at(0)->setRect(topleft.x()-handlew/2,topleft.y()-handleh/2,handlew,handleh);
	handles.at(1)->setRect(topleft.x()-handlew/2,topleft.y()+abs(delta.y())-handleh/2,handlew,handleh);
	handles.at(2)->setRect(topleft.x()+abs(delta.x())-handlew/2,topleft.y()+abs(delta.y())-handleh/2,handlew,handleh);
	handles.at(3)->setRect(topleft.x()+abs(delta.x())-handlew/2,topleft.y()-handleh/2,handlew,handleh);

}

void scene::drawLine(QPoint start_l, QPoint now)
{
	if(translate)
	{
		QPoint p1,p2,dp;
		dp=now-start_l;
		p1=line->line().p1().toPoint();
		p2=line->line().p2().toPoint();
		p1+=dp;
		p2+=dp;
		line->setLine(p1.x(),p1.y(),p2.x(),p2.y());
		handles.at(0)->setRect(p1.x()-handlew/2,p1.y()-handleh/2,handlew,handleh);
		handles.at(1)->setRect(p2.x()-handlew/2,p2.y()-handleh/2,handlew,handleh);
		handles.at(2)->setRect(now.x()-handlew/2,now.y()-handleh/2,handlew,handleh);
		start=now;
	}
	else
	{
		line->setLine(start_l.x(),start_l.y(),now.x(),now.y());
		handles.at(0)->setRect(start_l.x()-handlew/2,start.y()-handleh/2,handlew,handleh);
		handles.at(1)->setRect(now.x()-handlew/2,now.y()-handleh/2,handlew,handleh);
		handles.at(2)->setRect((now.x()+start_l.x())/2-handlew/2,(now.y()+start_l.y())/2-handleh/2,handlew,handleh);
	}
}

void scene::drawMarker(QPoint now)
{
	rect->setPos(now-start);
}

void scene::setScale(double factor)
{
	handleh/=factor;
	handlew/=factor;
	penwidth/=factor;
	for(int i=0;i<handles.size();i++)
	{
		QRectF rect=handles.at(i)->rect();
		rect.setX(rect.x()+rect.width()/2-handlew/2);
		rect.setY(rect.y()+rect.height()/2-handleh/2);
		rect.setWidth(handlew);
		rect.setHeight(handleh);
		handles.at(i)->setRect(rect);
	}
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
	if(!handles.empty())
	{
		int check=0;
		for(int i=0;i<handles.size();i++)
		{
			if(handles.at(i)->contains(event->scenePos()))
				check=i+1;
		}
		QPen pen;
		pen.setWidth(0);
		pen.setColor(Qt::red);
		for(int i=0;i<handles.size();i++)
		{
			handles.at(i)->setPen(pen);
		}
		if(check)
		{
			QPen pen;
			pen.setWidthF(penwidth);
			pen.setColor(Qt::green);
			handles.at(check-1)->setPen(pen);
		}
	}
}



void scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	if((selecting or translate) and event->button()==Qt::LeftButton)
	{
		selecting=false;
		translate=false;
		int number=0;
		for(int i=1;i<handles.size();i++)
			if(handles.at(i-1)->rect()==handles.at(i)->rect())
				number++;
		if (number)
		{
			for(int i=0;i<handles.size();i++)
				removeItem(handles.at(i));
			handles.clear();
		}
	}

}


void scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

	if(event->button()==Qt::LeftButton and currenttool!=-1)
	{
		bool check=false;
		for(int i=0;i<handles.size();i++)
		{
			if(handles.at(i)->contains(event->scenePos()))
				check=true;
		}
		if(!check)
			prepareItem(event->scenePos().toPoint());
		else
		{
			prepareMove(event->scenePos().toPoint());
		}

	}
}

void scene::prepareItem(QPoint startpoint)
{
	selecting=true;
	start=startpoint;
	tool=currenttool;

	if(rect!=NULL)
	{
		removeItem(rect);
		rect=NULL;
	}
	if(ellipse!=NULL)
	{
		removeItem(ellipse);
		ellipse=NULL;
	}
	if(line!=NULL)
	{
		removeItem(line);
		line=NULL;
	}
	handles.clear();


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
			QGraphicsRectItem* handle=new QGraphicsRectItem(start.x()-handlew/2,start.y()-handleh/2,handlew,handleh,rect);
			handle->setPen(pen);
			handles<<handle;
		}
		ellipse=addEllipse(rect->rect(),pen,brush);
		break;
	}
	case 1:
	{
		line=addLine(start.x(),start.y(),start.x(),start.y(),pen);
		for(int i=0;i<3;i++)
		{
			QGraphicsRectItem* handle=new QGraphicsRectItem(start.x()-handlew/2,start.y()-handleh/2,handlew,handleh,line);
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
			QGraphicsRectItem* handle=new QGraphicsRectItem(start.x()-handlew/2,start.y()-handleh/2,handlew,handleh,rect);
			handle->setPen(pen);
			handles<<handle;
		}
		break;
	}
	case 3:
	{
		rect=addRect(start.x()-handlew/2,start.y()-handleh/2,handlew,handleh,pen,brush);
		QGraphicsRectItem* handle=new QGraphicsRectItem(start.x()-handlew/2,start.y()-handleh/2,handlew,handleh,rect);
		handle->setPen(pen);
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

void scene::prepareMove(QPoint startpoint)
{
	selecting=true;
	int handlenum=0;

	for(int i=0;i<handles.size();i++)
		if(handles.at(i)->contains(startpoint))
			handlenum=i;

	if(handles.size()==4)
		handlenum=(handlenum+2)%4;
	if(handles.size()==3)
	{
		if(handlenum<2)
			handlenum=(handles.size()+1)%2;
		else
			translate=true;
	}
	start=QPoint(handles.at(handlenum)->rect().x()+handlew/2,handles.at(handlenum)->rect().y()+handleh/2);
}
