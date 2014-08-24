#ifndef IMAGES_H
#define IMAGES_H
#include "QImage"
#include "QPixmap"
#include "QGraphicsRectItem"
#include "opencv2/highgui/highgui.hpp"
class images
{
public:
	images(cv::Mat&);
	QPixmap getPixmap();
	cv::Mat getCvMat(void);
	cv::Mat ellipseCrop(QGraphicsRectItem *);
private:
	cv::Mat matrix;
	QImage image;
	QPixmap pixmap;
};

#endif // IMAGES_H
