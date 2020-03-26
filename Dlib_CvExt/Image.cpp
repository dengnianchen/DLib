#include "stdafx.h"
#include "Image.h"

using namespace Dlib::Basic;



NS_DLIB_CVEXT_BEGIN

Image::Image (EColorSpace colorSpace) :
_acceptFormat (Accept_All),
_colorSpace (colorSpace),
_image () {
}



Image::Image (const cv::Mat& R_IMAGE, EColorSpace colorSpace) :
_acceptFormat (Accept_All),
_image (R_IMAGE) {

	if (!R_IMAGE.empty () && colorSpace == EColor_Unknown) {
		// δָ����ɫ�ռ䣬����ݾ��������Զ��Ƶ�
		switch (R_IMAGE.type ()) {
		case CV_32FC3: _colorSpace = EColor_HSV; break;
		case CV_8UC1: _colorSpace = EColor_Mono; break;
		case CV_8UC3:
		case CV_8UC4: _colorSpace = EColor_BGR; break;
		default:
			throw Exception::InvalidArgument (L"OpenCV type is not supported yet");
		}
	} else {
		if (ColorSpaceSupport (colorSpace, R_IMAGE.empty () ? -1 : R_IMAGE.type ()))
			throw Exception::InvalidArgument (L"Image does not support specified color space");
		_colorSpace = colorSpace;
	}

}



Image::Image (PixelFormat format, const cv::Mat& R_IMAGE) :
_image (R_IMAGE.empty () ? cv::Mat (1, 1, format.CV_TYPE ()) : R_IMAGE),
_acceptFormat (Accept_All) {

	_colorSpace = format.COLOR_SPACE ();
	if (!ColorSpaceSupport (_colorSpace, R_IMAGE.empty () ? -1 : R_IMAGE.type ()))
		throw Exception::InvalidArgument (L"Image does not support specified color space");

}



Image::Image (PixelFormat format, cv::Size size) :
_image (cv::Mat (size, format == PixelFormat::EFormat_Unknown ? CV_8UC1 : format.CV_TYPE ())),
_acceptFormat (Accept_All) {
	
	_colorSpace = format.COLOR_SPACE ();

}



Image Image::concat (const std::initializer_list<Image>& R_IMAGES, bool horizontal) {

	if (R_IMAGES.size () == 0) return Image ();
	if (R_IMAGES.size () == 1) return *R_IMAGES.begin ();

	// ͳһ��ʽ
	auto format = PixelFormat::EFormat_Mono8;
	std::vector<Image> images (R_IMAGES);
	for (auto i = 0; i < images.size (); ++i) {
		if (images[i].FORMAT () != PixelFormat::EFormat_Mono8)
			format = PixelFormat::EFormat_BGR32;
	}
	for (auto i = 0; i < images.size (); ++i) {
		if (images[i].FORMAT () != format) {
			Image temp (format);
			images[i].CONVERT_TO (temp);
			images[i] = temp;
		}
	}

	// ͳһ�߶�/��Ȳ�ƴ��
	std::vector<cv::Mat> images_;
	Image outImage (format);
	if (horizontal) {
		auto maxHeight = 0;
		for (auto i = 0; i < images.size (); ++i) {
			if (images[i].HEIGHT () > maxHeight)
				maxHeight = images[i].HEIGHT ();
		}
		for (auto i = 0; i < images.size (); ++i) {
			if (images[i].HEIGHT () == maxHeight) {
				images_.push_back (images[i]);
				continue;
			}
			Image temp (format, cv::Size (images[i].WIDTH (), maxHeight));
			auto roi = ((cv::Mat)temp) ({
				{ 0, (maxHeight - images[i].HEIGHT ()) / 2 },
				images[i].SIZE ()
			});
			images[i]->copyTo (roi);
			images_.push_back (temp);
		}
		cv::hconcat (images_, outImage);
	} else {
		auto maxWidth = 0;
		for (auto i = 0; i < images.size (); ++i) {
			if (images[i].WIDTH () > maxWidth)
				maxWidth = images[i].WIDTH ();
		}
		for (auto i = 0; i < images.size (); ++i) {
			if (images[i].WIDTH () == maxWidth) {
				images_.push_back (images[i]);
				continue;
			}
			Image temp (format, cv::Size (maxWidth, images[i].HEIGHT ()));
			auto roi = ((cv::Mat)temp) ({
				{ (maxWidth - images[i].WIDTH ()) / 2, 0 },
				images[i].SIZE ()
				});
			images[i]->copyTo (roi);
			images[i] = temp;
		}
		cv::vconcat (images_, outImage);
	}
	return outImage;

}

Image::Image (const cv::Mat& R_IMAGE, _AcceptFormat acceptFormat) :
_image (R_IMAGE),
_acceptFormat (acceptFormat),
_colorSpace (_ToColorSpace (acceptFormat)) {

	if (!_IsAcceptable (acceptFormat, R_IMAGE.empty () ? -1 : R_IMAGE.type ()))
		throw Exception::InvalidArgument (L"Image format is not acceptable");

}



