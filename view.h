#ifndef VIEW_H
#define VIEW_H
#include "QGraphicsView"

class view : public QGraphicsView
{
public:
	view(QWidget *parent = 0);
private:
	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent* event);

	bool _pan;
	int _panStartX, _panStartY;
};

#endif // VIEW_H
