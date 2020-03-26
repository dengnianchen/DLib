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
	 * ��ɫ��������ɫ��BGRA��255 255 255 255��
	 */
	const static Color White;
	/**
	 * ��ɫ��������ɫ��BGRA��0 0 0 255��
	 */
	const static Color Black;
	/**
	 * ��ɫ��������ɫ��BGRA��0 0 255 255��
	 */
	const static Color Red;
	/**
	 * ��ɫ��������ɫ��BGRA��0 255 0 255��
	 */
	const static Color Green;
	/**
	 * ��ɫ��������ɫ��BGRA��255 0 0 255��
	 */
	const static Color Blue;
	/**
	 * ��ɫ��������ɫ��BGRA��0 255 255 255��
	 */
	const static Color Yellow;
	/**
	 * ��ɫ��������ɫ��BGRA��255 255 0 255��
	 */
	const static Color Cyan;
	/**
	 * ��ɫ��������ɫ��BGRA��255 0 255 255��
	 */
	const static Color Purple;
	/**
	 * ��ɫ��������ɫ��BGRA��0 0 0 0��
	 */
	const static Color Zero;

	Color () : _value (), _colorSpace (EColor_BGR) { }

	/**
	 * ���ݷ���ֵ����ɫ�ռ乹��Color����
	 *
	 * @param cv::Scalar value			ֵ
	 * @param EColorSpace colorSpace	��ɫ�ռ�
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
	 * ����ǰ��ɫת����ָ������ɫ�ռ䡣
	 *
	 * @param EColorSpace colorSpace ָ����ɫ�ռ�
	 *
	 * @return Color ת�������ɫ����
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
	 * ������ɫ���Ҷȣ���ɫ����
	 *
	 * @param uchar value �Ҷ�ֵ
	 *
	 * @return Color ��ɫ����
	 * @author Nianchen Deng
	 */
	static Color Mono (uchar value) { return Color (cv::Scalar (value), EColor_Mono); }

	/**
	 * ����BGR��ɫ�ռ����ɫ����
	 *
	 * @param uchar b ��ɫ����
	 * @param uchar g ��ɫ����
	 * @param uchar r ��ɫ����
	 * @param uchar a ����ѡ��Alpha������Ĭ��Ϊ255
	 *
	 * @return Color ��ɫ����
	 * @author Nianchen Deng
	 */
	static Color BGR (uchar b, uchar g, uchar r, uchar a = 255) { return Color (cv::Scalar (b, g, r, a), EColor_BGR); }

	/**
	 * ����RGB��ɫ�ռ����ɫ����
	 *
	 * @param uchar r ��ɫ����
	 * @param uchar b ��ɫ����
	 * @param uchar g ��ɫ����
	 * @param uchar a ����ѡ��Alpha������Ĭ��Ϊ255
	 *
	 * @return Color ��ɫ����
	 * @author Nianchen Deng
	 */
	static Color RGB (uchar r, uchar g, uchar b, uchar a = 255) { return Color (cv::Scalar (r, g, b, a), EColor_RGB); }

	/**
	 * ����HSV��ɫ�ռ����ɫ����
	 *
	 * @param float h ɫ��������0-360��
	 * @param float s ���Ͷȷ�����0-1��
	 * @param float v ���ȷ�����0-1��
	 *
	 * @return Color ��ɫ����
	 */
	static Color HSV (float h, float s, float v) { return Color (cv::Scalar (h, s, v), EColor_HSV); }

	/**
	 * ��HSV��ɫ�ռ��д��������ɫ
	 *
	 * @param cv::Vec3f min ���������ֵ����Сֵ
	 * @param cv::Vec3f max ���������ֵ�����ֵ
	 *
	 * @return Color ��ɫ����
	 */
	static Color RandomHSV (cv::Vec3f min = { }, cv::Vec3f max = { 360.0f, 1.0f, 1.0f }) {
		return Color::HSV (
			getRandom (min[0], max[0]),
			getRandom (min[1], max[1]),
			getRandom (min[2], max[2]));
	}

	/**
	 * ��BGR��ɫ�ռ��д��������ɫ
	 *
	 * @param cv::Vec3b min ���������ֵ����Сֵ
	 * @param cv::Vec3b max ���������ֵ�����ֵ
	 *
	 * @return Color ��ɫ����
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