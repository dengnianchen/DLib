#include "stdafx.h"
#include "PolyFunc.h"

using namespace Dlib::Basic;



NS_DLIB_CVEXT_BEGIN

PolyFunc::PolyFunc (ConstArrayRef<float> R_CEOFS, float error) :
CEOFS (R_CEOFS.GET_COPY ()),
MEAN_ERROR (error) {

}



float PolyFunc::operator() (float x) {
	
	return _Evaluate (CEOFS, x);

}



PolyFunc* PolyFunc::Fit (int degrees, ConstArrayRef<cv::Point2f> R_POINTS) {

	/* ********【预声明phy超定矩阵】************************/
	/* 多项式拟合的函数为多项幂函数
	 * f(x)=a0+a1*x+a2*x^2+a3*x^3+......+an*x^n
	 *a0、a1、a2......an是幂系数，也是拟合所求的未知量。设有m个抽样点，则：
	 * 超定矩阵phy=1 x1 x1^2 ... ...  x1^n
	 *           1 x2 x2^2 ... ...  x2^n
	 *           1 x3 x3^2 ... ...  x3^n
	 *              ... ... ... ...
	 *              ... ... ... ...
	 *           1 xm xm^2 ... ...  xm^n
	 *
	 * *************************************************/
	cv::Mat phy (R_POINTS.SIZE (), degrees + 1, CV_32F, cv::Scalar::all (0));
	cv::Mat y (R_POINTS.SIZE (), 1, CV_32F, cv::Scalar::all (0));
	for (int r = 0; r < phy.rows; r++) {
		float* pr = phy.ptr<float> (r);
		for (int c = 0; c < phy.cols; c++) {
			pr[c] = pow (R_POINTS[r].x, c);
		}
		y.at<float> (r) = R_POINTS[r].y;
	}
	cv::Mat a;
	cv::solve (phy, y, a, cv::DECOMP_NORMAL);
	ConstArrayRef<float> R_CEOFS ((float*)a.data, degrees + 1);
	
	// 计算RMSE
	float error = 0.0f;
	for (int i = 0; i < R_POINTS.SIZE (); ++i) {
		auto fitVal = _Evaluate (R_CEOFS, R_POINTS[i].x);
		error += std::pow (fitVal - R_POINTS[i].y, 2.0f);
	}
	error = std::sqrt (error / R_POINTS.SIZE ());

	return new PolyFunc (R_CEOFS, error);

}



float PolyFunc::_Evaluate (ConstArrayRef<float> R_CEOFS, float x) {

	float res = 0.0f;
	float xi = 1.0f;
	for (int i = 0; i < R_CEOFS.SIZE (); ++i, xi *= x)
		res += R_CEOFS[i] * xi;
	return res;

}

NS_DLIB_CVEXT_END