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
	void mousePressEvent(QGraphicsSceneMouseEvent *);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *);
};

#endif // SCENE_H
