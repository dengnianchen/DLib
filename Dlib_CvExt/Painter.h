#pragma once
#include "global.h"
#include "../Dlib_Basic/ArrayRef.h"
#include "Image.h"



NS_DLIB_CVEXT_BEGIN

class Painter {
public:
	Painter (Image& r_img, float scale = 1.0f, cv::Point2f offset = { }, int shift = 2);

	Image& img () { return _r_img; }

	Painter& line (cv::Point2f pt1, cv::Point2f pt2,
		Color color, int thickness = 1);

	Painter& rectangle (cv::Point2f tl, cv::Point2f br,
		Color color, int thickness = 1);
	Painter& rectangle (cv::Rect2f rect,
		Color color, int thickness = 1);

	Painter& circle (cv::Point2f center, float radius,
		Color color, int thickness = 1);
	Painter& ellipse (cv::Point2f center, cv::Size2f axes, double angle, double startAngle, double endAngle,
		Color color, int thickness = 1);

	Painter& polyline (Basic::ConstArrayRef<cv::Point2f> R_POINTS,
		Color color, int thickness = 1, bool isConvexOrClosed = true);
	Painter& polylines (const std::vector<std::vector<cv::Point2f>>& R_POINTS_ARRAY,
		Color color, int thickness = 1, bool isConvexOrClosed = true);

	Painter& marker (cv::Point2f pt, int type, int size,
		Color color, int thickness = 1);
	Painter& markers (Basic::ConstArrayRef<cv::Point2f> R_POINTS, int type, int size,
		Color color, int thickness = 1);

	Painter& fill (Color background);

	Painter& text (const std::string& R_TEXT, cv::Point org, int fontFace, double fontScale,
		Color color, int thickness = 1, bool bottomLeftOrigin = false);

	Painter& triangle (cv::Point2f pt, float r, float angle,
		Color color, int thickness = 1);

private:
	float _scale;
	cv::Point2f _offset;
	int _shift;
	Image& _r_img;

	float _transform (float val) { return val * _scale; }

	cv::Point2f _transform (cv::Point2f p) { return p * _scale + _offset; }

	int _toShifted (float val) { return cvRound (_transform (val) * (1 << _shift)); }

	cv::Point _toShifted (cv::Point2f p) {
		p = _transform (p);
		return { _toShifted (p.x), _toShifted (p.y) };
	}

	std::vector<cv::Point> _toShifted (Basic::ConstArrayRef<cv::Point2f> val) {
		std::vector<cv::Point> val_shifted;
		for (int i = 0; i < val.SIZE (); ++i)
			val_shifted.push_back (_toShifted (val[i]));
		return val_shifted;
	}

	std::vector<std::vector<cv::Point>> _toShifted (const std::vector<std::vector<cv::Point2f>>& val) {
		std::vector<std::vector<cv::Point>> val_shifted;
		for (int i = 0; i < val.size (); ++i)
			val_shifted.push_back (_toShifted (val[i]));
		return val_shifted;
	}

};

NS_DLIB_CVEXT_END