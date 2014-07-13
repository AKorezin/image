#include "images.h"
#include "QImage"
#include "QDebug"
images::images(cv::Mat &mat)
{
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