Image::Image (const Image& R_IMAGE, _AcceptFormat acceptFormat) :
_image (R_IMAGE),
_acceptFormat (acceptFormat),
_colorSpace (_ToColorSpace (acceptFormat)) {

	if (!_IsAcceptable (acceptFormat, R_IMAGE.FORMAT ()))
		throw Exception::InvalidArgument (L"Image format is not acceptable");

}



Image::Image (const cv::Size& R_SIZE, _AcceptFormat acceptFormat) :
_acceptFormat (acceptFormat),
_colorSpace (_ToColorSpace (acceptFormat)) {

	auto format = FORMAT ();
	if (format == PixelFormat::EFormat_Unknown)
		throw Exception::InvalidArgument (L"Pixel format cannot be determined by specified acceptFormat");
	_image = cv::Mat (R_SIZE, format.CV_TYPE ());

}



Image::Image (const cv::Size& R_SIZE, Color color, _AcceptFormat acceptFormat) :
_acceptFormat (acceptFormat),
_colorSpace (_ToColorSpace (acceptFormat)) {

	auto format = FORMAT ();
	if (format == PixelFormat::EFormat_Unknown)
		throw Exception::InvalidArgument (L"Pixel format cannot be determined by specified acceptFormat");
	_image = cv::Mat (R_SIZE, format.CV_TYPE (), color (_colorSpace));

}



Image Image::CLONE () const {

	auto cloned = (*this);
	return cloned._image = _image.clone (), cloned;

}



Image Image::SUB_IMAGE (const cv::Rect& R_ROI) const {

	auto subImage = (*this);
	return subImage._image = _image(R_ROI), subImage;

}



void Image::CONVERT_TO (Image& r_target) const {

	if (FORMAT () == r_target.FORMAT ())
		_image.copyTo (r_target);
	else
		PixelFormat::CvtColor (_image, r_target, FORMAT (), r_target.FORMAT ());

}



PixelFormat Image::FORMAT () const {

	// ������Ϊ�գ�ͨ��_acceptFormat��_colorSpace�ƶ����ظ�ʽ
	if (_image.empty ()) {
		switch (_acceptFormat) {
		case Accept_Mono8: return PixelFormat::EFormat_Mono8;
		case Accept_HSV: return PixelFormat::EFormat_HSV;
		case Accept_BGR8: return PixelFormat::EFormat_BGR;
		case Accept_BGRA8: return PixelFormat::EFormat_BGR32;
		case Accept_RGB8: return PixelFormat::EFormat_RGB;
		case Accept_RGBA8: return PixelFormat::EFormat_RGB;
		case Accept_BGR8_BGRA8:
		case Accept_RGB8_RGBA8: return PixelFormat::EFormat_Unknown;
		default: // ��_acceptFormat = Accept_All����ͨ��_colorSpace�ƶ����ظ�ʽ
			switch (_colorSpace) {
			case EColor_Mono: return PixelFormat::EFormat_Mono8;
			case EColor_HSV: return PixelFormat::EFormat_HSV;
			default: return PixelFormat::EFormat_Unknown;
			}
		}
	}
	// ͨ����ɫ�ռ�;��������ƶ����ظ�ʽ
	return { _colorSpace, _image.type () };

}



bool Image::_IsAcceptable (_AcceptFormat acceptFormat, int cvType) {

	if (cvType == -1)
		return true;
	switch (acceptFormat) {
	case Accept_Mono8: return cvType == CV_8UC1;
	case Accept_BGR8:
	case Accept_RGB8: return cvType == CV_8UC3;
	case Accept_BGRA8:
	case Accept_RGBA8: return cvType == CV_8UC4;
	case Accept_BGR8_BGRA8:
	case Accept_RGB8_RGBA8: return cvType == CV_8UC3 || cvType == CV_8UC4;
	case Accept_HSV: return cvType == CV_32FC3;
	default: return true;
	}

}

bool Image::_IsAcceptable (_AcceptFormat acceptFormat, PixelFormat format) {

	if (format == PixelFormat::EFormat_Unknown)
		return true;
	switch (acceptFormat) {
	case Accept_Mono8: return format == PixelFormat::EFormat_Mono8;
	case Accept_BGR8: return format == PixelFormat::EFormat_BGR;
	case Accept_RGB8: return format == PixelFormat::EFormat_RGB;
	case Accept_BGRA8: return format == PixelFormat::EFormat_BGR32;
	case Accept_RGBA8: return format == PixelFormat::EFormat_RGB32;
	case Accept_BGR8_BGRA8: return format == PixelFormat::EFormat_BGR || PixelFormat::EFormat_RGB;
	case Accept_RGB8_RGBA8: return format == PixelFormat::EFormat_BGR32 || PixelFormat::EFormat_RGB32;
	case Accept_HSV: return format == PixelFormat::EFormat_HSV;
	default: return true;
	}

}



EColorSpace Image::_ToColorSpace (_AcceptFormat acceptFormat) {

	switch (acceptFormat) {
	case Accept_Mono8: return EColor_Mono;
	case Accept_HSV: return EColor_HSV;
	case Accept_BGR8:
	case Accept_BGRA8:
	case Accept_BGR8_BGRA8: return EColor_BGR;
	case Accept_RGB8:
	case Accept_RGBA8:
	case Accept_RGB8_RGBA8: return EColor_RGB;
	default: return EColor_Unknown;
	}

}

NS_DLIB_CVEXT_END
