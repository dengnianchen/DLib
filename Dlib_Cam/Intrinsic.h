#pragma once
#include "global.h"
#include "../Dlib_Basic/Basic.h"
#include "../Dlib_CvExt/All.h"



NS_DLIB_CAM_BEGIN

/**
 * @brief Intrinsic类封装了相机内参数据，包括相机矩阵和畸变系数。有关于相机
 *        内参的含义和表示方式参见OpenCV官方文档。Intrinsic类提供一些便捷
 *        的方法用于相机内参的保存、读取，以及应用该内参对相机图像进行畸变补偿。
 *
 * @author Nianchen Deng
 */
class DLIBCAM_INTERFACE Intrinsic {
public:
	/**
	 * @brief 创建一个空的（无效的）相机内参
	 *
	 * @author Nianchen Deng
	 */
	Intrinsic ();
	/**
	 * @brief 创建一个相机内参，其参数从指定文件读取。若指定文件不存在或为无效格式，
	 *        则抛出异常。
	 *
	 * @param filePath 指定文件的路径，该文件为yml格式，内容由相机标定程序生成
	 *
	 * @exception Dlib::Basic::Exception::FileNotFound 当无法从指定文件读取内参数据时抛出
	 * @author Nianchen Deng
	 */
	Intrinsic (std::string filePath)
		throw (Basic::Exception::FileNotFound);
	/**
	 * @brief 用指定参数创建相机内参。该方法不验证参数内容是否有效。
	 *
	 * @param cameraMatrix 3x3相机矩阵
	 * @param distCoeffs 1x5畸变系数
	 *
	 * @author Nianchen Deng
	 */
	Intrinsic (cv::Size frameSize, cv::Mat cameraMatrix, cv::Mat distCoeffs);

	/**
	 * @brief 获取当前内参所对应的画面尺寸。
	 *
	 * @return 当前内参所对应的画面尺寸
	 * @author Nianchen Deng
	 */
	cv::Size FRAME_SIZE () const;
	/**
	 * @brief 设置内参的画面尺寸，并相应更新相机内参矩阵。
	 *
	 * @param frameSize 画面尺寸
	 *
	 * @author Nianchen Deng
	 */
	void setFrameSize (cv::Size frameSize);
	/**
	 * @brief 获取相机投影矩阵。
	 *
	 * @return 相机投影矩阵
	 * @author Nianchen Deng
	 */
	cv::Mat CAMERA_MATRIX () const;
	/**
	 * @brief 获取畸变系数。
	 *
	 * @return 畸变系数
	 * @author Nianchen Deng
	 */
	cv::Mat DIST_COEFFS () const;
	/**
	 * @brief 将内参保存到指定文件。利用OpenCV内置的数据存储接口保存为yml格式。
	 *
	 * @param filePath 指定文件的路径
	 * @return 是否成功保存
	 */
	bool SAVE (std::string filePath) const;
	/**
	 * @brief 用内参对指定图像畸变补偿，返回畸变补偿后的图像。
	 *
	 * @param R_IMAGE				指定图像，一般为从相机获得的画面
	 * @param o_undistortedImage	（输出）畸变矫正后的图像
	 * @param reallocate			指定若参数o_undistortedImage的尺寸或格式不符合要求，
	 *								是否允许重新分配空间
	 *
	 * @throw Dlib::Basic::Exception::InvalidArgument 输入图像为尺寸与内参不匹配或
	 *            输出图像尺寸/格式与输入图像不匹配
	 * @author Nianchen Deng
	 */
	void UNDISTORT (const CvExt::Image& R_IMAGE,
		CvExt::Image& o_undistortedImage, bool reallocate = true) const
		throw (Basic::Exception::InvalidArgument);
	/**
	 * @brief 用内参计算离散图像坐标在畸变补偿后对应的新坐标。
	 *
	 * @param R_POINTS 图像坐标数组
	 * @param o_points 畸变补偿后的新坐标数组
	 *
	 * @author Nianchen Deng
	 */
	void UNDISTORT (cv::InputArray R_POINTS, cv::OutputArray o_points) const;

private:
	/**
	 * @brief 画面尺寸
	 */
	cv::Size _frameSize;
	/**
	 * @brief 相机矩阵
	 */
	cv::Mat _cameraMatrix;
	/**
	 * @brief 畸变系数
	 */
	cv::Mat _distCoeffs;
	cv::Mat _map1, _map2;

	/**
	 * @brief 对指定图像尺寸生成用于畸变矫正的映射矩阵，
	 *        结果保存在_map1和_map2两个私有成员变量中。
	 *
	 * @param imageSize 图像尺寸
	 *
	 * @author Nianchen Deng
	 */
	void _generateUndistortionMap (cv::Size imageSize);

};

NS_DLIB_CAM_END