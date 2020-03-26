#pragma once
#include "global.h"
#include "../../Dlib_Cam/_CameraImpl.h"
#include "../../Dlib_Cam/Identifier.h"
#include "SpinnakerSystem.h"



NS_DLIB_CAM_BEGIN

class SpinnakerCamera : public _CameraImpl {
public:
	SpinnakerCamera (const Identifier& R_IDENTIFIER, SpinnakerSystem* p_system)
		throw (Exception::VideoCameraException);
	virtual ~SpinnakerCamera ();

	void init ()
		throw (Exception::VideoCameraException) override;
	void start ()
		throw (Exception::VideoCameraException) override;
	bool grabFrame () override;
	bool retrieveFrame (CvExt::Image& o_image, unsigned long long& o_timestamp)
		throw (Exception::VideoCameraException) override;

	void stop () override;
	std::string DEVICE_INFO () const override;
	EStatus STATUS () const override;
	cv::Size FRAME_SIZE () const override;
	void setFrameSize (cv::Size frameSize) override;
	CvExt::PixelFormat FORMAT () const override;
	bool setFormat (CvExt::PixelFormat format) override;
	Property::Range PROPERTY_RANGE (Property::EType type) const override;
	Property PROPERTY_VALUE (Property::EType type) const override;
	bool setPropertyValue (Property::EType type, Property value) override;

private:
	Identifier _identifier;
	SpinnakerSystem* _p_system;
	Spinnaker::CameraPtr _p_cam;
	CvExt::PixelFormat _format;
	cv::Size _outputFrameSize;
	cv::Mat _tempImage;
	unsigned long long _timestampAdjust;

};

NS_DLIB_CAM_END