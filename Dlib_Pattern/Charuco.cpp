#include "Charuco.h"
#include "CharucoDetector.h"

using namespace Dlib::CvExt;



NS_DLIB_PATTERN_BEGIN

Charuco::Charuco (const cv::Size& R_PATTERN_SIZE, float squareLength) :
UniformGridPattern ({ 0.0f, 0.0f, squareLength * R_PATTERN_SIZE.width, squareLength * R_PATTERN_SIZE.height }, R_PATTERN_SIZE),
SQUARE_LENGTH (squareLength) {

	_p_board = cv::aruco::CharucoBoard::create (
		R_PATTERN_SIZE.width + 1, R_PATTERN_SIZE.height + 1,
		squareLength, squareLength * 0.75f,
		cv::aruco::getPredefinedDictionary (cv::aruco::DICT_5X5_1000));

}



cv::Ptr<cv::aruco::CharucoBoard> Charuco::CV_OBJ () const {

	return _p_board;

}



void Charuco::DRAW (Image& r_img, Color color) const {

	Image8 tempImage;
	_p_board->draw (r_img.SIZE (), tempImage);
	tempImage.CONVERT_TO (r_img);
	// For debug >>>>
	//std::ostringstream sout;
	//sout << "board depth=" << cvBoardImage.channels ();
	//cv::imshow (sout.str (), cvBoardImage);
	// <<<<

}



Detector* Charuco::GET_DETECTOR () const {

	return new CharucoDetector (*this);

}

NS_DLIB_PATTERN_END