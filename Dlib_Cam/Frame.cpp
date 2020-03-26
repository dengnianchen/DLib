#include "Frame.h"
#include "Camera.h"

using namespace Dlib::CvExt;



NS_DLIB_CAM_BEGIN

uint64 Frame::TIMESTAMP () const {
	
	return _timestamp;

}



const Image& Frame::IMAGE () const {
	
	return _cvImg;

}



const Image& Frame::UNDISTORTED_IMAGE () const {

	if (_undistortedImageDirty) {
		if (_cvImg.IS_EMPTY ())
			_cvUndistortedImg = _cvImg;
		else
			R_CAMERA.INTRINSIC ().UNDISTORT (_cvImg, _cvUndistortedImg);
		_undistortedImageDirty = false;
	}
	return _cvUndistortedImg;

}



Frame::Frame (const Camera& R_CAMERA) :
_timestamp (0),
R_CAMERA (R_CAMERA),
_undistortedImageDirty (false) {
}

NS_DLIB_CAM_END