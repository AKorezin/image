#include "scene.h"

scene::scene(QObject *parent)  : QGraphicsScene(parent)
{

}

/*

void MainWindow::resizeEvent(QResizeEvent *)
{
	tabs->resize(this->size().width(),this->height()-ui->menuBar->height()+1);
}

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
