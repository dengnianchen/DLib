#pragma once
#include "global.h"
#include <QImage>
#include <opencv2/opencv.hpp>
#include "../Dlib_Basic/Exception.h"



NS_DLIB_CVT_BEGIN

inline QImage toQImage (const cv::Mat& R_CV_IMG) {
	if (R_CV_IMG.depth () != CV_8U)
		return QImage ();
	switch (R_CV_IMG.channels ()) {
	case 1:
	{
		QImage qtImg (R_CV_IMG.cols, R_CV_IMG.rows, QImage::Format_Indexed8);
		for (int r = 0; r < R_CV_IMG.rows; ++r)
			memcpy (qtImg.scanLine (r), R_CV_IMG.row (r).data, R_CV_IMG.cols);
		QVector< uint > grayTable (256);
		for (int i = 0; i < grayTable.size (); ++i)
			grayTable[i] = qRgb (i, i, i);
		qtImg.setColorTable (grayTable);
		return qtImg;
	}
	case 3:
	{
		QImage qtImg (R_CV_IMG.cols, R_CV_IMG.rows, QImage::Format_RGB888);
		for (int r = 0; r < R_CV_IMG.rows; ++r) {
			auto dstRow = (unsigned char*)qtImg.scanLine (r);
			auto srcRow = R_CV_IMG.row (r);
			for (int c = 0; c < R_CV_IMG.cols; ++c) {
				auto srcPixel = srcRow.at<cv::Vec3b> (c);
				dstRow[c * 3] = srcPixel[2];
				dstRow[c * 3 + 1] = srcPixel[1];
				dstRow[c * 3 + 2] = srcPixel[0];
			}
		}
		return qtImg;
	}
	case 4:
	{
		QImage qtImg (R_CV_IMG.cols, R_CV_IMG.rows, QImage::Format_ARGB32);
		for (int r = 0; r < R_CV_IMG.rows; ++r) {
			auto dstRow = (unsigned int*)qtImg.scanLine (r);
			auto srcRow = R_CV_IMG.row (r);
			for (int c = 0; c < R_CV_IMG.cols; ++c) {
				auto srcPixel = srcRow.at<cv::Vec4b> (c);
				dstRow[c] = qRgba (srcPixel[3], srcPixel[2], srcPixel[1],
					srcPixel[0]);
			}
		}
		return qtImg;
	}
	default:
		return QImage ();
	}
}



inline cv::Mat toCvImage (const QImage& R_QIMG)
throw (Basic::Exception::InvalidArgument) {
	if (R_QIMG.format () == QImage::Format_RGB888) {
		cv::Mat cvImg (R_QIMG.height (), R_QIMG.width (), CV_8UC3);
		for (int r = 0; r < cvImg.rows; ++r) {
			auto srcRow = R_QIMG.constScanLine (r);
			auto dstRow = cvImg.row (r);
			for (int c = 0; c < cvImg.cols; ++c) {
				dstRow.at< cv::Vec3b > (c) = { srcRow[c * 3 + 2], srcRow[c * 3 + 1], srcRow[c * 3] };
			}
		}
		return cvImg;
	}
	if (R_QIMG.format () == QImage::Format_ARGB32 || R_QIMG.format () == QImage::Format_RGB32) {
		cv::Mat cvImg (R_QIMG.height (), R_QIMG.width (), CV_8UC4);
		for (int r = 0; r < cvImg.rows; ++r) {
			auto srcRow = R_QIMG.constScanLine (r);
			auto dstRow = cvImg.row (r);
			for (int c = 0; c < cvImg.cols; ++c) {
				dstRow.at< cv::Vec4b > (c) = {
					srcRow[c * 4 + 3], srcRow[c * 4 + 2],
					srcRow[c * 4 + 1], srcRow[c * 4]
				};
			}
		}
		return cvImg;
	}
	if (R_QIMG.format () == QImage::Format_Indexed8) {
		cv::Mat cvImg (R_QIMG.height (), R_QIMG.width (), CV_8UC1);
		for (int r = 0; r < cvImg.rows; ++r) {
			auto srcRow = R_QIMG.constScanLine (r);
			auto dstRow = cvImg.row (r);
			memcpy (&dstRow.at< uchar > (0), srcRow, cvImg.cols);
			for (int c = 0; c < cvImg.cols; ++c) {
				dstRow.at< cv::Vec4b > (c) = {
					srcRow[c * 4 + 3], srcRow[c * 4 + 2],
					srcRow[c * 4 + 1], srcRow[c * 4]
				};
			}
		}
		return cvImg;
	}
	throw Basic::Exception::InvalidArgument (L"Format of input image does not supported yet");
}



inline cv::Size toCvSize (const QSize& R_QSIZE) { return { R_QSIZE.width (), R_QSIZE.height () }; }
inline QSize toQSize (const cv::Size& R_CV_SIZE) { return { R_CV_SIZE.width, R_CV_SIZE.height }; }

inline cv::Point toCvPoint (const QPoint& R_QPOINT) { return { R_QPOINT.x (), R_QPOINT.y () }; }
inline cv::Point2f toCvPointf (const QPointF& R_QPOINT) { cv::Point2f (R_QPOINT.x (), R_QPOINT.y ()); }
inline cv::Point2d toCvPointd (const QPointF& R_QPOINT) { return { R_QPOINT.x (), R_QPOINT.y () }; }
inline QPoint toQPoint (const cv::Point& R_CV_POINT) { return { R_CV_POINT.x, R_CV_POINT.y }; }
inline QPointF toQPoint (const cv::Point2f& R_CV_POINT) { return { R_CV_POINT.x, R_CV_POINT.y }; }
inline QPointF toQPoint (const cv::Point2d& R_CV_POINT) { return { R_CV_POINT.x, R_CV_POINT.y }; }

NS_DLIB_CVT_END