#include "Camera.h"
#include <Windows.h>
#include <sstream>
#include <map>
#include "../Dlib_Basic/Ptr.h"
#include "_CameraImpl.h"
#include "_SubModule.h"
#include "_System.h"

#undef DEVICE_TYPE

using namespace Dlib::Basic;
using namespace Dlib::CvExt;



NS_DLIB_CAM_BEGIN

/**
 * @brief [静态]相机设备实例映射表（模块名->实例列表）
 */
static std::map<std::string, std::vector<Ptr<Camera>>> _CameraInstances;



void Camera::init ()
throw (Exception::VideoCameraException) {

	_p_impl->init ();

}



void Camera::start ()
throw (Exception::VideoCameraException) {

	_p_impl->start ();

}



bool Camera::grabFrame ()
throw (Exception::VideoCameraException) {

	return _p_impl->grabFrame ();

}



bool Camera::updateFrame ()
throw (Exception::VideoCameraException) {

	bool isUpdated = _p_impl->retrieveFrame (_frame._cvImg,
		_frame._timestamp);
	if (isUpdated)
		_frame._undistortedImageDirty = true;
	return isUpdated;

}



void Camera::stop () {

	_p_impl->stop ();

}



std::string Camera::DEVICE_INFO () const {

	return _p_impl->DEVICE_INFO ();

}



std::string Camera::IDENTIFIER () const {

	return _identifier;

}



EStatus Camera::STATUS () const {

	return _p_impl->STATUS ();

}



const Intrinsic& Camera::INTRINSIC () const {
	
	return _intrinsic;

}



void Camera::loadIntrinsic (const std::string& file)
throw (Exception::VideoCameraException) try {
	
	_intrinsic = Intrinsic (file);

} catch (std::exception ex) {
	throw Exception::VideoCameraException (
		_identifier, "loadIntrinsic", ex.what ());
}



cv::Size Camera::FRAME_SIZE () const {
	
	return _p_impl->FRAME_SIZE ();

}



void Camera::setFrameSize (cv::Size size) {
	
	_p_impl->setFrameSize (size);

}



PixelFormat Camera::FORMAT () const {
	
	return _p_impl->FORMAT ();

}



bool Camera::setFormat (PixelFormat format) {

	return _p_impl->setFormat (format);

}



Property::Range Camera::PROPERTY_RANGE (Property::EType type) const {
	
	return _p_impl->PROPERTY_RANGE (type);

}



Property Camera::PROPERTY_VALUE (Property::EType type) const {
	
	return _p_impl->PROPERTY_VALUE (type);

}



bool Camera::setPropertyValue (Property::EType type, Property value) {

	return _p_impl->setPropertyValue (type, value);

}



const Frame& Camera::FRAME () const {
	
	return _frame;

}



Camera* Camera::GetInstance (const std::string& deviceType, int id)
throw (Exception::VideoCameraException) {

	// 根据标识符查找实例，若存在则引用计数加1，并返回该实例
	auto& r_instanceList = _CameraInstances[deviceType];
	if (id < r_instanceList.size () && r_instanceList[id] != NULL) {
		r_instanceList[id].incrCount ();
		return r_instanceList[id];
	}
	
	// 若不存在已有实例，则根据deviceType调用对应的设备实现模块创建实例
	auto _p_instance = new Camera ({ deviceType, id });
	if (id >= r_instanceList.size ())
		r_instanceList.resize (id + 1);
	r_instanceList[id] = _p_instance;
	OutputDebugStringA ("[Dlib_Cam] Create CEOFS new camera instance: ");
	OutputDebugStringA (_p_instance->IDENTIFIER ().c_str ());
	OutputDebugStringA ("\n");
	return _p_instance;

}



void Camera::ReleaseInstance (Camera*& p_instance) {
	
	if (!p_instance)
		return;
	auto identifier = p_instance->_identifier;
	auto& r_instanceList = _CameraInstances[identifier.DEVICE_TYPE];
	auto& r_ptr = r_instanceList[identifier.ID];
	if (r_ptr != NULL)
		r_ptr.decrCount ();
	if (r_ptr == NULL) {
		OutputDebugStringA ("[Dlib_Cam] Camera instance ");
		OutputDebugStringA (((std::string)identifier).c_str ());
		OutputDebugStringA (" released.\n");
		p_instance = NULL;
	}

}



void Camera::ReleaseAllInstances () {

	for (auto it = _CameraInstances.begin (); it != _CameraInstances.end (); ++it)
		for (int i = 0; i < it->second.size (); ++i)
			it->second[i].release ();
	_CameraInstances.clear ();

}



void Camera::AllGrabFrames ()
throw (Exception::VideoCameraException) {

	for (auto it = _CameraInstances.begin (); it != _CameraInstances.end (); ++it)
		for (int i = 0; i < it->second.size (); ++i) {
			if (it->second[i] != NULL)
				it->second[i]->grabFrame ();
		}

}



Camera::Camera (const Identifier& identifier)
throw (Exception::VideoCameraException) :
	_identifier (identifier),
	_frame (*this) {

	_p_impl = _SubModule::GetSubModule (identifier.DEVICE_TYPE).p_system
		->createInstance (identifier);

}


Camera::~Camera () {

	delete _p_impl;

}

NS_DLIB_CAM_END