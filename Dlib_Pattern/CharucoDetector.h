#pragma once
#include "global.h"
#include "Detector.h"
#include "Charuco.h"



NS_DLIB_PATTERN_BEGIN

class CharucoDetector : public Detector {
public:
	CharucoDetector (const Charuco& R_PATTERN);

	virtual bool run (const CvExt::Image& R_IMG,
		Basic::ArrayRef<cv::Point2f> r_resultsOut,
		CvExt::Image drawLayer, bool isDebug = false) override;

private:
	const Charuco& _R_PATTERN;

};

NS_DLIB_PATTERN_END