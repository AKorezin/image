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
	qDebug()<<currenttool;
}
scene::~scene()
{
	delete mainimage;
}

/*



void MainWindow::mouseMoveEvent(QMouseEvent *event)
{

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
	}
}

void MainWindow::mouseReleaseEvent(QMouseEvent * event)
{
	if((event->button() == Qt::MidButton) && flag)
	{
		qDebug()<<"release";
		flag=0;
	}
}
*/
