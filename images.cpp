#include "images.h"
#include "QImage"
#include "QGraphicsRectItem"
#include "QDebug"
images::images(cv::Mat &mat)
{
	matrix=mat;
	switch ( mat.type() )
	{
	case CV_8UC4:
	{
		image=QImage( mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB32 );
		break;
	}
	case CV_8UC3:
	{
		image=QImage( mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888 );
		break;
	}
	case CV_8UC1:
	{
		static QVector<QRgb>  sColorTable;
		if ( sColorTable.isEmpty() )
		{
			for ( int i = 0; i < 256; ++i )
				sColorTable.push_back( qRgb( i, i, i ) );
		}

		image=QImage( mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8 );

		image.setColorTable( sColorTable );

		break;
	}

	default:
		qDebug() << "ASM::cvMatToQImage() - cv::Mat image type not handled in switch:" << mat.type();
		break;
	}
	pixmap=QPixmap::fromImage(image);
}
QPixmap images::getPixmap()
{
	return pixmap;
}
cv::Mat images::getCvMat(void)
{
	return matrix;
}

cv::Mat images::ellipseCrop(QGraphicsRectItem * rect)
{
	cv::Rect myRoi(rect->rect().x(),rect->rect().y(),rect->rect().width(),rect->rect().height());
	cv::Mat croppedImage;
	cv::Point center(rect->rect().x()+rect->rect().width()/2,rect->rect().y()+rect->rect().height()/2);
	cv::Size size(rect->rect().width()/2,rect->rect().height()/2);
	cv::Mat im1(matrix.rows, matrix.cols, CV_8UC1, cv::Scalar(255,255,255));
	cv::Mat im2(matrix.rows, matrix.cols, CV_8UC1, cv::Scalar(0,0,0));
	cv::ellipse( im2, center, size, 0, 0, 360, cv::Scalar( 255, 255, 255), -1, 8 );
	cv::ellipse( im1, center, size, 0, 0, 360, cv::Scalar( 0, 0, 0), -1, 8 );
	cv::bitwise_and(matrix,im2,croppedImage);
	cv::bitwise_xor(croppedImage,im1,croppedImage);
	return croppedImage(myRoi);
}
