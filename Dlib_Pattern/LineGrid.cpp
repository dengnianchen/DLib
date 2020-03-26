#include "LineGrid.h"
#include "LineGridDetector.h"

using namespace Dlib::CvExt;



NS_DLIB_PATTERN_BEGIN

LineGrid::LineGrid (cv::Rect2f patternRegion, cv::Size patternSize) :
UniformGridPattern (patternRegion, patternSize) {

	// LineGrid仅支持奇数行奇数列
	assert (patternSize.width % 2 == 1 && patternSize.height % 2 == 1);

}



void LineGrid::DRAW (Image& r_img, Color color) const {

	Painter painter (r_img);

	// 用黑色背景填充图像
	painter.rectangle (cv::Point (0, 0),
		(cv::Point)(r_img.SIZE () - cv::Size (1, 1)),
		Color::Black, -1);

	int lineWidth = cvRound (std::min (X_GRIDS[1] - X_GRIDS[0], X_GRIDS[1] - X_GRIDS[0]) / 15.0f);

	// 绘制网格阵列
	for (auto x : X_GRIDS)
		painter.line ({ x, 0.0f }, { x, (float)r_img.HEIGHT () }, color, lineWidth);
	for (auto y : Y_GRIDS)
		painter.line ({ 0.0f, y }, { (float)r_img.WIDTH (), y }, color, lineWidth);
	cv::Point2f centerPoint (
		X_GRIDS[N_COLS / 2],
		Y_GRIDS[N_ROWS / 2]
	);
	painter.circle (centerPoint, 5.0f * lineWidth, Color::Black, -1)
		.circle (centerPoint, 1.5f * lineWidth, color, -1);

}



Detector* LineGrid::GET_DETECTOR () const {

	return new LineGridDetector (*this);

}

NS_DLIB_PATTERN_END