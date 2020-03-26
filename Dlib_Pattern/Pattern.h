#pragma once
#include "global.h"
#include "Detector.h"



NS_DLIB_PATTERN_BEGIN

class Pattern {
public:
	virtual ~Pattern () {}
	virtual void DRAW (CvExt::Image& r_img,
		CvExt::Color color = CvExt::Color::White) const = 0;
	virtual Detector* GET_DETECTOR () const = 0;
	virtual const std::vector< cv::Point2f >& CORNERS () const = 0;
};

NS_DLIB_PATTERN_END