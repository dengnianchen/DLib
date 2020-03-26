#pragma once



NS_DLIB_UNITY_BEGIN

class DLIB_UNITY_INTERFACE Str {
public:
	static char* ToReturnStr (const std::string& R_STR);
	static wchar_t* ToReturnStr (const std::wstring& R_STR);

};

NS_DLIB_UNITY_END