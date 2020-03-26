#pragma once
#include "global.h"
#include "../Dlib_Basic/ArrayRef.h"



NS_DLIB_CVEXT_BEGIN

/* f(x)=a0+a1*x+a2*x^2+a3*x^3+......+an*x^n */
class PolyFunc {
public:
	const float MEAN_ERROR;
	const std::vector<float> CEOFS;

	PolyFunc (Basic::ConstArrayRef<float> R_CEOFS = {}, float error = 0.0f);
	float operator() (float x);

	static PolyFunc* Fit (int degrees, Basic::ConstArrayRef<cv::Point2f> R_POINTS);

private:
	static float _Evaluate (Basic::ConstArrayRef<float> R_CEOFS, float x);

};

NS_DLIB_CVEXT_END