#include "stdafx.h"
#include "SpinnakerCamera.h"
#include <sstream>
#include "../../Dlib_Basic/Basic.h"

using namespace Spinnaker;
using namespace Spinnaker::GenApi;
using namespace Spinnaker::GenICam;
using namespace Dlib::Basic;
using namespace Dlib::CvExt;
using namespace Dlib::Cam::Exception;



NS_DLIB_CAM_BEGIN

const static Spinnaker::PixelFormatEnums _FormatTable[5][5] = {
	{ PixelFormat_Mono8, PixelFormat_BGR8, PixelFormat_RGB8, PixelFormat_BGRa8, PixelFormat_RGBa8 },	// EFormat_Mono8
	{ PixelFormat_BGR8, PixelFormat_RGB8, PixelFormat_BGRa8, PixelFormat_RGBa8, PixelFormat_Mono8 },	// EFormat_BGR
	{ PixelFormat_BGRa8, PixelFormat_RGBa8, PixelFormat_BGR8, PixelFormat_RGB8, PixelFormat_Mono8 },	// EFormat_BGR32
	{ PixelFormat_RGB8, PixelFormat_BGR8, PixelFormat_RGBa8, PixelFormat_BGRa8, PixelFormat_Mono8 },	// EFormat_RGB
	{ PixelFormat_RGBa8, PixelFormat_BGRa8, PixelFormat_RGB8, PixelFormat_BGR8, PixelFormat_Mono8 },	// EFormat_RGB32
};



static std::vector<int64_t> _SupportFormats (Spinnaker::CameraPtr p_cam) {

	NodeList_t eformatNodes;
	p_cam->PixelFormat.GetEntries (eformatNodes);
	NodeList_t eformatReadableNodes;
	std::vector<int64_t> supportFormats;
	for (auto i = 0; i < eformatNodes.size (); ++i) {
		if (eformatNodes[i]->GetAccessMode () == RO)
			supportFormats.push_back (((CEnumEntryPtr)eformatNodes[i])
				->GetValue ());
	}
	return supportFormats;

}



static PixelFormat _SpFormat2EFormat (Spinnaker::PixelFormatEnums rawFormat) {

	switch (rawFormat) {
	case PixelFormat_Mono8:
		return PixelFormat::EFormat_Mono8;
	case PixelFormat_BGR8:
		return PixelFormat::EFormat_BGR;
	case PixelFormat_BGRa8:
		return PixelFormat::EFormat_BGR32;
	case PixelFormat_RGB8:
		return PixelFormat::EFormat_RGB;
	case PixelFormat_RGBa8:
		return PixelFormat::EFormat_RGB32;
	default:
		return PixelFormat::EFormat_Unknown;
	}

}



SpinnakerCamera::SpinnakerCamera (const Identifier& R_IDENTIFIER,
	SpinnakerSystem* p_system)
throw (VideoCameraException) :
	_identifier (R_IDENTIFIER),
	_p_system (p_system),
	_format (PixelFormat::EFormat_Unknown) {

	init ();

}



SpinnakerCamera::~SpinnakerCamera () {

	if (_p_cam == NULL)
		return;
	if (_p_cam->IsStreaming ())
		stop ();
	if (_p_cam->IsInitialized ())
		_p_cam->DeInit ();
	_p_cam = NULL;

}



void SpinnakerCamera::init ()
throw (VideoCameraException)
try {

	if (STATUS () != EStatus_None)
		return;

	_p_cam = NULL;
	_timestampAdjust = 0;
	auto cameraList = _p_system->getDeviceList ();
	if (_identifier.ID >= (int)cameraList.GetSize ())
		throw VideoCameraException (_identifier, "init",
			"No specified Spinnaker camera");

	_p_cam = cameraList.GetByIndex (_identifier.ID);
	_p_cam->Init ();
	_p_cam->TLStream.StreamBufferHandlingMode.SetValue (
		StreamBufferHandlingMode_NewestOnly);
	_timestampAdjust = Dlib::Basic::GetTimestamp () -
		_p_cam->Timestamp.GetValue () / 1000000;
	_format = _SpFormat2EFormat (_p_cam->PixelFormat.GetValue ());

} catch (Spinnaker::Exception& e) {
	_p_cam = NULL;
	throw VideoCameraException (_identifier, "init", e.what ());
}



