#pragma once
#include "global.h"
#include "Identifier.h"
#include "VideoCameraException.h"



NS_DLIB_CAM_BEGIN

class _CameraImpl;

class _System {
public:
	virtual ~_System () {}
	virtual _CameraImpl* createInstance (const Identifier& R_IDENTIFIER)
		throw (Exception::VideoCameraException) = 0;

};

NS_DLIB_CAM_END