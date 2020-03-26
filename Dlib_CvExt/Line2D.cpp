#include "stdafx.h"
#include "Line2D.h"



NS_DLIB_CVEXT_BEGIN

float Line2D::DISTANCE (cv::Point2f p, bool limitSegment) const {

	float distance;
	NEAREST_POINT_ON_LINE (p, distance, limitSegment);
	return distance;

}



cv::Point2f Line2D::NEAREST_POINT_ON_LINE (cv::Point2f p, float& o_distance, bool limitSegment) const {
	
	if (std::abs (p.x - _p1.x) < 1e-5f && std::abs (p.y - _p1.y) < 1e-5f) {
		o_distance = 0.0f;
		return _p1;
	}
	auto v1 = p - _p1;
	auto v = V ();
	auto t = v1.dot (v) / v.dot (v);
	if (limitSegment)
		t = std::min (std::max (t, 0.0f), 1.0f);
	auto nearestPointOnLine = _p1 + v * t;
	o_distance = magnitude (p - nearestPointOnLine);
	return nearestPointOnLine;

}



cv::Point2f Line2D::Intersect (const Line2D& R_LINE1, const Line2D& R_LINE2) {

	auto cross3 = R_LINE1.CEOF ().cross (R_LINE2.CEOF ());
	return { cross3.x / cross3.z, cross3.y / cross3.z };

}



cv::Point2f Line2D::Intersect (const Line2D& R_LINE1, const Line2D& R_LINE2, float& o_t1, float& o_t2) {
	
	auto p = Intersect (R_LINE1, R_LINE2);
	o_t1 = R_LINE1.T (p);
	o_t2 = R_LINE2.T (p);
	return p;

}

NS_DLIB_CVEXT_END