void SpinnakerCamera::start ()
throw (VideoCameraException)
try {

	if (STATUS () == EStatus_None)
		init ();
	_p_cam->AcquisitionMode.SetIntValue (AcquisitionMode_Continuous);
	_p_cam->BeginAcquisition ();

} catch (Spinnaker::Exception& e) {
	throw VideoCameraException (_identifier, "start", e.what ());
}



bool SpinnakerCamera::grabFrame () {

	return false;

}



bool SpinnakerCamera::retrieveFrame (CvExt::Image& o_image,
	unsigned long long& o_timestamp)
throw (VideoCameraException)
try {

	// ����ǰ�豸��������������״̬�����Ը����䵱ǰ״ִ̬����Ӧ��ǰ�ò���
	if (STATUS () != EStatus_Streaming)
		start ();

	// ���Բ����µĻ���֡
	auto p_rawImage = _p_cam->GetNextImage ();
	if (p_rawImage->IsIncomplete ()) {
		p_rawImage->Release ();
		return false;
	}
	// ��ͼ�����ݰ�װΪOpenCV��ʽ
	cv::Mat cvRawImage ((int)p_rawImage->GetHeight (), (int)p_rawImage->GetWidth (),
		CV_MAKETYPE (CV_8U, (int)p_rawImage->GetBitsPerPixel () / 8), p_rawImage->GetData ());
	PixelFormat rawFormat = _SpFormat2EFormat (_p_cam->PixelFormat.GetValue ());

	// ��δָ������ߴ磬��Ĭ��Ϊ��ͬ�������
	if (_outputFrameSize.empty ())
		_outputFrameSize = cvRawImage.size ();

	// �����ͼ��ߴ���ʽ������Ҫ��������ͼ��ռ�
	if (o_image.SIZE () != _outputFrameSize || o_image.FORMAT () != _format)
		o_image = CvExt::Image (_format, _outputFrameSize);
	
	// �������ʽ��ԭʼ��ʽ��ͬ�������������ţ����ƣ�
	if (_format == rawFormat) {
		if (_outputFrameSize == cvRawImage.size ())
			cvRawImage.copyTo (o_image);
		else
			cv::resize (cvRawImage, o_image, _outputFrameSize);
	}
	// �����ͼ����ԭʼͼ���С��ͬ�����ִ�и�ʽת��
	else if (_outputFrameSize.area () == cvRawImage.size ().area ())
		PixelFormat::CvtColor (cvRawImage, o_image, rawFormat, _format);
	// �����ͼ���ԭʼͼ��С������ִ��������ִ�и�ʽת��
	else if (_outputFrameSize.area () < cvRawImage.size ().area ()) {
		if (_tempImage.size () != _outputFrameSize ||
			_tempImage.type () != rawFormat.CV_TYPE ())
			_tempImage = cv::Mat (_outputFrameSize, rawFormat.CV_TYPE ());
		cv::resize (cvRawImage, _tempImage, _outputFrameSize);
		PixelFormat::CvtColor (_tempImage, o_image, rawFormat, _format);
	}
	// �����ͼ���ԭʼͼ�������ִ�и�ʽת����ִ������
	else {
		if (_tempImage.size () != cvRawImage.size () ||
			_tempImage.type () != _format.CV_TYPE ())
			_tempImage = cv::Mat (cvRawImage.size (), _format.CV_TYPE ());
		PixelFormat::CvtColor (cvRawImage, _tempImage, rawFormat, _format);
		cv::resize (_tempImage, o_image, _outputFrameSize);
	}

	// ����ʱ���
	o_timestamp = p_rawImage->GetTimeStamp () / 1000000 + _timestampAdjust;

	// �ͷ�ԭʼ֡���ݿռ�
	p_rawImage->Release ();
	return true;

} catch (Spinnaker::Exception& e) {
	// ��ʱ�����ǳ�����󣬼ȵ�ǰû�л����֡���ݡ���˽�����false���������׳��쳣
	if (e.GetError () == SPINNAKER_ERR_TIMEOUT)
		return false;
	throw VideoCameraException (_identifier, "retrieveFrame", e.what ());
}



void SpinnakerCamera::stop () {

	if (STATUS () != EStatus_Streaming)
		return;
	try {
		_p_cam->EndAcquisition ();
	} catch (...) {}

}



