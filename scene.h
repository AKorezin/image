#ifndef SCENE_H
#define SCENE_H
#include "QGraphicsScene"
#include "images.h"

class scene : public QGraphicsScene
{
	Q_OBJECT
public:
	scene(QObject *parent = 0);
	~scene();
	void setMainImage(images *);
	images* getMainImage();
	cv::Mat getSelected();
	void exportcurrent();
	QList<int> getProfile();
public slots:
	void setScale(double);
private:
	images* mainimage;
	QPoint start;
	int tool;
	double handlew,handleh,penwidth;
	bool selecting,translate;
	QGraphicsRectItem *rect;
	QGraphicsEllipseItem *ellipse;
	QGraphicsLineItem *line;
	QList<QGraphicsRectItem *> handles;


	void mousePressEvent(QGraphicsSceneMouseEvent *);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *);
	void prepareItem(QPoint);
	void prepareMove(QPoint);
	void drawRect(QPoint,QPoint);
	void drawEllipse(QPoint,QPoint);
	void drawLine(QPoint,QPoint);
	void drawMarker(QPoint);

};

#endif // SCENE_H
