#pragma once
#include "global.h"
#include <string>
#include <vector>



NS_DLIB_BASIC_BEGIN

class StringUtils {
public:
	static std::wstring ToWString (const std::string& R_STRING);
	static std::string ToString (const std::wstring& R_STRING_W);
	static std::string Join (const std::vector<std::string>& R_STRING_LIST, const std::string& R_DELIMITER);
	static std::wstring Join (const std::vector<std::wstring>& R_STRING_W_LIST, const std::wstring& R_DELIMITER_W);

};

NS_DLIB_BASIC_END
