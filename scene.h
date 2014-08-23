#ifndef SCENE_H
#define SCENE_H
#include "QGraphicsScene"
#include "images.h"
class scene : public QGraphicsScene
{
public:
	scene(QObject *parent = 0);
	~scene();
	void setMainImage(images *);
	images* getMainImage();
private:
	images* mainimage;
	QPoint start;
	QGraphicsRectItem *rect;
	bool selecting;
	void mousePressEvent(QGraphicsSceneMouseEvent *);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *);
	void drawRect(QPoint,QPoint);
	/*void drawLine();
	void drawCirc();
	void drawMark();*/
};

#endif // SCENE_H
