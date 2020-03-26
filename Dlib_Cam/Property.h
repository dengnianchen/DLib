#pragma once
#include "global.h"



NS_DLIB_CAM_BEGIN

class Property {
public:
	enum EType { EType_FrameRate, EType_ExposureTime, EType_Gain };

	class Range {
	public:
		float low;
		float high;
		Range (float low = NAN, float high = NAN) : low (low), high (high) {}
	};

	bool isValid;
	bool isAuto;
	float value;

	Property () : isValid (false) {}
	Property (bool isAuto, float value) : isAuto (isAuto), value (value), isValid (true) {}

};

NS_DLIB_CAM_END