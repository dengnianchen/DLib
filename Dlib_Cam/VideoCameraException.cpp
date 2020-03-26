#include "VideoCameraException.h"
#include "../Dlib_Basic/Basic.h"

using namespace Dlib::Basic;



NS_DLIB_CAM_EX_BEGIN

VideoCameraException::VideoCameraException (const std::string& identifier,
	const std::string& function, const std::string& message) :
	Exception::Exception (StringUtils::ToWString ("Camera " + identifier + " failed in " + function + ": " + message)) {}

NS_DLIB_CAM_EX_END