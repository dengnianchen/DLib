#include "stdafx.h"
#include "Painter.h"

using namespace Dlib::Basic;



NS_DLIB_CVEXT_BEGIN

Painter::Painter (Image& r_img, float scale, cv::Point2f offset, int shift) :
_r_img (r_img),
_scale (scale),
_offset (offset),
_shift (shift) {

}



Painter& Painter::line (cv::Point2f pt1, cv::Point2f pt2, Color color, int thickness) {

	color = color (_r_img.COLOR_SPACE ());
	cv::line (_r_img, _toShifted (pt1), _toShifted (pt2), color, thickness, cv::LINE_AA, _shift);
	return *this;

}



Painter& Painter::rectangle (cv::Point2f tl, cv::Point2f br, Color color, int thickness) {

	color = color (_r_img.COLOR_SPACE ());
	cv::rectangle (_r_img, _toShifted (tl), _toShifted (br), color, thickness, cv::LINE_AA, _shift);
	return *this;

}



Painter& Painter::rectangle (cv::Rect2f rect, Color color, int thickness) {

	color = color (_r_img.COLOR_SPACE ());
	rectangle (rect.tl (), rect.br (), color, thickness);
	return *this;

}



Painter& Painter::circle (cv::Point2f center, float radius, Color color, int thickness) {

	color = color (_r_img.COLOR_SPACE ());
	cv::circle (_r_img, _toShifted (center), _toShifted (radius), color, thickness, cv::LINE_AA, _shift);
	return *this;

}



Painter& Painter::ellipse (cv::Point2f center, cv::Size2f axes, double angle, double startAngle, double endAngle, Color color, int thickness) {

	color = color (_r_img.COLOR_SPACE ());
	cv::ellipse (_r_img, _toShifted (center), _toShifted (axes), angle, startAngle, endAngle, color, thickness, cv::LINE_AA, _shift);
	return *this;

}



Painter& Painter::polyline (Basic::ConstArrayRef<cv::Point2f> R_POINTS, Color color, int thickness, bool isConvexOrClosed) {

	color = color (_r_img.COLOR_SPACE ());
	if (thickness >= 0)
		cv::polylines (_r_img, std::vector<std::vector<cv::Point>> ({ _toShifted (R_POINTS) }), isConvexOrClosed, color, thickness, cv::LINE_AA, _shift);
	else if (isConvexOrClosed)
		cv::fillConvexPoly (_r_img, _toShifted (R_POINTS), color, cv::LINE_AA, _shift);
	else
		cv::fillPoly (_r_img, std::vector<std::vector<cv::Point>> ({ _toShifted (R_POINTS) }), color, cv::LINE_AA, _shift);
	return *this;

}


Painter& Painter::polylines (const std::vector<std::vector<cv::Point2f>>& R_POINTS_ARRAY, Color color, int thickness, bool isConvexOrClosed) {

	color = color (_r_img.COLOR_SPACE ());
	if (thickness >= 0)
		cv::polylines (_r_img, _toShifted (R_POINTS_ARRAY), isConvexOrClosed, color, thickness, cv::LINE_AA, _shift);
	else if (isConvexOrClosed)
		for (auto i = 0; i < R_POINTS_ARRAY.size (); ++i)
			cv::fillConvexPoly (_r_img, _toShifted (R_POINTS_ARRAY[i]), color, cv::LINE_AA, _shift);
	else
		cv::fillPoly (_r_img, _toShifted (R_POINTS_ARRAY), color, cv::LINE_AA, _shift);
	return *this;

}



Painter& Painter::marker (cv::Point2f pt, int type, int size, Color color, int thickness) {

	color = color (_r_img.COLOR_SPACE ());
	if (isnan (pt.x) || isnan (pt.y)) return *this;
	cv::drawMarker (_r_img, _transform (pt), color, type, size, thickness, cv::LINE_AA);
	return *this;

}



Painter& Painter::markers (ConstArrayRef<cv::Point2f> pts, int type, int size, Color color, int thickness) {

	color = color (_r_img.COLOR_SPACE ());
	for (int i = 0; i < pts.SIZE (); ++i)
		marker (pts[i], type, size, color, thickness);
	return *this;

}



Painter& Painter::fill (Color background) {

	background = background (_r_img.COLOR_SPACE ());
	cv::rectangle (_r_img, { 0, 0, _r_img.WIDTH (), _r_img.HEIGHT () }, background, -1);
	return *this;

}



Painter& Painter::text (const std::string& R_TEXT, cv::Point org, int fontFace, double fontScale, Color color, int thickness, bool bottomLeftOrigin) {

	color = color (_r_img.COLOR_SPACE ());
	cv::putText (_r_img, R_TEXT, (cv::Point)_transform (org), fontFace, fontScale, color, thickness, cv::LINE_AA, bottomLeftOrigin);
	return *this;

}



Painter& Painter::triangle (cv::Point2f pt, float r, float angle,
	Color color, int thickness) {

	color = color (_r_img.COLOR_SPACE ());
	std::vector<cv::Point2f> vertices (3);
	for (int i = 0; i < 3; ++i) {
		vertices[i].x = pt.x + r * std::cos ((angle + 120.0f * i) / 180.0f * 3.14159f);
		vertices[i].y = pt.y + r * std::sin ((angle + 120.0f * i) / 180.0f * 3.14159f);
	}
	polyline (vertices, color, thickness);
	return *this;

}

NS_DLIB_CVEXT_END