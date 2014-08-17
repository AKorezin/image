#include "scene.h"
#include "QDebug"
extern int currenttool;
scene::scene(QObject *parent)  : QGraphicsScene(parent)
{

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
	qDebug()<<"1";
	/*
	if(event->buttons() & Qt::MidButton)
	{
		QWidget *drager=tabs->currentWidget();
		int dx=event->x()-x,dy=event->y()-y;

		if(drager!=NULL)
		{
			qDebug()<<drager;
			if(flag)
				drager->move(drager->x()+dx,drager->y()+dy);
			x=event->x();
			y=event->y();
			flag=1;
		}
	}*/
}

void scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	qDebug()<<"2";
}

void scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	qDebug()<<"3";
}
