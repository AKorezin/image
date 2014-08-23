#include "scene.h"
#include "QGraphicsSceneEvent"
#include "QGraphicsRectItem"
#include "QDebug"
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
void scene::setMainImage(images *image)
{
	mainimage=image;
	addPixmap(mainimage->getPixmap());
	setSceneRect(0,0,mainimage->getPixmap().size().width(),mainimage->getPixmap().size().height());
}
scene::~scene()
{
	delete mainimage;
	//delete rect;
}

void scene::drawRect(QPoint start,QPoint now)
{
	QPoint delta=(now-start);
	QPoint topleft=start;
	if(delta.x()<0)
		topleft.setX(topleft.x()+delta.x());
	if(delta.y()<0)
		topleft.setY(topleft.y()+delta.y());

	rect->setRect(topleft.x(),topleft.y(),abs(delta.x()),abs(delta.y()));
}



void scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if(selecting)
	{
		drawRect(start,event->scenePos().toPoint());
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
		if(rect!=NULL)
			removeItem(rect);

		QPen pen;
		pen.setWidth(0);
		QBrush brush;
		start=event->scenePos().toPoint();


		switch (currenttool) {
		case 2:
			rect=addRect(start.x(),start.y(),0,0,pen,brush);
			selecting=true;
			break;
		default:
			break;
		}

	}
	qDebug()<<"3";
}
