#pragma once
#include "global.h"
#include "../../Dlib_Cam/_CameraImpl.h"
#include "CvCamSystem.h"



NS_DLIB_CAM_BEGIN

class CvCamera : public _CameraImpl {
public:
	CvCamera (const Identifier& R_IDENTIFIER, CvCamSystem* p_system)
		throw (Exception::VideoCameraException);
	virtual ~CvCamera ();

	void init ()
		throw (Exception::VideoCameraException) override;
	void start ()
		throw (Exception::VideoCameraException) override;
	bool grabFrame ()
		throw (Exception::VideoCameraException) override;
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
	cv::VideoCapture _cap;
	CvExt::PixelFormat _format;
	cv::Size _outputFrameSize;
	cv::Size _rawFrameSize;
	EStatus _status;
	bool _grabbed;
	cv::Mat _tempImage;

	CvExt::PixelFormat _CVFORMAT_TO_EFORMAT (int rawFormat) const;
};

NS_DLIB_CAM_END