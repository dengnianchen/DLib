#include "stdafx.h"
#include <sstream>
#include "../../Dlib_Basic/Basic.h"
#include "CvCamera.h"

using namespace Dlib::Basic;
using namespace Dlib::CvExt;
using namespace Dlib::Cam::Exception;



NS_DLIB_CAM_BEGIN

CvCamera::CvCamera (const Identifier& R_IDENTIFIER, CvCamSystem* p_system)
throw (VideoCameraException) :
	_identifier (R_IDENTIFIER),
	_format (PixelFormat::EFormat_Unknown),
	_status (EStatus_None) {
	
	init ();

}



CvCamera::~CvCamera () {

}



void CvCamera::init ()
throw (VideoCameraException) {

	if (_cap.isOpened ())
		return;
	if (!_cap.open (_identifier.ID))
		throw VideoCameraException (_identifier, "init",
			"Cannot open specified camera");
	_format = _CVFORMAT_TO_EFORMAT ((int)_cap.get (cv::CAP_PROP_FORMAT));
	_rawFrameSize = {
		int (_cap.get (cv::CAP_PROP_FRAME_WIDTH)),
		int (_cap.get (cv::CAP_PROP_FRAME_HEIGHT))
	};
	_status = EStatus_Initialized;

}



void CvCamera::start ()
throw (VideoCameraException) {

	init ();
	_grabbed = false;
	_status = EStatus_Streaming;

}



bool CvCamera::grabFrame ()
throw (VideoCameraException) {

	// ����ǰ�豸��������������״̬�����Ը����䵱ǰ״ִ̬����Ӧ��ǰ�ò���
	if (STATUS () != EStatus_Streaming)
		start ();

	_grabbed = _cap.grab ();
	return _grabbed;

}



bool CvCamera::retrieveFrame (CvExt::Image& o_image, unsigned long long& o_timestamp)
throw (VideoCameraException) {

	// ����ǰ�豸��������������״̬�����Ը����䵱ǰ״ִ̬����Ӧ��ǰ�ò���
	if (STATUS () != EStatus_Streaming)
		start ();


	// ���Բ����µĻ���֡
	cv::Mat rawImage;
	_grabbed ? _cap.retrieve (rawImage) : _cap.read (rawImage);
	_grabbed = false;
	if (rawImage.empty ())
		return false;

	// ��δָ������ߴ磬��Ĭ��Ϊ��ͬ�������
	if (_outputFrameSize.empty ())
		_outputFrameSize = rawImage.size ();
	PixelFormat rawFormat = _CVFORMAT_TO_EFORMAT ((int)_cap.get (cv::CAP_PROP_FORMAT));

	// �������ʽ�������С����ԭʼ��ͬ��������������ź͸�ʽת��
	if (_format == rawFormat && _outputFrameSize == rawImage.size ())
		o_image = CvExt::Image (_format, rawImage);
	else {
		// �����ͼ��ߴ���ʽ������Ҫ��������ͼ��ռ�
		if (o_image.SIZE () != _outputFrameSize || o_image.FORMAT () != _format)
			o_image = CvExt::Image (_format, _outputFrameSize);

		// �����ͼ����ԭʼͼ���С��ͬ�����ִ�и�ʽת��
		if (_outputFrameSize == rawImage.size ())
			PixelFormat::CvtColor (rawImage, o_image, rawFormat, _format);
		// �������ʽ��ԭʼ��ʽ��ͬ��������������
		else if (_format == rawFormat)
			cv::resize (rawImage, o_image, _outputFrameSize);
		// �����ͼ���ԭʼͼ��С������ִ��������ִ�и�ʽת��
		else if (_outputFrameSize.area () < rawImage.size ().area ()) {
			if (_tempImage.size () != _outputFrameSize ||
				_tempImage.type () != rawFormat.CV_TYPE ())
				_tempImage = cv::Mat (_outputFrameSize, rawFormat.CV_TYPE ());
			cv::resize (rawImage, _tempImage, _outputFrameSize);
			PixelFormat::CvtColor (_tempImage, o_image, rawFormat, _format);
		}
		// �����ͼ���ԭʼͼ�������ִ�и�ʽת����ִ������
		else { 
			if (_tempImage.size () != rawImage.size () ||
				_tempImage.type () != _format.CV_TYPE ())
				_tempImage = cv::Mat (rawImage.size (), _format.CV_TYPE ());
			PixelFormat::CvtColor (rawImage, _tempImage, rawFormat, _format);
			cv::resize (_tempImage, o_image, _outputFrameSize);
		}
	}
	// ����ʱ���
	o_timestamp = GetTimestamp ();
	return true;

}



