#pragma once
#include "Vec.h"



NS_DLIB_UNITY_BEGIN

class DLIB_UNITY_INTERFACE UImage {
public:
	CvExt::PixelFormat format;
	Vec2i size;
	void* data;

	UImage& operator= (const UImage&) = default;
	/**
	 * 获得指定图像数据的引用副本。该操作不会复制图像数据。
	 */
	UImage& operator= (const CvExt::Image& R_IMAGE);
	/**
	 * 将指定图像数据复制（传输）到当前图像中，该操作不会改变当前图像的格式和大小，
	 * 若两者的格式或大小不同，则执行相应的转换
	 */
	UImage& operator<< (const CvExt::Image& R_IMAGE);
	/**
	 * 将当前图像数据的引用副本以Dlib::CvExt::Image类型返回。该操作不会复制图像数据。
	 */
	operator CvExt::Image ();
	/**
	 * 将当前图像数据的引用副本以cv::Mat类型返回。该操作不会复制图像数据。
	 */
	operator cv::Mat ();

};

NS_DLIB_UNITY_END