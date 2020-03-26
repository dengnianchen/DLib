#include "Intrinsic.h"

using namespace Dlib::Basic;
using namespace Dlib::CvExt;



NS_DLIB_CAM_BEGIN

Intrinsic::Intrinsic () {}



Intrinsic::Intrinsic (std::string filePath) 
throw (Exception::FileNotFound) {

	cv::FileStorage fs;
	if (!fs.open (filePath, cv::FileStorage::READ))
		throw Exception::FileNotFound (StringUtils::ToWString (filePath));
	fs["frameSize"] >> _frameSize;
	fs["cameraMatrix"] >> _cameraMatrix;
	fs["distCoeffs"] >> _distCoeffs;

}



Intrinsic::Intrinsic (cv::Size frameSize, cv::Mat cameraMatrix, cv::Mat distCoeffs) :
	_frameSize (frameSize),
	_cameraMatrix (cameraMatrix),
	_distCoeffs (distCoeffs) {}



cv::Size Intrinsic::FRAME_SIZE () const {
	
	return _frameSize;

}



void Intrinsic::setFrameSize (cv::Size frameSize) {

	if (frameSize == _frameSize)
		return;
	if (!_cameraMatrix.empty ()) {
		_cameraMatrix.at<double> (0, 0) *= (double)frameSize.width / _frameSize.width;
		_cameraMatrix.at<double> (1, 1) *= (double)frameSize.height / _frameSize.height;
		_cameraMatrix.at<double> (0, 2) *= (double)frameSize.width / _frameSize.width;
		_cameraMatrix.at<double> (1, 2) *= (double)frameSize.height / _frameSize.height;
	}
	_frameSize = frameSize;

}



cv::Mat Intrinsic::CAMERA_MATRIX () const {
	
	return _cameraMatrix;

}



cv::Mat Intrinsic::DIST_COEFFS () const {
	
	return _distCoeffs;

}



bool Intrinsic::SAVE (std::string filePath) const {

	cv::FileStorage fs;
	if (!fs.open (filePath, cv::FileStorage::WRITE))
		return false;
	fs << "frameSize" << _frameSize;
	fs << "cameraMatrix" << _cameraMatrix;
	fs << "distCoeffs" << _distCoeffs;
	return true;

}



void Intrinsic::UNDISTORT (const Image& R_IMAGE, Image& o_undistortedImage,
	bool reallocate) const
throw (Exception::InvalidArgument) {

	if (R_IMAGE.SIZE () != _frameSize)
		throw Exception::InvalidArgument (L"Size of input image does not match "
			"intrinsic parameter \"frameSize\"");
	if (o_undistortedImage.SIZE () != R_IMAGE.SIZE () ||
		o_undistortedImage.FORMAT () != R_IMAGE.FORMAT ()) {
		if (!reallocate)
			throw Exception::InvalidArgument (L"Size or format of undistorted image "
				"does not match input image");
		else
			o_undistortedImage = Image (R_IMAGE.FORMAT ());
	}
	if (_cameraMatrix.empty ())
		R_IMAGE->copyTo (o_undistortedImage);
	else {
		if (R_IMAGE.SIZE () != _map1.size ())
			const_cast<Intrinsic*>(this)->_generateUndistortionMap (R_IMAGE.SIZE ());
		cv::remap (R_IMAGE, o_undistortedImage, _map1, _map2, cv::INTER_LINEAR,
			cv::BORDER_CONSTANT);
	}

}



void Intrinsic::UNDISTORT (cv::InputArray R_POINTS, cv::OutputArray o_points) const {

	if (_cameraMatrix.empty ())
		R_POINTS.copyTo (o_points);
	if (!R_POINTS.empty ())
		cv::undistortPoints (R_POINTS, o_points, _cameraMatrix, _distCoeffs,
			cv::noArray (), _cameraMatrix);

}



void Intrinsic::_generateUndistortionMap (cv::Size imageSize) {

	if (_cameraMatrix.empty ())
		return;
	_map1 = cv::Mat (imageSize.height, imageSize.width, CV_16SC2);
	_map2 = cv::Mat (imageSize.height, imageSize.width, CV_16UC1);
	cv::initUndistortRectifyMap (_cameraMatrix, _distCoeffs,
		cv::Mat_<double>::eye (3, 3), _cameraMatrix,
		imageSize, _map1.type (), _map1, _map2);

}

NS_DLIB_CAM_END