#pragma once
#include "global.h"
#include <opencv2/opencv.hpp>
#include "../Dlib_CvExt/All.h"
#include "EStatus.h"
#include "Property.h"
#include "VideoCameraException.h"



NS_DLIB_CAM_BEGIN

class _CameraImpl {
public:
	virtual ~_CameraImpl () {};

	virtual void init ()
		throw (Exception::VideoCameraException) = 0;
	virtual void start ()
		throw (Exception::VideoCameraException) = 0;
	virtual bool grabFrame ()
		throw (Exception::VideoCameraException) = 0;
	virtual bool retrieveFrame (CvExt::Image& o_image, unsigned long long& o_timestamp)
		throw (Exception::VideoCameraException) = 0;

	virtual void stop () = 0;
	virtual std::string DEVICE_INFO () const = 0;
	virtual EStatus STATUS () const = 0;
	virtual cv::Size FRAME_SIZE () const = 0;
	virtual void setFrameSize (cv::Size frameSize) = 0;
	virtual CvExt::PixelFormat FORMAT () const = 0;
	virtual bool setFormat (CvExt::PixelFormat format) = 0;
	virtual Property::Range PROPERTY_RANGE (Property::EType type) const = 0;
	virtual Property PROPERTY_VALUE (Property::EType type) const = 0;
	virtual bool setPropertyValue (Property::EType type, Property value) = 0;

};

NS_DLIB_CAM_END