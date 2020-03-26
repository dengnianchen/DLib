#pragma once
#include "global.h"
#include "CvPointEx.h"



NS_DLIB_CVEXT_BEGIN

class Line2D {
public:
	Line2D (cv::Point2f p1 = { }, cv::Point2f p2 = { }) :
		_p1 (p1), _p2 (p2), _ceof (NAN), _length (NAN) { }

	const cv::Point3f& CEOF () const {
		return _ceof = isnan (_ceof.x) ?
			cv::Point3f (_p1.x, _p1.y, 1).cross ({ _p2.x, _p2.y, 1 }) :
			_ceof;
	}

	float LENGTH () const {
		return _length = isnan (_length) ? magnitude (_p2 - _p1) : _length;
	}

	cv::Point2f V () const { return _p2 - _p1; }

	cv::Point2f V0 () const { return V () / LENGTH (); }

	bool NEAR_HORIZONTAL () const {
		return std::abs (_p2.x - _p1.x) > std::abs (_p2.y - _p1.y);
	}

	float T (cv::Point2f pointOnLine) const {
		return NEAR_HORIZONTAL () ?
			(pointOnLine.x - _p1.x) / V ().x :
			(pointOnLine.y - _p1.y) / V ().y;
	}

	float DISTANCE (cv::Point2f p, bool limitSegment = false) const;
	
	cv::Point2f NEAREST_POINT_ON_LINE (cv::Point2f p, float& o_distance, bool limitSegment = false) const;

	Line2D operator- () const { return { _p2, _p1 }; }

	static cv::Point2f Intersect (const Line2D& R_LINE1, const Line2D& R_LINE2);

	static cv::Point2f Intersect (const Line2D& R_LINE1, const Line2D& R_LINE2, float& o_t1, float& o_t2);

private:
	cv::Point2f _p1, _p2;
	mutable cv::Point3f _ceof;
	mutable float _length;

};

NS_DLIB_CVEXT_END
