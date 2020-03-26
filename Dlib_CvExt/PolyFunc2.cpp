#include "stdafx.h"
#include "PolyFunc2.h"

using namespace Dlib::Basic;



NS_DLIB_CVEXT_BEGIN

PolyFunc2::PolyFunc2 (ConstArrayRef<float> R_CEOFS, float error) :
CEOFS (R_CEOFS.GET_COPY ()),
MEAN_ERROR (error) {
}



float PolyFunc2::operator() (cv::Point2f x) {

	return _Evaluate (CEOFS, x);

}



PolyFunc2* PolyFunc2::Fit (int degrees, ConstArrayRef<cv::Point2f> R_X,
	ConstArrayRef<float> R_Y) {

	assert (R_X.SIZE () == R_Y.SIZE () && R_X.SIZE () >= GetItemCount (degrees));
	int n = (int)R_X.SIZE ();

	/* ********【预声明phy超定矩阵】************************/
	/* 多项式拟合的函数为多项幂函数
	 * f(x)=a0*x^2 + a1*y^2 + a2*xy + a3*x + a4*y + a5
	 *
	 * 超定矩阵phy=x1^2 y1^2 x1y1 x1 y1 1
	 *            x2^2 y2^2 x2y2 x2 y2 1
	 *              ... ... ... ...
	 *              ... ... ... ...
	 *            xn^2 yn^2 xnyn xn yn 1
	 *
	 * *************************************************/
	cv::Mat phy (n, GetItemCount (degrees), CV_32F, cv::Scalar::all (0));
	cv::Mat y (n, 1, CV_32F, cv::Scalar::all (0));
	for (int i = 0; i < n; i++) {
		float* pr = phy.ptr<float> (i);
		auto items = _GetItems (degrees, R_X[i]);
		memcpy (pr, &items[0], sizeof (float) * items.size ());
		y.at<float> (i) = R_Y[i];
	}
	cv::Mat a;
	cv::solve (phy, y, a, cv::DECOMP_NORMAL);
	ConstArrayRef<float> R_CEOFS ((float*)a.data, a.rows);

	// 计算RMSE
	float error = 0.0f;
	for (int i = 0; i < n; ++i) {
		auto fitVal = _Evaluate (R_CEOFS, R_X[i]);
		error += std::pow (fitVal - R_Y[i], 2.0f);
	}
	error = std::sqrt (error / n);

	return new PolyFunc2 (R_CEOFS, 0.0f);

}



int PolyFunc2::GetItemCount (int degrees) {

	// 对常用低阶进行硬编码以加速
	switch (degrees) {
	case 0: return 1;
	case 1: return 3;
	case 2: return 6;
	case 3: return 10;
	case 4: return 15;
	}
	// 其他高阶推导
	int n = 0;
	for (int i = 0; i <= degrees; ++i)
		n += i + 1;
	return n;

}



int PolyFunc2::GetDegrees (int itemCount) {
	
	// 对常用低阶进行硬编码以加速
	switch (itemCount) {
	case 1: return 0;
	case 3: return 1;
	case 6: return 2;
	case 10: return 3;
	case 15: return 4;
	}
	if (itemCount < 15)
		throw std::invalid_argument ("item count is invalid");

	// 其他高阶推导
	int n = 0, i = 0;
	for (; n < itemCount; ++i)
		n += i + 1;
	if (n != itemCount)
		throw std::invalid_argument ("item count is invalid");
	return i;

}



std::vector<float> PolyFunc2::_GetItems (int degrees, cv::Point2f val) {

	// 对常用低阶进行硬编码以加速
	switch (degrees) {
	case 0: return { 1.0f };
	case 1: return { val.x, val.y, 1.0f };
	case 2: return { val.x * val.x, val.x * val.y, val.y * val.y, val.x, val.y, 1.0f };
	}

	// 预计算x和y的所有阶数（0~degrees）的值
	auto x = new float[degrees + 1]; x[0] = 1.0f;
	auto y = new float[degrees + 1]; y[0] = 1.0f;
	for (int d = 1; d <= degrees; ++d) {
		x[d] = x[d - 1] * val.x;
		y[d] = y[d - 1] * val.y;
	}

	// 从高阶开始进行组合计算
	std::vector<float> items (GetItemCount (degrees));
	for (int d = degrees, i = 0; d >= 0; --d) {
		for (int dx = d; dx >= 0; --dx, ++i)
			items[i] = x[dx] * y[d - dx];
	}

	delete[] x;
	delete[] y;
	return items;

}



float PolyFunc2::_Evaluate (ConstArrayRef<float> R_CEOFS, cv::Point2f x) {

	auto items = _GetItems (GetDegrees (R_CEOFS.SIZE ()), x);
	float val = 0.0f;
	for (int i = 0; i < items.size (); ++i)
		val += R_CEOFS[i] * items[i];
	return val;

}

NS_DLIB_CVEXT_END