#include "pch.h"
#include "UImage.h"

using namespace Dlib::CvExt;



NS_DLIB_UNITY_BEGIN

UImage& UImage::operator= (const CvExt::Image& R_IMAGE) {

	format = R_IMAGE.FORMAT ();
	size = R_IMAGE.SIZE ();
	data = R_IMAGE->data;
	return *this;

}



UImage& UImage::operator<< (const Image& R_IMAGE) {

	if (size.x * size.y == 0)
		return *this;
	cv::Mat srcImage;
	if ((cv::Size)size != R_IMAGE.SIZE ())
		cv::resize (R_IMAGE, srcImage, size);
	else
		srcImage = R_IMAGE;
	cv::Mat dstImage = *this;
	if (R_IMAGE.FORMAT () != format)
		PixelFormat::CvtColor (srcImage, dstImage, R_IMAGE.FORMAT (), format);
	else
		srcImage.copyTo (dstImage);
	return *this;

}



UImage::operator Image () {

	return Image (format, cv::Mat (size, format.CV_TYPE (), data));

}



UImage::operator cv::Mat () {

	return cv::Mat (size, format.CV_TYPE (), data);

}

NS_DLIB_UNITY_END

