#pragma once
#include "global.h"
#include "../Dlib_Basic/ArrayRef.h"



NS_DLIB_CVEXT_BEGIN

class PolyFunc2 {
public:
	const float MEAN_ERROR;
	const std::vector<float> CEOFS;

	PolyFunc2 (Basic::ConstArrayRef<float> R_CEOFS, float error = 0.0f);
	float operator() (cv::Point2f x);

	static PolyFunc2* Fit (int degrees,
		Basic::ConstArrayRef<cv::Point2f> R_X,
		Basic::ConstArrayRef<float> R_Y);
	static int GetItemCount (int degrees);
	static int GetDegrees (int itemCount);

private:
	static std::vector<float> _GetItems (int degrees, cv::Point2f val);
	static float _Evaluate (Basic::ConstArrayRef<float> R_CEOFS, cv::Point2f x);

};

NS_DLIB_CVEXT_END