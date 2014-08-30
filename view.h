#ifndef VIEW_H
#define VIEW_H
#include "QGraphicsView"
#include <QObject>

class view : public QGraphicsView
{
	Q_OBJECT
public:
	view(QWidget *parent = 0);
signals:
	void export_triggered();
private:

	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent* event);
	void contextMenuEvent(QContextMenuEvent *);
	bool _pan;
	int _panStartX, _panStartY;
};

#endif // VIEW_H
