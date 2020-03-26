#pragma once
#include "global.h"
#include "PixelFormat.h"
#include "MathEx.h"

#ifdef RGB
#undef RGB
#endif // RGB



NS_DLIB_CVEXT_BEGIN

class Color {
public:
	/**
	 * 颜色常量：白色（BGRA：255 255 255 255）
	 */
	const static Color White;
	/**
	 * 颜色常量：黑色（BGRA：0 0 0 255）
	 */
	const static Color Black;
	/**
	 * 颜色常量：红色（BGRA：0 0 255 255）
	 */
	const static Color Red;
	/**
	 * 颜色常量：绿色（BGRA：0 255 0 255）
	 */
	const static Color Green;
	/**
	 * 颜色常量：蓝色（BGRA：255 0 0 255）
	 */
	const static Color Blue;
	/**
	 * 颜色常量：黄色（BGRA：0 255 255 255）
	 */
	const static Color Yellow;
	/**
	 * 颜色常量：青色（BGRA：255 255 0 255）
	 */
	const static Color Cyan;
	/**
	 * 颜色常量：紫色（BGRA：255 0 255 255）
	 */
	const static Color Purple;
	/**
	 * 颜色常量：无色（BGRA：0 0 0 0）
	 */
	const static Color Zero;

	Color () : _value (), _colorSpace (EColor_BGR) { }

	/**
	 * 根据分量值和颜色空间构造Color对象
	 *
	 * @param cv::Scalar value			值
	 * @param EColorSpace colorSpace	颜色空间
	 *
	 * @author Nianchen Deng
	 */
	Color (cv::Scalar value, EColorSpace colorSpace) : _value (value), _colorSpace (colorSpace) { }

	uchar ValueMono () {
		return (uchar)(*this)(EColor_Mono)[0];
	}

	cv::Vec3b ValueBGR () const {
		auto c = (*this)(EColor_BGR);
		return { (uchar)c[0], (uchar)c[1], (uchar)c[2] };
	}

	cv::Vec4b ValueBGR32 () const {
		auto c = (*this)(EColor_BGR);
		return { (uchar)c[0], (uchar)c[1], (uchar)c[2], (uchar)c[3] };
	}

	cv::Vec3b ValueRGB () const {
		auto c = (*this)(EColor_RGB); 
		return { (uchar)c[0], (uchar)c[1], (uchar)c[2] };
	}

	cv::Vec4b ValueRGB32 () const {
		auto c = (*this)(EColor_RGB);
		return { (uchar)c[0], (uchar)c[1], (uchar)c[2], (uchar)c[3] };
	}

	cv::Vec3f ValueHSV () const {
		auto c = (*this)(EColor_HSV);
		return { (float)c[0], (float)c[1], (float)c[2] };
	}

	/**
	 * 将当前颜色转换到指定的颜色空间。
	 *
	 * @param EColorSpace colorSpace 指定颜色空间
	 *
	 * @return Color 转换后的颜色对象
	 * @author Nianchen Deng
	 */
	Color operator() (EColorSpace colorSpace) const;

	double operator[] (int i) const { return _value[i]; }

	Color operator+ (double s) const { return { _value + cv::Scalar (s, s, s, s), _colorSpace }; }
	Color operator+ (Color c) const { return { _value + c (_colorSpace)._value, _colorSpace }; }
	Color operator+ (cv::Scalar s) const { return { _value + s, _colorSpace }; }
	Color operator- (double s) const { return { _value - cv::Scalar (s, s, s, s), _colorSpace }; }
	Color operator- (Color c) const { return { _value - c (_colorSpace)._value, _colorSpace }; }
	Color operator- (cv::Scalar s) const { return { _value - s, _colorSpace }; }
	Color operator* (double s) const { return { _value * s, _colorSpace }; }
	Color operator/ (double s) const { return { _value / s, _colorSpace }; }

	Color& operator+= (double s) { return _value += { s, s, s, s }, *this; }
	Color& operator+= (Color c) { return _value += c (_colorSpace)._value, *this; }
	Color& operator+= (cv::Scalar s) { return _value += s, *this; }
	Color& operator-= (double s) { return _value -= { s, s, s, s }, *this; }
	Color& operator-= (Color c) { return _value -= c (_colorSpace)._value, *this; }
	Color& operator-= (cv::Scalar s) { return _value -= s, *this; }
	Color& operator*= (double s) { return _value *= s, *this; }
	Color& operator/= (double s) { return _value /= s, *this; }

	operator cv::Scalar () const { return _value; }

	/**
	 * 创建单色（灰度）颜色对象
	 *
	 * @param uchar value 灰度值
	 *
	 * @return Color 颜色对象
	 * @author Nianchen Deng
	 */
	static Color Mono (uchar value) { return Color (cv::Scalar (value), EColor_Mono); }

	/**
	 * 创建BGR彩色空间的颜色对象
	 *
	 * @param uchar b 蓝色分量
	 * @param uchar g 绿色分量
	 * @param uchar r 红色分量
	 * @param uchar a （可选）Alpha分量，默认为255
	 *
	 * @return Color 颜色对象
	 * @author Nianchen Deng
	 */
	static Color BGR (uchar b, uchar g, uchar r, uchar a = 255) { return Color (cv::Scalar (b, g, r, a), EColor_BGR); }

	/**
	 * 创建RGB彩色空间的颜色对象
	 *
	 * @param uchar r 红色分量
	 * @param uchar b 蓝色分量
	 * @param uchar g 绿色分量
	 * @param uchar a （可选）Alpha分量，默认为255
	 *
	 * @return Color 颜色对象
	 * @author Nianchen Deng
	 */
	static Color RGB (uchar r, uchar g, uchar b, uchar a = 255) { return Color (cv::Scalar (r, g, b, a), EColor_RGB); }

	/**
	 * 创建HSV彩色空间的颜色对象
	 *
	 * @param float h 色调分量（0-360）
	 * @param float s 饱和度分量（0-1）
	 * @param float v 明度分量（0-1）
	 *
	 * @return Color 颜色对象
	 */
	static Color HSV (float h, float s, float v) { return Color (cv::Scalar (h, s, v), EColor_HSV); }

	/**
	 * 在HSV彩色空间中创建随机颜色
	 *
	 * @param cv::Vec3f min 各分量随机值的最小值
	 * @param cv::Vec3f max 各分量随机值的最大值
	 *
	 * @return Color 颜色对象
	 */
	static Color RandomHSV (cv::Vec3f min = { }, cv::Vec3f max = { 360.0f, 1.0f, 1.0f }) {
		return Color::HSV (
			getRandom (min[0], max[0]),
			getRandom (min[1], max[1]),
			getRandom (min[2], max[2]));
	}

	/**
	 * 在BGR彩色空间中创建随机颜色
	 *
	 * @param cv::Vec3b min 各分量随机值的最小值
	 * @param cv::Vec3b max 各分量随机值的最大值
	 *
	 * @return Color 颜色对象
	 */
	static Color RandomBGR (cv::Vec3b min = { }, cv::Vec3b max = { 255, 255, 255 }) {
		return Color::BGR (
			getRandom (min[0], max[0]),
			getRandom (min[1], max[1]),
			getRandom (min[2], max[2]));
	}

private:
	cv::Scalar _value;
	EColorSpace _colorSpace;

};

NS_DLIB_CVEXT_END