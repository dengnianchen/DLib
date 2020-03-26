#include "Identifier.h"
#include <sstream>



NS_DLIB_CAM_BEGIN

Identifier::Identifier (const std::string& deviceType, int id) :
	DEVICE_TYPE (deviceType),
	ID (id) {

	std::ostringstream sout;
	sout << deviceType << id;
	_str = sout.str ();

}



Identifier::operator const std::string& () const {

	return _str;

}

NS_DLIB_CAM_END