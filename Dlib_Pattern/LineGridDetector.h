#pragma once
#include "global.h"	
#include "Detector.h"
#include "LineGrid.h"



NS_DLIB_PATTERN_BEGIN

class LineGridDetector : public Detector {
public:
	LineGridDetector (const LineGrid& R_PATTERN);
	~LineGridDetector ();
	virtual bool run (const CvExt::Image& R_IMG,
		Basic::ArrayRef<cv::Point2f> r_resultsOut,
		CvExt::Image drawLayer, bool isDebug = false) override;

private:
	void* _p_private;

};

NS_DLIB_PATTERN_END