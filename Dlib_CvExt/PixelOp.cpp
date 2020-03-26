#include "stdafx.h"
#include "PixelOp.h"



NS_DLIB_CVEXT_BEGIN

cv::Scalar GetPixel (const cv::Mat& R_IMAGE, cv::Point p) {

	switch (R_IMAGE.type ()) {
	case CV_8UC1: return R_IMAGE.at<uchar> (p); break;
	case CV_8UC2: return R_IMAGE.at<cv::Vec2b> (p); break;
	case CV_8UC3: return R_IMAGE.at<cv::Vec3b> (p); break;
	case CV_8UC4: return R_IMAGE.at<cv::Vec4b> (p); break;
	case CV_32FC1: return R_IMAGE.at<double> (p); break;
	case CV_32FC2: return R_IMAGE.at<cv::Vec2f> (p); break;
	case CV_32FC3: return R_IMAGE.at<cv::Vec3f> (p); break;
	case CV_32FC4: return R_IMAGE.at<cv::Vec4f> (p); break;
	default: throw Basic::Exception::InvalidArgument (L"OpenCV type is unsupported yet");
	}

}



cv::Scalar GetPixel (const cv::Mat& R_IMAGE, int x, int y) {

	return GetPixel (R_IMAGE, { x, y });

}



cv::Scalar GetPixel (const cv::Mat& R_IMAGE, int i) {

	return GetPixel (R_IMAGE, { 0, i });

}



void SetPixel (cv::Mat& r_image, cv::Point p, const cv::Scalar& R_VALUE) {

	switch (r_image.type ()) {
	case CV_8UC1: r_image.at<uchar> (p) = (uchar)R_VALUE[0]; break;
	case CV_8UC2: r_image.at<cv::Vec2b> (p) = { (uchar)R_VALUE[0], (uchar)R_VALUE[1] }; break;
	case CV_8UC3: r_image.at<cv::Vec3b> (p) = { (uchar)R_VALUE[0], (uchar)R_VALUE[1], (uchar)R_VALUE[2] }; break;
	case CV_8UC4: r_image.at<cv::Vec4b> (p) = { (uchar)R_VALUE[0], (uchar)R_VALUE[1], (uchar)R_VALUE[2], (uchar)R_VALUE[3] }; break;
	case CV_32FC1: r_image.at<float> (p) = (float)R_VALUE[0]; break;
	case CV_32FC2: r_image.at<cv::Vec2f> (p) = { (float)R_VALUE[0], (float)R_VALUE[1] }; break;
	case CV_32FC3: r_image.at<cv::Vec3f> (p) = { (float)R_VALUE[0], (float)R_VALUE[1], (float)R_VALUE[2] }; break;
	case CV_32FC4: r_image.at<cv::Vec4f> (p) = { (float)R_VALUE[0], (float)R_VALUE[1], (float)R_VALUE[2], (float)R_VALUE[3] }; break;
	default: throw Basic::Exception::InvalidArgument (L"OpenCV type is unsupported yet");
	}

}



void SetPixel (cv::Mat& r_image, int x, int y, const cv::Scalar& R_VALUE) {

	SetPixel (r_image, { x, y }, R_VALUE);

}



void SetPixel (cv::Mat& r_image, int i, const cv::Scalar& R_VALUE) {

	SetPixel (r_image, { 0, i }, R_VALUE);

}

NS_DLIB_CVEXT_END