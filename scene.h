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
	cv::Mat getSelected();
public slots:
	void exportcurrent();
private:
	images* mainimage;
	QPoint start;
	int tool;
	bool selecting;
	QGraphicsRectItem *rect;
	QGraphicsEllipseItem *ellipse;
	QGraphicsLineItem *line;
	QList<QGraphicsRectItem *> handles;


	void mousePressEvent(QGraphicsSceneMouseEvent *);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *);
	void prepareItem();
	void drawRect(QPoint,QPoint);
	void drawEllipse(QPoint,QPoint);
	void drawLine(QPoint,QPoint);
	void drawMarker(QPoint);
};

#endif // SCENE_H
