#include "../../Dlib_Cam/Camera.h"
#include "../../Dlib_Unity/All.h"

using namespace Dlib::Basic;
using namespace Dlib::Cam;
using namespace Dlib::CvExt;
using namespace Dlib::Unity;



extern "C" {

UDLIB_INTERFACE (Camera* Camera_GetInstance (const char* deviceType, int id)) {

	return Camera::GetInstance (deviceType, id);

} END_UDLIB_INTERFACE



UDLIB_INTERFACE (void Camera_ReleaseInstance (Camera* p_inst)) {

	Camera::ReleaseInstance (p_inst);

} END_UDLIB_INTERFACE



UDLIB_INTERFACE (void Camera_ReleaseAllInstances ()) {

	Camera::ReleaseAllInstances ();

} END_UDLIB_INTERFACE



UDLIB_INTERFACE (void Camera_AllGrabFrames ()) {

	Camera::AllGrabFrames ();

} END_UDLIB_INTERFACE



UDLIB_INTERFACE (void Camera_init (Camera* p_inst)) {
	
	p_inst->init ();

} END_UDLIB_INTERFACE



UDLIB_INTERFACE (void Camera_start (Camera* p_inst)) {

	p_inst->start ();

} END_UDLIB_INTERFACE



UDLIB_INTERFACE (void Camera_stop (Camera* p_inst)) {

	p_inst->stop ();

} END_UDLIB_INTERFACE



UDLIB_INTERFACE (const char* Camera_getDeviceInfo (Camera* p_inst)) {

	return Str::ToReturnStr (p_inst->DEVICE_INFO ());

} END_UDLIB_INTERFACE



UDLIB_INTERFACE (int Camera_getStatus (Camera* p_inst)) {

	return p_inst->STATUS ();

} END_UDLIB_INTERFACE



UDLIB_INTERFACE (const Intrinsic* Camera_getIntrinsic (Camera* p_inst)) {

	return &p_inst->INTRINSIC ();

} END_UDLIB_INTERFACE



UDLIB_INTERFACE (void Camera_loadIntrinsic (Camera* p_inst, const char* file)) {

	p_inst->loadIntrinsic (file);

} END_UDLIB_INTERFACE



UDLIB_INTERFACE (void Camera_getFrameSize (Camera* p_inst, Vec2i& ret)) {

	ret = p_inst->FRAME_SIZE ();

} END_UDLIB_INTERFACE



UDLIB_INTERFACE (void Camera_setFrameSize (Camera* p_inst, Vec2i size)) {

	p_inst->setFrameSize (size);

} END_UDLIB_INTERFACE



UDLIB_INTERFACE (PixelFormat::EFormat Camera_getFormat (Camera* p_inst)) {

	return p_inst->FORMAT ();

} END_UDLIB_INTERFACE



UDLIB_INTERFACE (bool Camera_setFormat (Camera* p_inst, PixelFormat format)) {

	return p_inst->setFormat (format);

} END_UDLIB_INTERFACE



UDLIB_INTERFACE (bool Camera_getPropertyRange (Camera* p_inst,
	Property::EType type, Property::Range& ret)) {

	ret = p_inst->PROPERTY_RANGE (type);
	return !isnan (ret.high);

} END_UDLIB_INTERFACE



UDLIB_INTERFACE (bool Camera_getPropertyValue (Camera* p_inst,
	Property::EType type, Property& ret)) {

	ret = p_inst->PROPERTY_VALUE (type);
	return ret.isValid;

} END_UDLIB_INTERFACE



UDLIB_INTERFACE (bool Camera_setPropertyValue (Camera* p_inst,
	Property::EType type, Property value)) {

	return p_inst->setPropertyValue (type, value);

} END_UDLIB_INTERFACE



UDLIB_INTERFACE (bool Camera_grabFrame (Camera* p_inst)) {

	return p_inst->grabFrame ();

} END_UDLIB_INTERFACE



UDLIB_INTERFACE (bool Camera_updateFrame (Camera* p_inst)) {

	return p_inst->updateFrame ();

} END_UDLIB_INTERFACE



UDLIB_INTERFACE (const Frame* Camera_getFrame (Camera* p_inst)) {

	return &p_inst->FRAME ();

} END_UDLIB_INTERFACE



UDLIB_INTERFACE (int64 Frame_getTimestamp (Frame* p_inst)) {
	
	return p_inst->TIMESTAMP ();

} END_UDLIB_INTERFACE



UDLIB_INTERFACE (void Frame_getImage (Frame* p_inst, UImage outImage)) {
	
	outImage << p_inst->IMAGE ();

} END_UDLIB_INTERFACE



UDLIB_INTERFACE (void Frame_getUndistortedImage (Frame* p_inst, UImage outImage)) {

	outImage << p_inst->UNDISTORTED_IMAGE ();

} END_UDLIB_INTERFACE



UDLIB_INTERFACE (bool Intrinsic_getData (Intrinsic* p_inst, UMatrix<float> outCameraMatrix,
	ArrayRef<float> outDistCeoffs)) {

	cv::Mat cmat = p_inst->CAMERA_MATRIX (); // 3 x 3
	cv::Mat dvec = p_inst->DIST_COEFFS (); // 1 x 5
	if (cmat.empty ())
		return false;
	cv::Mat_<float> outCameraMatrix_ = outCameraMatrix;
	for (int r = 0; r < cmat.rows; ++r)
		for (int c = 0; c < cmat.cols; ++c)
			if (r == 1 && c == 1)
				outCameraMatrix_ (r, c) = -float (cmat.at<double> (r, c));
			else
				outCameraMatrix_ (r, c) = float (cmat.at<double> (r, c));
	for (int c = 0; c < dvec.cols; ++c)
		outDistCeoffs[c] = float (dvec.at<double> (c));
	return true;

} END_UDLIB_INTERFACE



UDLIB_INTERFACE (void Intrinsic_undistortImage (Intrinsic* p_inst, UImage srcImage,
	UImage outUndistortedImage)) {

	Image undistortedImage = outUndistortedImage;
	p_inst->UNDISTORT (srcImage, undistortedImage, false);

} END_UDLIB_INTERFACE



UDLIB_INTERFACE (void Intrinsic_undistortPoints (Intrinsic* p_inst,
	ConstArrayRef<cv::Point2f> inPoints, ArrayRef<cv::Point2f> outPoints)) {

	p_inst->UNDISTORT (inPoints, outPoints);

} END_UDLIB_INTERFACE

}