#pragma once
#include "global.h"
#include <vector>
#include "../Dlib_Basic/Exception.h"



NS_DLIB_CAM_EX_BEGIN

class DLIBCAM_INTERFACE LoadModuleFailedException : public Basic::Exception::Exception {
public:

	LoadModuleFailedException (const std::wstring& R_MODULE_NAME_W,
		const std::vector<std::wstring>& R_SEARCH_DIRECTORIES);

};

NS_DLIB_CAM_EX_END