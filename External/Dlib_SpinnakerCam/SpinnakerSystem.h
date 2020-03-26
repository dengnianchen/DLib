#pragma once
#include "global.h"
#include "../../Dlib_Cam/_System.h"



NS_DLIB_CAM_BEGIN

/**
 * @brief SpinnakerSystem类封装了Spinnaker库的System类，维护一个单例对象。通过静态
 *        方法提供Spinnaker系统的启动、停止、相机设备查询接口。该类主要为
 *        SpinnakerCamera类调用。
 */
class SpinnakerSystem : public _System {
public:
	/**
	 * 默认构造函数，创建Spinnaker系统实例
	 */
	SpinnakerSystem ();
	/**
	 * 默认析构函数，释放Spinnaker系统资源
	 */
	~SpinnakerSystem ();

	virtual _CameraImpl* createInstance (const Identifier& R_IDENTIFIER)
		throw (Dlib::Cam::Exception::VideoCameraException) override;
	Spinnaker::CameraList getDeviceList ();

private:
	Spinnaker::SystemPtr _p_system;

};

NS_DLIB_CAM_END