#include "stdafx.h"
#include "PixelFormat.h"

using namespace Dlib::Basic;



NS_DLIB_CVEXT_BEGIN

const static int _ColorCvtCodeTable[6][6] = {
	{ -1, cv::COLOR_GRAY2BGR, cv::COLOR_GRAY2BGRA, cv::COLOR_GRAY2RGB, cv::COLOR_GRAY2RGBA, -1 },
	{ cv::COLOR_BGR2GRAY, -1, cv::COLOR_BGR2BGRA, cv::COLOR_BGR2RGB, cv::COLOR_BGR2RGBA, cv::COLOR_BGR2HSV },
	{ cv::COLOR_BGRA2GRAY, cv::COLOR_BGRA2BGR, -1, cv::COLOR_BGRA2RGB, cv::COLOR_BGRA2RGBA, cv::COLOR_BGR2HSV },
	{ cv::COLOR_RGB2GRAY, cv::COLOR_RGB2BGR, cv::COLOR_RGB2BGRA, -1, cv::COLOR_RGB2RGBA, cv::COLOR_RGB2HSV },
	{ cv::COLOR_RGBA2GRAY, cv::COLOR_RGBA2BGR, cv::COLOR_RGBA2BGRA, cv::COLOR_RGBA2RGB, -1, cv::COLOR_BGR2HSV },
	{ -1, cv::COLOR_HSV2BGR, cv::COLOR_HSV2BGR, cv::COLOR_HSV2RGB, cv::COLOR_HSV2BGR, -1 },
};

const static int _CvFormatTable[6] = {
	CV_8U, CV_8UC3, CV_8UC4, CV_8UC3, CV_8UC4, CV_32FC3
};



bool ColorSpaceSupport (EColorSpace colorSpace, int cvType) {

	if (colorSpace == EColor_Unknown || cvType == -1)
		return true;
	switch (colorSpace) {
	case EColor_Mono: return cvType == CV_8UC1;
	case EColor_BGR: 
	case EColor_RGB: return cvType == CV_8UC3 || cvType == CV_8UC4;
	case EColor_HSV: return cvType == CV_32FC3;
	default: return false;
	}

}



PixelFormat::PixelFormat (EFormat value) :
_value (value) {

}



PixelFormat::PixelFormat (EColorSpace colorSpace, int cvType/* = -1*/) {

	switch (colorSpace) {
	case EColor_Mono: _value = EFormat_Mono8; break;
	case EColor_BGR: _value = cvType == CV_8UC3 ? EFormat_BGR : EFormat_BGR32; break;
	case EColor_RGB: _value = cvType == CV_8UC3 ? EFormat_RGB : EFormat_RGB32; break;
	case EColor_HSV: _value = EFormat_HSV; break;
	default: _value = EFormat_Unknown; break;
	}
	if (!MATCH (cvType))
		_value = EFormat_Unknown;

}



PixelFormat::PixelFormat (int cvType) {

	switch (cvType) {
	case CV_8UC1: _value = EFormat_Mono8; break;
	case CV_8UC3: _value = EFormat_BGR; break;
	case CV_8UC4: _value = EFormat_BGR32; break;
	case CV_32FC3: _value = EFormat_HSV; break;
	default: _value = EFormat_Unknown; break;
	}

}



PixelFormat& PixelFormat::operator= (EFormat right) {

	_value = right;
	return *this;

}



PixelFormat::operator EFormat () const {
	
	return _value;

}



bool PixelFormat::MATCH (int cvType) const {

	if (_value == EFormat_Unknown || cvType == -1)
		return true;
	return _CvFormatTable[_value] == cvType;

}



EColorSpace PixelFormat::COLOR_SPACE () const {
	
	switch (_value) {
	case EFormat_Mono8: return EColor_Mono;
	case EFormat_BGR32: case EFormat_BGR: return EColor_BGR;
	case EFormat_RGB32: case EFormat_RGB: return EColor_RGB;
	case EFormat_HSV: return EColor_HSV;
	default: return EColor_Unknown;
	}

}



int PixelFormat::CV_TYPE () const {

	return _value == EFormat_Unknown ? -1 : _CvFormatTable[_value];

}



void PixelFormat::CvtColor (const cv::Mat& R_SRC, cv::Mat& r_dst,
	PixelFormat srcFormat, PixelFormat dstFormat) {

	if (srcFormat == EFormat_Unknown || dstFormat == EFormat_Unknown)
		throw Exception::InvalidArgument (L"Specified format cannot be EFormat_Unknown");
	if (!srcFormat.MATCH (R_SRC.type ()) || !dstFormat.MATCH (r_dst.empty () ? -1 : r_dst.type ()))
		throw Exception::InvalidArgument (L"Format mismatch with OpenCV type");
	if (_ColorCvtCodeTable[srcFormat][dstFormat] == -1)
		throw Exception::InvalidArgument (L"Cannot convert between specified formats");
	cv::cvtColor (R_SRC, r_dst, _ColorCvtCodeTable[srcFormat][dstFormat],
		CV_MAT_CN (dstFormat.CV_TYPE ()));

}

NS_DLIB_CVEXT_END