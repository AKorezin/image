#include "view.h"
#include "scene.h"
#include "QMenu"
#include "QMouseEvent"
#include "QScrollBar"
#include "QDebug"
extern int currenttool;
view::view(QWidget *parent) : QGraphicsView(parent)
{
	_pan=false;
	setMouseTracking(true);
}

void view::mouseMoveEvent(QMouseEvent *event)
{
	if (_pan)
	{
		horizontalScrollBar()->setValue(horizontalScrollBar()->value() - (event->x() - _panStartX));
		verticalScrollBar()->setValue(verticalScrollBar()->value() - (event->y() - _panStartY));
		_panStartX = event->x();
		_panStartY = event->y();
		event->accept();
		return;
	}
	else
	{
		event->ignore();
		QGraphicsView::mouseMoveEvent(event);
	}
}

void view::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::MidButton)
	{
		_pan = true;
		_panStartX = event->x();
		_panStartY = event->y();
		setCursor(Qt::ClosedHandCursor);
		event->accept();
		return;
	}
	if(!_pan)
	{
		event->ignore();
		QGraphicsView::mousePressEvent(event);
	}

}

void view::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() == Qt::MidButton)
	{
		_pan = false;
		setCursor(Qt::ArrowCursor);
		event->accept();
		return;
	}
	if(!_pan)
	{
		event->ignore();
		QGraphicsView::mouseReleaseEvent(event);
	}
}

void view::wheelEvent(QWheelEvent *event)
{
	if(event->modifiers()==Qt::ControlModifier)
	{
		double scaleFactor = pow((double)2, event->delta() / 300.0);
		qreal factor = matrix().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
		if (factor < 0.02 || factor > 100)
			return;
		QPointF center= mapToScene(QPoint(viewport()->width() / 2.0, viewport()->height() / 2.0));
		QPointF mouse=mapToScene(event->pos());
		scale(scaleFactor, scaleFactor);
		QPointF mouse1=mapToScene(event->pos());
		QPointF delta=mouse-mouse1;
		centerOn(center+delta);
		emit scaleChanged(scaleFactor);
	}

}

void view::contextMenuEvent(QContextMenuEvent *event)
{
	QMenu menu(this);
	menu.addAction("Экспортировать выделенную область");
	menu.addAction("Профиль");
	connect(menu.actions().at(0),SIGNAL(triggered()),this,SIGNAL(export_triggered()));
	connect(menu.actions().at(1),SIGNAL(triggered()),this,SIGNAL(profile_triggered()));
	menu.exec(event->globalPos());
}


