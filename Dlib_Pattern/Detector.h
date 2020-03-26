#pragma once
#include "global.h"
#include "../Dlib_Basic/ArrayRef.h"



NS_DLIB_PATTERN_BEGIN

class Detector {
public:
	virtual ~Detector () {}
	virtual bool run (const CvExt::Image& R_IMG,
		Basic::ArrayRef<cv::Point2f> r_resultsOut,
		CvExt::Image drawLayer, bool isDebug = false) = 0;

};

NS_DLIB_PATTERN_END