#include "scene.h"
#include "QGraphicsSceneEvent"
#include "QGraphicsRectItem"
#include "QDebug"
extern int currenttool;
scene::scene(QObject *parent)  : QGraphicsScene(parent)
{
	selecting=false;
	rect=new QGraphicsRectItem;
}
images* scene::getMainImage()
{
	return mainimage;
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
	delete rect;
}





void scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if(selecting)
	{

		QPoint delta=(event->scenePos().toPoint()-start);
		if(delta.x()<0 or delta.y()<0)
			rect->setRect(start.x()+delta.x(),start.y()+delta.y(),-delta.x(),-delta.y());
		else
			rect->setRect(start.x(),start.y(),delta.x(),delta.y());
		qDebug()<<rect->rect();
	}
}

void scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	if(selecting and event->button()==Qt::LeftButton)
		 selecting=false;
	qDebug()<<"2";
}

void scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if(event->button()==Qt::LeftButton)
	{
		//qDebug()<<rect;
		if(rect!=NULL)
			removeItem(rect);
		selecting=true;
		QPen pen;
		pen.setWidth(0);
		QBrush brush;
		start=event->scenePos().toPoint();
		rect=addRect(start.x(),start.y(),0,0,pen,brush);
	}
	qDebug()<<"3";
}
