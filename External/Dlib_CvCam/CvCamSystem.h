#pragma once
#include "global.h"
#include "../../Dlib_Cam/_System.h"



NS_DLIB_CAM_BEGIN

/**
 * @brief CvCamSystem类实现了Dlib::Cam::_System接口，提供实例化CvCam的接口实现。
 */
class CvCamSystem : public _System {
public:
	virtual _CameraImpl* createInstance (const Identifier& R_IDENTIFIER)
		throw (Exception::VideoCameraException) override;

};

NS_DLIB_CAM_END