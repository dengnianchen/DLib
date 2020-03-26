#pragma once
#include "global.h"
#include "../Dlib_Basic/Basic.h"



NS_DLIB_CVEXT_BEGIN

/**
 * @brief 读取图像中指定位置的像素值。

 * 目前支持的OpenCV类型包括CV_8UC[1-4]和CV_32FC[1-4]。
 *
 * @param const cv::Mat&	R_IMAGE	图像
 * @param cv::Point			p		坐标
 *
 * @return cv::Scalar 像素值
 * @throw Dlib::Basic::Exception::InvalidArgument 若不支持图像的OpenCV类型
 * @author Nianchen Deng
 */
cv::Scalar GetPixel (const cv::Mat& R_IMAGE, cv::Point p);

/**
 * @brief 读取图像中指定位置的像素值。
 *
 * cv::Scalar GetPixel (const cv::Mat&, cv::Point)的重载版本。
 *
 * @param const cv::Mat&	R_IMAGE	图像
 * @param int				x		横坐标
 * @param int				y		纵坐标
 *
 * @return cv::Scalar 像素值
 * @throw Dlib::Basic::Exception::InvalidArgument 若不支持图像的OpenCV类型
 * @author Nianchen Deng
 */
cv::Scalar GetPixel (const cv::Mat& R_IMAGE, int x, int y);

/**
 * @brief 读取图像中指定位置的像素值。
 *
 * cv::Scalar GetPixel (const cv::Mat&, cv::Point)的重载版本。
 *
 * @param const cv::Mat&	R_IMAGE	图像
 * @param int				i		一维坐标
 *
 * @return cv::Scalar 像素值
 * @throw Dlib::Basic::Exception::InvalidArgument 若不支持图像的OpenCV类型
 * @author Nianchen Deng
 */
cv::Scalar GetPixel (const cv::Mat& R_IMAGE, int i);

/**
 * @brief 设置图像中指定位置的像素值。
 *
 * 目前支持的OpenCV类型包括CV_8UC[1-4]和CV_32FC[1-4]。
 *
 * @param const cv::Mat&	R_IMAGE	图像
 * @param cv::Point			p		坐标
 * @param const cv::Scalar&	R_VALUE	像素值
 *
 * @throw Dlib::Basic::Exception::InvalidArgument 若不支持图像的OpenCV类型
 * @author Nianchen Deng
 */
void SetPixel (cv::Mat& r_image, cv::Point p, const cv::Scalar& R_VALUE);

/**
 * @brief 设置图像中指定位置的像素值。
 *
 * void SetPixel (cv::Mat&, cv::Point, const cv::Scalar)的重载版本。
 *
 * @param const cv::Mat&	R_IMAGE	图像
 * @param int				x		横坐标
 * @param int				y		纵坐标
 * @param const cv::Scalar&	R_VALUE	像素值
 *
 * @throw Dlib::Basic::Exception::InvalidArgument 若不支持图像的OpenCV类型
 * @author Nianchen Deng
 */
void SetPixel (cv::Mat& r_image, int x, int y, const cv::Scalar& R_VALUE);

/**
 * @brief 设置图像中指定位置的像素值。
 *
 * void SetPixel (cv::Mat&, cv::Point, const cv::Scalar)的重载版本。
 *
 * @param const cv::Mat&	R_IMAGE	图像
 * @param int				i		一维坐标
 * @param const cv::Scalar&	R_VALUE	像素值
 *
 * @throw Dlib::Basic::Exception::InvalidArgument 若不支持图像的OpenCV类型
 * @author Nianchen Deng
 */
void SetPixel (cv::Mat& r_image, int i, const cv::Scalar& R_VALUE);

NS_DLIB_CVEXT_END