#pragma once
#include "global.h"
#include "../Dlib_Basic/ArrayRef.h"



NS_DLIB_CVEXT_BEGIN

class Registration {
public:

	static cv::Mat EstimateTransform (
		Basic::ConstArrayRef<cv::Point3f> R_SRC_POINTS,
		Basic::ConstArrayRef<cv::Point3f> R_TGT_POINTS);

};

NS_DLIB_CVEXT_END