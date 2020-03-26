#include "stdafx.h"
#include "SpinnakerSystem.h"
#include "SpinnakerCamera.h"



NS_DLIB_CAM_BEGIN

SpinnakerSystem::SpinnakerSystem () {

	_p_system = Spinnaker::System::GetInstance ();

}



SpinnakerSystem::~SpinnakerSystem () {

	_p_system->ReleaseInstance ();

}



_CameraImpl* SpinnakerSystem::createInstance (const Identifier& R_IDENTIFIER)
throw (Dlib::Cam::Exception::VideoCameraException) {

	return new SpinnakerCamera (R_IDENTIFIER, this);

}



Spinnaker::CameraList SpinnakerSystem::getDeviceList () {

	return _p_system->GetCameras ();

}

NS_DLIB_CAM_END