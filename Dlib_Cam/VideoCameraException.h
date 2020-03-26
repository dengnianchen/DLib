#pragma once
#include "global.h"
#include "../Dlib_Basic/Exception.h"



NS_DLIB_CAM_EX_BEGIN

class DLIBCAM_INTERFACE VideoCameraException : public Basic::Exception::Exception {
public:

	VideoCameraException (const std::string& identifier,
		const std::string& function, const std::string& message);

};

NS_DLIB_CAM_EX_END