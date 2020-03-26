#pragma once
#include "global.h"
#include <string>



NS_DLIB_CAM_BEGIN

class DLIBCAM_INTERFACE Identifier {
public:
	const std::string DEVICE_TYPE;
	const int ID;

	Identifier (const std::string& deviceType, int id);
	operator const std::string& () const;

private:
	std::string _str;

};

NS_DLIB_CAM_END