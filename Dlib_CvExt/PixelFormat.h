#pragma once
#include "global.h"
#include "../Dlib_Basic/Basic.h"



NS_DLIB_CVEXT_BEGIN

/**
 * 颜色空间枚举变量
 */
enum EColorSpace {
	EColor_Unknown = -1,	/* 未知 */
	EColor_Mono,			/* 单色（灰度），支持单通道格式 */
	EColor_BGR,				/* 彩色空间（B首位顺序），支持3-4通道格式 */
	EColor_RGB,				/* 彩色空间（R首位顺序），支持3-4通道格式 */
	EColor_HSV				/* HSV空间，支持3通道浮点数格式 */
};



/**
 * 检查OpenCV矩阵类型是否支持指定的颜色空间。
 *
 * 目前OpenCV矩阵类型仅支持最常用的8位深度，即CV_8UC[1/3/4]（HSV颜色空间仅支持CV_32FC3）。
 * 未知的颜色空间（EColor_Unknown）被认为支持所有OpenCV矩阵类型。
 * 未知的OpenCV矩阵类型（-1）被认为支持所有颜色空间。
 *
 * @param EColorSpace colorSpace	颜色空间枚举类型
 * @param int cvType				OpenCV矩阵类型
 *
 * @return bool OpenCV矩阵类型是否支持指定的颜色空间
 * @author Nianchen Deng
 */
bool ColorSpaceSupport (EColorSpace colorSpace, int cvType);



/**
 * 枚举类型EFormat的包装类，用于在图像中表示不同的像素格式。
 */
class PixelFormat {
public:
	/**
	 * 像素格式枚举变量
	 */
	enum EFormat {
		EFormat_Unknown = -1,	/* 未知 */
		EFormat_Mono8,			/* 8位单色（灰度），对应CV_8UC1 */
		EFormat_BGR,			/* 24位BGR彩色，对应CV_8UC3 */
		EFormat_BGR32,			/* 32位BGR彩色（带Alpha通道），对应CV_8UC4 */
		EFormat_RGB,			/* 24位RGB彩色，对应CV_8UC3 */
		EFormat_RGB32,			/* 32位RGB彩色（带Alpha通道），对应CV_8UC4 */
		EFormat_HSV				/* 3通道浮点数HSV彩色，对应CV_32FC3 */
	};

	/**
	 * EFormat枚举值构造PixelFormat。
	 *
	 * @param EFormat value （可选）值，默认为EFormat_Unknown
	 *
	 * @author Nianchen Deng
	 */
	PixelFormat (EFormat value = EFormat_Unknown);
	/**
	 * 根据颜色空间和OpenCV矩阵类型推导构造PixelFormat。
	 *
	 * 若指定的颜色空间不支持指定的OpenCV矩阵类型，则PixelFormat的值为EFormat_Unknown
	 *
	 * @param EColorSpace colorSpace	颜色空间
	 * @param int cvType				（可选）OpenCV矩阵类型。若未指定则自动推断像素格式，
	 *									即对于BGR和RGB默认为4通道类型。
	 *
	 * @author Nianchen Deng
	 */
	PixelFormat (EColorSpace colorSpace, int cvType = -1);
	/**
	 * 根据OpenCV矩阵类型推导构造PixelFormat。
	 *
	 * 若矩阵类型为未知（-1）或为不受支持的类型，则PixelFormat值为EFormat_Unknown
	 *
	 * @param int cvType OpenCV矩阵类型，对于CV_8UC3/CV_8UC4，默认为BGR/BGR32格式
	 *
	 * @author Nianchen Deng
	 */
	PixelFormat (int cvType);
	/**
	 * 重载赋值操作符。
	 *
	 * @param const PixelFormat& right 赋值操作符的右值
	 *
	 * @return PixelFormat&
	 * @author Nianchen Deng
	 */
	PixelFormat& operator= (const PixelFormat& right) = default;
	/**
	 * 重载赋值操作符。
	 *
	 * @param const PixelFormat& right 赋值操作符的右值
	 *
	 * @return PixelFormat&
	 * @author Nianchen Deng
	 */
	PixelFormat& operator= (EFormat right);

	/**
	 * EFormat隐式转换操作符。
	 *
	 * 此操作符保证PixelFormat可被当成普通的枚举类型EFormat使用在等值判断或switch语句中。
	 *
	 * @return EFormat 对应的枚举值 
	 * @author Nianchen Deng
	 */
	operator EFormat () const;

	/**
	 * 判断像素格式是否与指定OpenCV矩阵类型匹配。
	 *
	 * 未知像素格式（EFormat_Unknown）或OpenCV矩阵类型（-1）被认为与任何OpenCV
	 * 矩阵类型或像素格式匹配。
	 *
	 * @param int cvType 指定OpenCV矩阵类型
	 *
	 * @return bool 像素格式是否与指定OpenCV矩阵类型匹配
	 * @author Nianchen Deng
	 */
	bool MATCH (int cvType) const;
	/**
	 * 获取像素格式所对应的颜色空间。
	 *
	 * @return EColorSpace 对应的颜色空间
	 * @author Nianchen Deng
	 */
	EColorSpace COLOR_SPACE () const;
	/**
	 * 获取对应的OpenCV矩阵类型。
	 *
	 * 若像素格式为未知（EFormat_Unknown），则返回-1。
	 *
	 * @return int 对应的OpenCV矩阵类型
	 * @author Nianchen Deng
	 */
	int CV_TYPE () const;

	/**
	 * 根据指定的像素格式执行色彩转换。
	 *
	 * 该方法无法进行相同像素格式之间的转换，应由调用方自行判断源和目标的像素格式是否相同。
	 *
	 * @param const cv::Mat&	R_SRC		源图像数据
	 * @param cv::Mat&			r_dst		（输出）目标图像数据
	 * @param PixelFormat		srcFormat	源图像像素格式
	 * @param PixelFormat		dstFormat	目标图像像素格式，不可与源图像像素格式相同
	 *
	 * @throw Dlib::Basic::Exception::InvalidArgument 若所指定的格式与矩阵类型不匹配或相互之间无法进行转换
	 * @author Nianchen Deng
	 */
	static void CvtColor (const cv::Mat& R_SRC, cv::Mat& r_dst,
		PixelFormat srcFormat, PixelFormat dstFormat);
	
private:
	EFormat _value;

};

NS_DLIB_CVEXT_END