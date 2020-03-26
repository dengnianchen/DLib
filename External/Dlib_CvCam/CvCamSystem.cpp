#include "stdafx.h"
#include "CvCamSystem.h"
#include "CvCamera.h"



NS_DLIB_CAM_BEGIN

_CameraImpl* CvCamSystem::createInstance (const Identifier& R_IDENTIFIER)
throw (Exception::VideoCameraException) {

	return new CvCamera (R_IDENTIFIER, this);

}

NS_DLIB_CAM_END