void CvCamera::stop () {

	_cap.release ();
	_status = EStatus_Initialized;

}



std::string CvCamera::DEVICE_INFO () const {
	
	std::ostringstream sout;
	sout << "*** Cv Camera Information ***\n\n";
	if (_status != EStatus_None) {
		sout << "Backend Name: " << _cap.getBackendName () << "\n";
		sout << "Width: " << _cap.get (cv::CAP_PROP_FRAME_WIDTH) << "\n";
		sout << "Height: " << _cap.get (cv::CAP_PROP_FRAME_HEIGHT) << "\n";
		sout << "FPS: " << _cap.get (cv::CAP_PROP_FPS) << "\n";
	} else
		sout << "Cv Camera is not opened.\n";
	sout << "\n*****************************\n";
	return sout.str ();

}



EStatus CvCamera::STATUS () const {

	return _status;

}



cv::Size CvCamera::FRAME_SIZE () const {

	if (!_outputFrameSize.empty ())
		return _outputFrameSize;
	return _rawFrameSize;

}



void CvCamera::setFrameSize (cv::Size frameSize) {

	_outputFrameSize = frameSize;

}



PixelFormat CvCamera::FORMAT () const {

	return _format;

}



bool CvCamera::setFormat (PixelFormat format) {

	if (format == _format)
		return true;
	_cap.set (cv::CAP_PROP_FORMAT, format.CV_TYPE ());
	_format = format;
	return true;

}



Property::Range CvCamera::PROPERTY_RANGE (Property::EType type) const {

	return {};

}



Property CvCamera::PROPERTY_VALUE (Property::EType type) const {

	switch (type) {
	case Property::EType_FrameRate:
		return {
			false,
			(float)_cap.get (cv::CAP_PROP_FPS)
		};
	case Property::EType_ExposureTime:
		return {
			(bool)_cap.get (cv::CAP_PROP_AUTO_EXPOSURE),
			(float)_cap.get (cv::CAP_PROP_EXPOSURE)
		};
	case Property::EType_Gain:
		return {
			false,
			(float)_cap.get (cv::CAP_PROP_GAIN)
		};
	default:
		return {};
	}

}



bool CvCamera::setPropertyValue (Property::EType type, Property value) {

	//throw std::exception ("CvCamera::setPropertyValue is not implemented yet.");
	return false;
	/*
	switch (type) {
	case Property::EType_FrameRate: {
		CBooleanPtr p_acqFrameRateEnable = _p_cam->GetNodeMap ().GetNode (
			"AcquisitionFrameRateEnable");
		p_acqFrameRateEnable->SetValue (!value.isAuto);
		if (!value.isAuto)
			_p_cam->AcquisitionFrameRate.SetValue (double (value.value));
		break;
	}
	case Property::EType_ExposureTime:
		if (value.isAuto)
			_p_cam->ExposureAuto.SetValue (ExposureAuto_Continuous);
		else {
			_p_cam->ExposureAuto.SetValue (ExposureAuto_Off);
			_p_cam->ExposureTime.SetValue (double (value.value));
		}
		break;
	case Property::EType_Gain:
		if (value.isAuto)
			_p_cam->GainAuto.SetValue (GainAuto_Continuous);
		else {
			_p_cam->GainAuto.SetValue (GainAuto_Off);
			_p_cam->Gain.SetValue (double (value.value));
		}
		break;
	}
	*/

}



PixelFormat CvCamera::_CVFORMAT_TO_EFORMAT (int rawFormat) const {

	switch (rawFormat) {
	case CV_8UC1:
		return PixelFormat::EFormat_Mono8;
	case CV_8UC3:
		return PixelFormat::EFormat_BGR;
	case CV_8UC4:
		return PixelFormat::EFormat_BGR32;
	default:
		return PixelFormat::EFormat_Unknown;
	}

}

NS_DLIB_CAM_END