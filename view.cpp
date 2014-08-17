#include "view.h"
#include "scene.h"
#include "QMouseEvent"
#include "QScrollBar"
#include "QDebug"
view::view(QWidget *parent) : QGraphicsView(parent)
{

	//setSceneRect();
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
	event->ignore();
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
	event->ignore();
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
	event->ignore();
}

void view::wheelEvent(QWheelEvent *event)
{

	if(event->modifiers()==Qt::ControlModifier)
	{
		//setResizeAnchor(QGraphicsView::AnchorUnderMouse);
		centerOn(mapToScene(event->pos()));
		double scaleFactor = pow((double)2, event->delta() / 300.0);
		qreal factor = matrix().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
		if (factor < 0.07 || factor > 100)
			return;
		scale(scaleFactor, scaleFactor);

	}
}


