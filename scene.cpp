#include "scene.h"
#include "QGraphicsSceneEvent"
#include "QGraphicsRectItem"
#include "QDebug"
extern int currenttool;
scene::scene(QObject *parent)  : QGraphicsScene(parent)
{
	selecting=false;
}
images* scene::getMainImage()
{
	return mainimage;
}
void scene::setMainImage(images *image)
{
	mainimage=image;
	addPixmap(mainimage->getPixmap());
}
scene::~scene()
{
	delete mainimage;
}





void scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	//qDebug()<<event->scenePos();
	if(selecting)
	{
		QPoint delta=(event->scenePos().toPoint()-start);
		rect->setRect(start.x(),start.y(),delta.x(),delta.y());
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
		selecting=true;
		QPen pen;
		pen.setWidth(0);
		QBrush brush;
		start=event->scenePos().toPoint();
		rect=addRect(start.x(),start.y(),0,0,pen,brush);
	}
	qDebug()<<"3";
}
