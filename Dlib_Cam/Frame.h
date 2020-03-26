#pragma once
#include "global.h"
#include "../Dlib_CvExt/All.h"
#include "Intrinsic.h"



NS_DLIB_CAM_BEGIN

class Camera;

/**
 * @brief Frame类用于表示相机的画面帧。画面帧包含图像、时间戳。
 *        为了方便使用，Frame类亦保持了对相关相机对象的引用。
 */
class DLIBCAM_INTERFACE Frame {
public:
	/**
	 * @brief 对相机的常量引用
	 */
	const Camera& R_CAMERA;

	/**
	 * @brief 获取当前画面帧的时间戳。该值由设备相关的第三方SDK提供。
	 * @return 当前画面帧的时间戳
	 */
	uint64 TIMESTAMP () const;
	/**
	 * @brief 获取当前帧的图像数据。
	 * @return 当前帧的图像数据
	 */
	const CvExt::Image& IMAGE () const;
	/**
	 * @brief 获取经过畸变矫正的当前帧图像。若当前无内参标定数据，则返回原始图像。
	 * @return 经过畸变矫正的当前帧图像
	 */
	const CvExt::Image& UNDISTORTED_IMAGE () const;

private:
	friend class Camera;

	/**
	 * @brief 图像大小
	 */
	cv::Size _size;
	/**
	 * @brief 从相机获得的图像
	 */
	CvExt::Image _cvImg;
	/**
	 * @brief 相机产生该画面帧时的时间戳
	 */
	uint64 _timestamp;
	/**
	 * @brief 畸变矫正后的帧图像
	 */
	mutable CvExt::Image _cvUndistortedImg;
	/**
	 * @brief 是否需要更新_cvUndistortedImg
	 */
	mutable bool _undistortedImageDirty;

	/**
	 * @brief 创建一个画面帧。该方法由SpinnakerCamera类调用，外部无需创建Frame对象。
	 * @param R_CAMERA 相关相机对象
	 */
	Frame (const Camera& R_CAMERA);

};

NS_DLIB_CAM_END