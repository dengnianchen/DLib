#include "stdafx.h"
#include "Registration.h"

using namespace Dlib::Basic;



NS_DLIB_CVEXT_BEGIN

cv::Mat Registration::EstimateTransform (
	ConstArrayRef<cv::Point3f> R_SRC_POINTS,
	ConstArrayRef<cv::Point3f> R_TGT_POINTS) {

	CV_Assert (R_SRC_POINTS.SIZE () >= 3 && R_SRC_POINTS.SIZE () == R_TGT_POINTS.SIZE ());
	int nPoints = (int)R_SRC_POINTS.SIZE ();

	// 1. 计算两组点云的质心
	cv::Point3f cSrc, cTgt;
	for (int i = 0; i < nPoints; ++i) {
		cSrc += R_SRC_POINTS[i];
		cTgt += R_TGT_POINTS[i];
	}
	cSrc /= (float)nPoints;
	cTgt /= (float)nPoints;

	// 2. 计算两组点云相对于各自质心的坐标
	std::vector<cv::Point3f> src2C (nPoints), tgt2C (nPoints);
	for (int i = 0; i < nPoints; ++i) {
		src2C[i] = R_SRC_POINTS[i] - cSrc;
		tgt2C[i] = R_TGT_POINTS[i] - cTgt;
	}

	// 3. 计算W并进行SVD分解
	cv::Mat W (3, 3, CV_32FC1);
	for (int i = 0; i < nPoints; ++i) {
		cv::Mat p1 (3, 1, CV_32FC1, &src2C[i]);
		cv::Mat p2 (1, 3, CV_32FC1, &tgt2C[i]);
		W += p1 * p2;
	}
	cv::Mat U, S, V;
	cv::SVD::compute (W, S, U, V);

	// 4. 计算旋转矩阵R和平移向量t
	auto det = cv::determinant (U * V.t ());
	float sign = det > 0.0 ? 1.0f : det < 0.0 ? -1.0f : 0.0f;
	cv::Mat R = V * cv::Mat::diag ((cv::Mat_<float> (1, 3) << 1.0f, 1.0f, sign)) * U.t ();
	cv::Mat t = cv::Mat(3, 1, CV_32FC1, &cTgt) - R * cv::Mat (3, 1, CV_32FC1, &cSrc);

	// 5. 合成齐次变换矩阵并返回
	cv::Mat M (4, 4, CV_32FC1);
	for (int r = 0; r < 3; ++r) {
		memcpy (M.row (r).data, R.row (r).data, sizeof (float) * 3);
		M.at<float> (0, r) = t.at<float> (0, r);
	}
	M.at<float> (3, 3) = 1.0f;
	return M;

}

NS_DLIB_CVEXT_END