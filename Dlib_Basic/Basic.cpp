#include "Basic.h"
#include <chrono>



NS_DLIB_BASIC_BEGIN

unsigned long long GetTimestamp () {

	return std::chrono::duration_cast<std::chrono::milliseconds> (
		std::chrono::system_clock::now ().time_since_epoch ()).count ();

}

NS_DLIB_BASIC_END
