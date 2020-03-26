#pragma once
#include "global.h"
#include "UniformGridPattern.h"


NS_DLIB_PATTERN_BEGIN

class LineGrid : public UniformGridPattern {
public:
	LineGrid (cv::Rect2f patternRegion, cv::Size patternSize);
	virtual void DRAW (CvExt::Image& r_img,
		CvExt::Color color = CvExt::Color::White) const override;
	virtual Detector* GET_DETECTOR () const override;

};

NS_DLIB_PATTERN_END