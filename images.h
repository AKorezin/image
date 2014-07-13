#ifndef IMAGES_H
#define IMAGES_H
#include "QImage"
#include "QPixmap"
#include "opencv2/highgui/highgui.hpp"
class images
{
public:
	images(cv::Mat&);
	QPixmap getPixmap();
	cv::Mat getCvMat(void);
private:
	cv::Mat matrix;
	QImage image;
	QPixmap pixmap;
};

#endif // IMAGES_H
