#include "../../Dlib_Unity/All.h"

using namespace Dlib::Basic;
using namespace Dlib::Unity;
using namespace Dlib::CvExt;



extern "C" {

UDLIB_INTERFACE (PolyFunc* PolyFunc_Fit (int degrees, ConstArrayRef<cv::Point2f> points, float& o_error)) {
	
	auto p_inst = PolyFunc::Fit (degrees, points);
	o_error = p_inst->MEAN_ERROR;
	return p_inst;
	
} END_UDLIB_INTERFACE



UDLIB_INTERFACE (void PolyFunc_delete (PolyFunc* p_inst)) {

	delete p_inst;

} END_UDLIB_INTERFACE



UDLIB_INTERFACE (float PolyFunc_eval (PolyFunc* p_inst, float x)) {

	return (*p_inst) (x);

} END_UDLIB_INTERFACE



UDLIB_INTERFACE (PolyFunc2* PolyFunc2_Fit (int degrees, ConstArrayRef<cv::Point2f> x, ConstArrayRef<float> y, float& o_error)) {

	auto p_inst = PolyFunc2::Fit (degrees, x, y);
	o_error = p_inst->MEAN_ERROR;
	return p_inst;

} END_UDLIB_INTERFACE



UDLIB_INTERFACE (void PolyFunc2_delete (PolyFunc2* p_inst)) {

	delete p_inst;

} END_UDLIB_INTERFACE



UDLIB_INTERFACE (float PolyFunc2_eval (PolyFunc2* p_inst, Vec2f x)) {

	return (*p_inst) (x);

} END_UDLIB_INTERFACE



UDLIB_INTERFACE (float solveLinear (UMatrix<float> matA, UMatrix<float> vecB, UMatrix<float> vecX, int flags)) {

	cv::Mat matA_ = matA;
	cv::Mat vecB_ = vecB;
	cv::Mat vecX_ = vecX;
	if (!cv::solve (matA_, vecB_, vecX_, flags))
		return NAN;
	cv::Mat d = matA_ * vecX_ - vecB_;
	cv::Mat d_2;
	cv::pow (d, 2.0, d_2);
	return (float)cv::sqrt (cv::mean (d_2)[0]);

} END_UDLIB_INTERFACE



UDLIB_INTERFACE (void triangulatePoints (UMatrix<float>& projMat1, UMatrix<float>& projMat2,
	UMatrix<cv::Point2f> projPoints1, UMatrix<cv::Point2f> projPoints2,
	UMatrix<float>& points4dOut, float& o_error1, float& o_error2)) {

	cv::Mat projMat1_ = projMat1; // 3 x 4
	cv::Mat projMat2_ = projMat2; // 3 x 4
	cv::Mat_<cv::Point2f> projPoints1_ = projPoints1; // 1 x n c2
	cv::Mat_<cv::Point2f> projPoints2_ = projPoints2; // 1 x n c2
	cv::Mat points4dOut_ = points4dOut; // 4 x n

	CV_Assert (projPoints1_.size () == projPoints2_.size () && projPoints1_.rows == 1 && projPoints1_.cols > 0);
	CV_Assert (points4dOut_.cols == projPoints1_.cols && points4dOut_.rows == 4);

	auto n = projPoints1_.cols;
	cv::triangulatePoints (projMat1_, projMat2_, projPoints1_, projPoints2_, points4dOut_);

	// 计算在1中的重投影误差
	cv::Mat pointsReproj1 = projMat1 * points4dOut_; // 3 x n
	o_error1 = 0.0f;
	for (int i = 0; i < n; ++i) {
		cv::Point3f p (pointsReproj1.at<float> (0, i),
			pointsReproj1.at<float> (1, i),
			pointsReproj1.at<float> (2, i));
		cv::Point2f dp = cv::Point2f (p.x / p.z, p.y / p.z) - projPoints1_ (i);
		o_error1 += dp.x * dp.x + dp.y * dp.y;
	}
	o_error1 = sqrt (o_error1 / n);

	// 计算在2中的重投影误差
	cv::Mat pointsReproj2 = projMat2 * points4dOut_; // 3 x n
	o_error2 = 0.0f;
	for (int i = 0; i < n; ++i) {
		cv::Point3f p (pointsReproj2.at<float> (0, i),
			pointsReproj2.at<float> (1, i),
			pointsReproj2.at<float> (2, i));
		cv::Point2f dp = cv::Point2f (p.x / p.z, p.y / p.z) - projPoints2_ (i);
		o_error2 += dp.x * dp.x + dp.y * dp.y;
	}
	o_error2 = sqrt (o_error2 / n);

} END_UDLIB_INTERFACE



UDLIB_INTERFACE (void drawLine (UImage img, Vec2f pt1, Vec2f pt2, Vec4b color, int thickness, int shift)) {

	Image img1 = img;
	Painter (img1, shift).line (pt1, pt2, color, thickness);

} END_UDLIB_INTERFACE



UDLIB_INTERFACE (void drawRectangle (UImage img, Vec2f tl, Vec2f br, Vec4b color, int thickness, int shift)) {

	Image img1 = img;
	Painter (img1, shift).rectangle (tl, br, color, thickness);

} END_UDLIB_INTERFACE



UDLIB_INTERFACE (void drawCircle (UImage img, Vec2f center, float radius, Vec4b color, int thickness, int shift)) {

	Image img1 = img;
	Painter (img1, shift).circle (center, radius, color, thickness);

} END_UDLIB_INTERFACE



UDLIB_INTERFACE (void drawEllipse (UImage img, Vec2f center, Vec2f axes, double angle, double startAngle, double endAngle, Vec4b color, int thickness, int shift)) {

	Image img1 = img;
	Painter (img1, shift).ellipse (center, axes, angle, startAngle, endAngle, color, thickness);

} END_UDLIB_INTERFACE



UDLIB_INTERFACE (void drawPolyline (UImage img, ConstArrayRef<cv::Point2f> points, Vec4b color, int thickness, bool isConvexOrClosed, int shift)) {

	Image img1 = img;
	Painter (img1, shift).polyline (points, color, thickness, isConvexOrClosed);

} END_UDLIB_INTERFACE



UDLIB_INTERFACE (void drawMarker (UImage img, Vec2f pt, int type, int size, Vec4b color, int thickness, int shift)) {

	Image img1 = img;
	Painter (img1, shift).marker (pt, type, size, color, thickness);

} END_UDLIB_INTERFACE



UDLIB_INTERFACE (void drawMarkers (UImage img, ConstArrayRef<cv::Point2f> pts, int type, int size, Vec4b color, int thickness, int shift)) {

	Image img1 = img;
	Painter (img1, shift).markers (pts, type, size, color, thickness);

} END_UDLIB_INTERFACE



UDLIB_INTERFACE (void drawFill (UImage img, Vec4b background, int shift)) {

	Image img1 = img;
	Painter (img1, shift).fill (background);

} END_UDLIB_INTERFACE
	


UDLIB_INTERFACE (void drawText (UImage img, const char* text, Vec2i org, int fontFace, double fontScale, Vec4b color, int thickness, bool bottomLeftOrigin, int shift)) {

	Image img1 = img;
	Painter (img1, shift).text (text, org, fontFace, fontScale, color, thickness, bottomLeftOrigin);

} END_UDLIB_INTERFACE



UDLIB_INTERFACE (void drawTriangle (UImage img, Vec2f pt, float r, float angle, Vec4b color, int thickness, int shift)) {

	Image img1 = img;
	Painter (img1, shift).triangle (pt, r, angle, color, thickness);

} END_UDLIB_INTERFACE

}