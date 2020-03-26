#pragma once
#include "global.h"
#include "UniformGridPattern.h"
#include <opencv2/aruco/charuco.hpp>



NS_DLIB_PATTERN_BEGIN

class Charuco : public UniformGridPattern {
public:
	const float SQUARE_LENGTH;

	Charuco (const cv::Size& R_PATTERN_SIZE, float squareLength);
	cv::Ptr<cv::aruco::CharucoBoard> CV_OBJ () const;
	virtual void DRAW (CvExt::Image& r_img,
		CvExt::Color color = CvExt::Color::White) const override;
	virtual Detector* GET_DETECTOR () const override;

private:
	cv::Ptr<cv::aruco::CharucoBoard> _p_board;

};

NS_DLIB_PATTERN_END