std::string SpinnakerCamera::DEVICE_INFO () const {
	
	std::ostringstream sout;
	sout << "*** Spinnaker Device Information ***\n\n";
	if (STATUS () != EStatus_None) {
		CCategoryPtr category = _p_cam->GetTLDeviceNodeMap ()
			.GetNode ("DeviceInformation");
		if (IsAvailable (category) && IsReadable (category)) {
			FeatureList_t features;
			category->GetFeatures (features);
			for (auto it = features.begin (); it != features.end (); ++it) {
				CNodePtr pfeatureNode = *it;
				sout << pfeatureNode->GetName () << " : ";
				CValuePtr pValue = (CValuePtr)pfeatureNode;
				sout << (IsReadable (pValue) ?
					pValue->ToString () :
					"Node not readable");
				sout << std::endl;
			}
		} else {
			sout << "Device control information is not available.\n";
		}
	} else
		sout << "Device is not avaliable.\n";
	sout << "\n************************************\n";
	return sout.str ();

}



EStatus SpinnakerCamera::STATUS () const {

	if (_p_cam == NULL || !_p_cam->IsValid () || !_p_cam->IsInitialized ())
		return EStatus_None;
	if (_p_cam->IsStreaming ())
		return EStatus_Streaming;
	return EStatus_Initialized;

}



cv::Size SpinnakerCamera::FRAME_SIZE () const {

	if (!_outputFrameSize.empty ())
		return _outputFrameSize;
	if (_p_cam == NULL || !_p_cam->IsInitialized ())
		return { 0, 0 };
	return {
		int (_p_cam->Width.GetValue ()),
		int (_p_cam->Height.GetValue ())
	};

}



void SpinnakerCamera::setFrameSize (cv::Size frameSize) {

	_outputFrameSize = frameSize;

}



PixelFormat SpinnakerCamera::FORMAT () const {

	return _format;

}



bool SpinnakerCamera::setFormat (PixelFormat format) {

	if (format == _format)
		return true;

	// ����_FormatTable����ָ���ĺ�ѡ��ʽ����֧�ֵĸ�ʽ��ѡ������ʸ�ʽ
	auto supportFormats = _SupportFormats (_p_cam);
	for (int i = 0; i < 5; ++i) {
		for (int j = 0; j < supportFormats.size (); ++j) {
			if (_FormatTable[format][i] == supportFormats[j]) {
				_format = format;
				_p_cam->PixelFormat.SetIntValue (supportFormats[i]);
				return true;
			}
		}
	}

	return false;

}



Property::Range SpinnakerCamera::PROPERTY_RANGE (Property::EType type) const try {

	switch (type) {
	case Property::EType_FrameRate:
		return {
			(float) _p_cam->AcquisitionFrameRate.GetMin (),
			(float) _p_cam->AcquisitionFrameRate.GetMax ()
		};
	case Property::EType_ExposureTime:
		return {
			(float)_p_cam->ExposureTime.GetMin (),
			(float)_p_cam->ExposureTime.GetMax ()
		};
	case Property::EType_Gain:
		return {
			(float)_p_cam->Gain.GetMin (),
			(float)_p_cam->Gain.GetMax ()
		};
	default:
		return {};
	}

} catch (Spinnaker::Exception&) {
	return {};
}



Property SpinnakerCamera::PROPERTY_VALUE (Property::EType type) const try {

	switch (type) {
	case Property::EType_FrameRate: {
		CBooleanPtr p_acqFrameRateEnable = _p_cam->GetNodeMap ().GetNode (
			"AcquisitionFrameRateEnable");
		return {
			!p_acqFrameRateEnable->GetValue (),
			float (_p_cam->AcquisitionFrameRate.GetValue ())
		};
	}
	case Property::EType_ExposureTime:
		return {
			_p_cam->ExposureAuto.GetValue () == ExposureAuto_Continuous,
			float (_p_cam->ExposureTime.GetValue (true, true))
		};
	case Property::EType_Gain:
		return {
			_p_cam->GainAuto.GetValue () == GainAuto_Continuous,
			float (_p_cam->Gain.GetValue (true, true))
		};
	default:
		return {};
	}

} catch (Spinnaker::Exception&) {
	return {};
}



bool SpinnakerCamera::setPropertyValue (Property::EType type, Property value) try {

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
	default:
		return false;
	}
	return true;

} catch (Spinnaker::Exception&) {
	return false;
}

NS_DLIB_CAM_END