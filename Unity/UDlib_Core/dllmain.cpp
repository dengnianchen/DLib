#include "../../Dlib_Unity/All.h"

using namespace Dlib::Unity;



extern "C" {

UDLIB_INTERFACE (void Framework_RegisterExHandler (Framework::ExceptionHandle h,
	Framework::ExceptionHandleW hw)) {

	Framework::RegisterExHandler (h, hw);

} END_UDLIB_INTERFACE

}