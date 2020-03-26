#include "CharucoDetector.h"

using namespace Dlib::CvExt;
using namespace Dlib::Basic;



NS_DLIB_PATTERN_BEGIN

CharucoDetector::CharucoDetector (const Charuco& R_PATTERN) :
_R_PATTERN (R_PATTERN) {
}



bool CharucoDetector::run (const Image& R_IMG, ArrayRef<cv::Point2f> r_resultsOut,
	Image drawLayer, bool isDebug) {

	/* ��ά������ţ����̸�ǵ���� */
	std::vector< int > markerIds, charucoIds;
	/* ��ά���ǽǵ����� */
	std::vector< std::vector< cv::Point2f > > markerCorners;
	/* ���̸�ǵ����� */
	std::vector< cv::Point2f > charucoCorners;

	// 0. ��ս�������е�����ֵ����ΪNaN��
	for (int i = 0; i < _R_PATTERN.N_ALL_CORNERS; ++i)
		r_resultsOut[i] = { NAN, NAN };

	// 1. �ڻ����м���ά����
	Image8 greyImage;
	if (greyImage.FORMAT () == R_IMG.FORMAT ())
		greyImage = R_IMG;
	else
		R_IMG.CONVERT_TO (greyImage);
	cv::aruco::detectMarkers (greyImage, _R_PATTERN.CV_OBJ ()->dictionary,
		markerCorners, markerIds);
	if (markerIds.empty ())
		return false;

	// 2. �����ά������Χ�����̸�ǵ�λ��
	cv::aruco::interpolateCornersCharuco (markerCorners, markerIds,
		greyImage, _R_PATTERN.CV_OBJ (), charucoCorners, charucoIds);

	// 3. �������м�⵽����Ч�ǵ�����
	for (size_t i = 0; i < charucoIds.size (); ++i)
		r_resultsOut[charucoIds[i]] = charucoCorners[i];

	// 4. �ڻ���ͼ���л��ƽ��
	Painter painter (drawLayer);
	painter.fill (Color::Zero);
	painter.markers (r_resultsOut, cv::MARKER_CROSS, 15, Color::Red, 2);

	return true;

}

NS_DLIB_PATTERN_END