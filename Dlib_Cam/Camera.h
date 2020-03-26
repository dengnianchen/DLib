#pragma once
#include "global.h"
#include "../Dlib_CvExt/All.h"
#include "Frame.h"
#include "Property.h"
#include "EStatus.h"
#include "Identifier.h"
#include "VideoCameraException.h"



NS_DLIB_CAM_BEGIN

class _CameraImpl;

class DLIBCAM_INTERFACE Camera {
public:
	void init ()
		throw (Exception::VideoCameraException);
	void start ()
		throw (Exception::VideoCameraException);
	bool grabFrame ()
		throw (Exception::VideoCameraException);
	bool updateFrame ()
		throw (Exception::VideoCameraException);
	void stop ();
	std::string IDENTIFIER () const;
	std::string DEVICE_INFO () const;
	EStatus STATUS () const;
	const Frame& FRAME () const;
	const Intrinsic& INTRINSIC () const;
	void loadIntrinsic (const std::string& file)
		throw (Exception::VideoCameraException);
	cv::Size FRAME_SIZE () const;
	void setFrameSize (cv::Size size);
	CvExt::PixelFormat FORMAT () const;
	bool setFormat (CvExt::PixelFormat format);
	Property::Range PROPERTY_RANGE (Property::EType type) const;
	Property PROPERTY_VALUE (Property::EType type) const;
	bool setPropertyValue (Property::EType type, Property value);

	static Camera* GetInstance (const std::string& deviceType, int id)
		throw (Exception::VideoCameraException);
	static void ReleaseInstance (Camera*& p_instance);
	static void ReleaseAllInstances ();
	static void AllGrabFrames ()
		throw (Exception::VideoCameraException);

private:
	/**
	 * @brief 相机标识符，由相机类型+序号组成
	 */
	Identifier _identifier;
	/**
	 * @brief 相机设备功能实现对象，根据相机的类型由相应子模块提供具体实现
	 */
	_CameraImpl* _p_impl;
	/**
	 * @brief 相机内参
	 */
	Intrinsic _intrinsic;
	/**
	 * @brief 相机画面帧对象
	 */
	Frame _frame;

public:
	Camera (const Identifier& identifier)
		throw (Exception::VideoCameraException);
	~Camera ();
	
};

NS_DLIB_CAM_END