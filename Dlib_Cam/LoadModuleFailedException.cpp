#include "LoadModuleFailedException.h"
#include "../Dlib_Basic/Basic.h"

using namespace Dlib::Basic;



NS_DLIB_CAM_EX_BEGIN

LoadModuleFailedException::LoadModuleFailedException (
	const std::wstring& R_MODULE_NAME_W,
	const std::vector<std::wstring>& R_SEARCH_DIRECTORIES) :
Exception::Exception (L"Failed to load module " + R_MODULE_NAME_W + L"in:\n    " +
	StringUtils::Join (R_SEARCH_DIRECTORIES, L"\n    ")) { }

NS_DLIB_CAM_EX_END