#pragma once
#include "global.h"
#include "PixelFormat.h"
#include "Color.h"
#include "PixelOp.h"



#define DLIB_FINAL_IMAGE_TYPE(__Class__, __Base__, __Accept__) \
	class __Class__ : public __Base__ { \
	public: \
		__Class__ (const cv::Mat& R_IMAGE = {}) :        __Base__ (R_IMAGE, __Accept__) {} \
		__Class__ (const Image& R_IMAGE) :               __Base__ (R_IMAGE, __Accept__) {} \
		__Class__ (int width, int height) :              __Base__ (cv::Size (width, height), __Accept__) {} \
		__Class__ (int width, int height, Color color) : __Base__ (cv::Size (width, height), color, __Accept__) {} \
		__Class__ (cv::Size size) :                      __Base__ (size, __Accept__) {} \
		__Class__ (cv::Size size, Color color) :         __Base__ (size, color, __Accept__) {} \
		__Class__ CLONE () const { auto cloned = (*this); return cloned._image = _image.clone (), cloned; } \
	}
#define DLIB_INTERMEDIATE_IMAGE_TYPE(__Class__, __Base__, __Accept__) \
	class __Class__ : public __Base__ { \
	public: \
		__Class__ (const cv::Mat& R_IMAGE = {}) : __Base__ (R_IMAGE, __Accept__) {} \
		__Class__ (const Image& R_IMAGE) :        __Base__ (R_IMAGE, __Accept__) {} \
	protected: \
		__Class__ (const cv::Mat& R_IMAGE, _AcceptFormat acceptFormat) :              __Base__ (R_IMAGE, acceptFormat) {} \
		__Class__ (const Image& R_IMAGE, _AcceptFormat acceptFormat) :                __Base__ (R_IMAGE, acceptFormat) {} \
		__Class__ (const cv::Size& R_SIZE, _AcceptFormat acceptFormat) :              __Base__ (R_SIZE, acceptFormat) {} \
		__Class__ (const cv::Size& R_SIZE, Color color, _AcceptFormat acceptFormat) : __Base__ (R_SIZE, color, acceptFormat) {} \
		__Class__ CLONE () const { auto cloned = (*this); return cloned._image = _image.clone (), cloned; } \
	}



NS_DLIB_CVEXT_BEGIN

enum _AcceptFormat {
	Accept_All = 0xFF,
	Accept_Mono8 = 0x01,
	Accept_BGR8 = 0x02,
	Accept_BGRA8 = 0x04,
	Accept_RGB8 = 0x08,
	Accept_RGBA8 = 0x10,
	Accept_HSV = 0x20,
	Accept_BGR8_BGRA8 = Accept_BGR8 | Accept_BGRA8,
	Accept_RGB8_RGBA8 = Accept_RGB8 | Accept_RGBA8
};



class Image {
public:
	Image (EColorSpace colorSpace = EColor_Unknown);
	Image (const cv::Mat& R_IMAGE, EColorSpace colorSpace = EColor_Unknown);
	Image (PixelFormat format, const cv::Mat& R_IMAGE = {});
	Image (PixelFormat format, cv::Size size);

	Image CLONE () const;
	Image SUB_IMAGE (const cv::Rect& R_ROI) const;
	void CONVERT_TO (Image& r_target) const;

	PixelFormat FORMAT () const;
	cv::Size SIZE () const { return _image.size (); }
	int WIDTH () const { return _image.cols; }
	int HEIGHT () const { return _image.rows; }
	EColorSpace COLOR_SPACE () const { return _colorSpace; }
	bool IS_EMPTY () const { return _image.empty (); }

	Color GET (int x, int y) const { return GET ({ x, y }); }
	Color GET (cv::Point p) const { return { GetPixel (_image, p), _colorSpace }; }
	void set (int x, int y, Color c) { set ({ x, y }, c); }
	void set (cv::Point p, Color c) { SetPixel (_image, p, c (_colorSpace)); }

	operator cv::Mat& () { return _image; }
	operator const cv::Mat& () const { return _image; }
	operator cv::_InputArray () const { return _image; }
	operator cv::_InputOutputArray () { return _image; }
	cv::Mat* operator-> () { return &_image; }
	const cv::Mat* operator-> () const { return &_image; }

