#include "stdafx.h"
#include "Color.h"
#include "PixelOp.h"



NS_DLIB_CVEXT_BEGIN

const Color Color::White = Color::BGR (255, 255, 255);
const Color Color::Black = Color::BGR (0, 0, 0);
const Color Color::Red = Color::BGR (0, 0, 255);
const Color Color::Green = Color::BGR (0, 255, 0);
const Color Color::Blue = Color::BGR (255, 0, 0);
const Color Color::Yellow = Color::BGR (0, 255, 255);
const Color Color::Purple = Color::BGR (255, 0, 255);
const Color Color::Cyan = Color::BGR (255, 255, 0);
const Color Color::Zero = Color::BGR (0, 0, 0, 0);



Color Color::operator() (EColorSpace colorSpace) const {

	if (_colorSpace == colorSpace)
		return *this;
	PixelFormat srcFormat (_colorSpace), dstFormat (colorSpace);
	cv::Mat src (1, 1, srcFormat.CV_TYPE (), *this);
	cv::Mat dst (1, 1, dstFormat.CV_TYPE ());
	PixelFormat::CvtColor (src, dst, srcFormat, dstFormat);
	return { GetPixel (dst, 0), colorSpace };

}

NS_DLIB_CVEXT_END