#include "pch.h"
#include "Str.h"
#include <combaseapi.h>



NS_DLIB_UNITY_BEGIN

char* Str::ToReturnStr (const std::string& R_STR) {

	char* cstrRet = (char*)CoTaskMemAlloc (R_STR.size () + 1);
	memcpy_s (cstrRet, R_STR.size () + 1, R_STR.c_str (), R_STR.size () + 1);
	return cstrRet;

}



wchar_t* Str::ToReturnStr (const std::wstring& R_STR) {

	auto size = sizeof (wchar_t) * (R_STR.size () + 1);
	wchar_t* cstrRet = (wchar_t*)CoTaskMemAlloc (size);
	memcpy_s (cstrRet, size, R_STR.c_str (), size);
	return cstrRet;

}

NS_DLIB_UNITY_END