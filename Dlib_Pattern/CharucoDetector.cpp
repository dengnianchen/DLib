#include "CharucoDetector.h"

using namespace Dlib::CvExt;
using namespace Dlib::Basic;



NS_DLIB_PATTERN_BEGIN

CharucoDetector::CharucoDetector (const Charuco& R_PATTERN) :
_R_PATTERN (R_PATTERN) {
}



bool CharucoDetector::run (const Image& R_IMG, ArrayRef<cv::Point2f> r_resultsOut,
	Image drawLayer, bool isDebug) {

	/* 二维码标记序号，棋盘格角点序号 */
	std::vector< int > markerIds, charucoIds;
	/* 二维码标记角点坐标 */
	std::vector< std::vector< cv::Point2f > > markerCorners;
	/* 棋盘格角点坐标 */
	std::vector< cv::Point2f > charucoCorners;

	// 0. 清空结果数组中的所有值（设为NaN）
	for (int i = 0; i < _R_PATTERN.N_ALL_CORNERS; ++i)
		r_resultsOut[i] = { NAN, NAN };

	// 1. 在画面中检测二维码标记
	Image8 greyImage;
	if (greyImage.FORMAT () == R_IMG.FORMAT ())
		greyImage = R_IMG;
	else
		R_IMG.CONVERT_TO (greyImage);
	cv::aruco::detectMarkers (greyImage, _R_PATTERN.CV_OBJ ()->dictionary,
		markerCorners, markerIds);
	if (markerIds.empty ())
		return false;

	// 2. 计算二维码标记周围的棋盘格角点位置
	cv::aruco::interpolateCornersCharuco (markerCorners, markerIds,
		greyImage, _R_PATTERN.CV_OBJ (), charucoCorners, charucoIds);

	// 3. 填入所有检测到的有效角点坐标
	for (size_t i = 0; i < charucoIds.size (); ++i)
		r_resultsOut[charucoIds[i]] = charucoCorners[i];

	// 4. 在绘制图层中绘制结果
	Painter painter (drawLayer);
	painter.fill (Color::Zero);
	painter.markers (r_resultsOut, cv::MARKER_CROSS, 15, Color::Red, 2);

	return true;

}

NS_DLIB_PATTERN_END