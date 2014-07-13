#ifndef IMAGES_H
#define IMAGES_H
#include "QImage"
#include "QPixmap"
#include "opencv2/highgui/highgui.hpp"
class images
{
public:
	images(cv::Mat&);
	QPixmap getPixmap(){return pixmap;};
private:
	QImage image;
	QPixmap pixmap;
};

#endif // IMAGES_H
