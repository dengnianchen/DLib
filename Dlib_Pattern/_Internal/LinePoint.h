#pragma once
#include "../global.h"



NS_DLIB_PATTERN_BEGIN namespace _Internal {

	class Port : public cv::Point2f {
	public:
		float width;

		Port () : width () { }
		Port (const cv::Point2f& R_POINT, float width) :
			cv::Point2f (R_POINT), width (width) { }
		bool IS_VALID () const { return width > 1e-5f; }

	};



	class LinePoint : public Port {
	public:
		enum Type { Unknown, Guess, TEnd = 4, Normal = 12, Endpoint = 8 };

		Port in, out;
		Type type;

		LinePoint () = default;
		LinePoint (const Port& R_CENTER, const Port& R_IN,
			const Port& R_OUT, Type type = Normal) :
			Port (R_CENTER), out (R_OUT), in (R_IN), type (type) { }

		LinePoint& operator= (const Port& R_CENTER) {
			x = R_CENTER.x;
			y = R_CENTER.y;
			width = R_CENTER.width;
			return *this;
		}

		CvExt::Line2D LINE_IN () const { return { in, *this }; }

		CvExt::Line2D LINE_OUT () const { return { *this, out }; }

		LinePoint REVERSE () const { return { *this, out, in, type }; }

		bool HAS_OUT () const { return out.IS_VALID (); }

		bool HAS_IN () const { return in.IS_VALID (); }

	};



} NS_DLIB_PATTERN_END