	static Image concat (const std::initializer_list<Image>& R_IMAGES, bool horizontal = true);

protected:
	_AcceptFormat _acceptFormat;
	EColorSpace _colorSpace;
	cv::Mat _image;

	Image (const cv::Mat& R_IMAGE, _AcceptFormat acceptFormat);
	Image (const Image& R_IMAGE, _AcceptFormat acceptFormat);
	Image (const cv::Size& R_SIZE, _AcceptFormat acceptFormat);
	Image (const cv::Size& R_SIZE, Color color, _AcceptFormat acceptFormat);

	static bool _IsAcceptable (_AcceptFormat acceptFormat, int cvType);
	static bool _IsAcceptable (_AcceptFormat acceptFormat, PixelFormat format);
	static EColorSpace _ToColorSpace (_AcceptFormat acceptFormat);

};



DLIB_INTERMEDIATE_IMAGE_TYPE (ImageBGR24_32, Image, Accept_BGR8_BGRA8);
DLIB_INTERMEDIATE_IMAGE_TYPE (ImageRGB24_32, Image, Accept_RGB8_RGBA8);
DLIB_FINAL_IMAGE_TYPE (Image8, Image, Accept_Mono8);
DLIB_FINAL_IMAGE_TYPE (ImageHSV, Image, Accept_HSV);
DLIB_FINAL_IMAGE_TYPE (ImageBGR24, ImageBGR24_32, Accept_BGR8);
DLIB_FINAL_IMAGE_TYPE (ImageBGR32, ImageBGR24_32, Accept_BGRA8);
DLIB_FINAL_IMAGE_TYPE (ImageRGB24, ImageRGB24_32, Accept_RGB8);
DLIB_FINAL_IMAGE_TYPE (ImageRGB32, ImageRGB24_32, Accept_RGBA8);
/*
class Image8 : public Image {
public:
	Image8 (const cv::Mat& R_IMAGE = {}) :        Image (R_IMAGE, Accept_Mono8) {}
	Image8 (const Image& R_IMAGE) :               Image (R_IMAGE, Accept_Mono8) {}
	Image8 (int width, int height) :              Image ({ width, height }, Accept_Mono8) {}
	Image8 (int width, int height, Color color) : Image ({ width, height }, color, Accept_Mono8) {}
	Image8 (cv::Size size) :                      Image (size, Accept_Mono8) {}
	Image8 (cv::Size size, Color color) :         Image (size, color, Accept_Mono8) {}

};



class ImageHSV : public Image {
public:
	ImageHSV (const cv::Mat& R_IMAGE = {}) :        Image (R_IMAGE, Accept_HSV) {}
	ImageHSV (const Image& R_IMAGE) :               Image (R_IMAGE, Accept_HSV) {}
	ImageHSV (int width, int height) :              Image ({ width, height }, Accept_HSV) {}
	ImageHSV (int width, int height, Color color) : Image ({ width, height }, color, Accept_HSV) {}
	ImageHSV (cv::Size size) :                      Image (size, Accept_HSV) {}
	ImageHSV (cv::Size size, Color color) :         Image (size, color, Accept_HSV) {}

};



class ImageBGR24_32 : public Image {
public:
	ImageBGR24_32 (const cv::Mat& R_IMAGE = {}) : Image (R_IMAGE, Accept_BGR8_BGRA8) {}
	ImageBGR24_32 (const Image& R_IMAGE) :        Image (R_IMAGE, Accept_BGR8_BGRA8) {}

protected:
	ImageBGR24_32 (const cv::Mat& R_IMAGE, _AcceptFormat acceptFormat) :              Image (R_IMAGE, acceptFormat) {}
	ImageBGR24_32 (const Image& R_IMAGE, _AcceptFormat acceptFormat) :                Image (R_IMAGE, acceptFormat) {}
	ImageBGR24_32 (const cv::Size& R_SIZE, _AcceptFormat acceptFormat) :              Image (R_SIZE, acceptFormat) {}
	ImageBGR24_32 (const cv::Size& R_SIZE, Color color, _AcceptFormat acceptFormat) : Image (R_SIZE, color, acceptFormat) {}

};



class ImageBGR24 : public ImageBGR24_32 {
public:
	ImageBGR24 (const cv::Mat& R_IMAGE = {}) :        ImageBGR24_32 (R_IMAGE, Accept_BGR8) {}
	ImageBGR24 (const Image& R_IMAGE) :               ImageBGR24_32 (R_IMAGE, Accept_BGR8) {}
	ImageBGR24 (int width, int height) :              ImageBGR24_32 ({ width, height }, Accept_BGR8) {}
	ImageBGR24 (int width, int height, Color color) : ImageBGR24_32 ({ width, height }, color, Accept_BGR8) {}
	ImageBGR24 (cv::Size size) :                      ImageBGR24_32 (size, Accept_BGR8) {}
	ImageBGR24 (cv::Size size, Color color) :         ImageBGR24_32 (size, color, Accept_BGR8) {}

};



class ImageBGR32 : public ImageBGR24_32 {
public:
	ImageBGR32 (const cv::Mat& R_IMAGE = {}) :        ImageBGR24_32 (R_IMAGE, Accept_BGRA8) {}
	ImageBGR32 (const Image& R_IMAGE) :               ImageBGR24_32 (R_IMAGE, Accept_BGRA8) {}
	ImageBGR32 (int width, int height) :              ImageBGR24_32 ({ width, height }, Accept_BGRA8) {}
	ImageBGR32 (int width, int height, Color color) : ImageBGR24_32 ({ width, height }, color, Accept_BGRA8) {}
	ImageBGR32 (cv::Size size) :                      ImageBGR24_32 (size, Accept_BGRA8) {}
	ImageBGR32 (cv::Size size, Color color) :         ImageBGR24_32 (size, color, Accept_BGRA8) {}

};



class ImageRGB24_32 : public Image {
public:
	ImageRGB24_32 (const cv::Mat& R_IMAGE = {}) : Image (R_IMAGE, Accept_RGB8_RGBA8) {}
	ImageRGB24_32 (const Image& R_IMAGE) :        Image (R_IMAGE, Accept_RGB8_RGBA8) {}

protected:
	ImageRGB24_32 (const cv::Mat& R_IMAGE, _AcceptFormat acceptFormat) :              Image (R_IMAGE, acceptFormat) {}
	ImageRGB24_32 (const Image& R_IMAGE, _AcceptFormat acceptFormat) :                Image (R_IMAGE, acceptFormat) {}
	ImageRGB24_32 (const cv::Size& R_SIZE, _AcceptFormat acceptFormat) :              Image (R_SIZE, acceptFormat) {}
	ImageRGB24_32 (const cv::Size& R_SIZE, Color color, _AcceptFormat acceptFormat) : Image (R_SIZE, color, acceptFormat) {}

};



class ImageRGB24 : public ImageRGB24_32 {
public:
	ImageRGB24 (const cv::Mat& R_IMAGE = {}) :        ImageRGB24_32 (R_IMAGE, Accept_RGB8) {}
	ImageRGB24 (const Image& R_IMAGE) :               ImageRGB24_32 (R_IMAGE, Accept_RGB8) {}
	ImageRGB24 (int width, int height) :              ImageRGB24_32 ({ width, height }, Accept_RGB8) {}
	ImageRGB24 (int width, int height, Color color) : ImageRGB24_32 ({ width, height }, color, Accept_RGB8) {}
	ImageRGB24 (cv::Size size) :                      ImageRGB24_32 (size, Accept_RGB8) {}
	ImageRGB24 (cv::Size size, Color color) :         ImageRGB24_32 (size, color, Accept_RGB8) {}

};



class ImageRGB32 : public ImageRGB24_32 {
public:
	ImageRGB32 (const cv::Mat& R_IMAGE = {}) :        ImageRGB24_32 (R_IMAGE, Accept_RGBA8) {}
	ImageRGB32 (const Image& R_IMAGE) :               ImageRGB24_32 (R_IMAGE, Accept_RGBA8) {}
	ImageRGB32 (int width, int height) :              ImageRGB24_32 ({ width, height }, Accept_RGBA8) {}
	ImageRGB32 (int width, int height, Color color) : ImageRGB24_32 ({ width, height }, color, Accept_RGBA8) {}
	ImageRGB32 (cv::Size size) :                      ImageRGB24_32 (size, Accept_RGBA8) {}
	ImageRGB32 (cv::Size size, Color color) :         ImageRGB24_32 (size, color, Accept_RGBA8) {}

};
*/

NS_DLIB_CVEXT_END