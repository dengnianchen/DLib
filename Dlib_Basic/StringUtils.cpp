#include "StringUtils.h"
#include <Windows.h>



NS_DLIB_BASIC_BEGIN

std::wstring StringUtils::ToWString (const std::string& R_STRING) {

	LPCSTR pszSrc = R_STRING.c_str ();
	int nLen = MultiByteToWideChar (CP_ACP, 0, pszSrc, -1, NULL, 0);
	if (nLen == 0)
		return std::wstring (L"");

	wchar_t* pwszDst = new wchar_t[nLen];
	if (!pwszDst)
		return std::wstring (L"");

	MultiByteToWideChar (CP_ACP, 0, pszSrc, -1, pwszDst, nLen);
	std::wstring wstr (pwszDst);
	delete[] pwszDst;
	pwszDst = NULL;
	return wstr;

}



std::string StringUtils::ToString (const std::wstring& R_STRING_W) {

	LPCWSTR pwszSrc = R_STRING_W.c_str ();
	int nLen = WideCharToMultiByte (CP_ACP, 0, pwszSrc, -1, NULL, 0, NULL, NULL);
	if (nLen == 0)
		return std::string ("");

	char* pszDst = new char[nLen];
	if (!pszDst)
		return std::string ("");

	WideCharToMultiByte (CP_ACP, 0, pwszSrc, -1, pszDst, nLen, NULL, NULL);
	std::string str (pszDst);
	delete[] pszDst;
	pszDst = NULL;

	return str;

}



std::string StringUtils::Join (const std::vector<std::string>& R_STRING_LIST,
	const std::string& R_DELIMITER) {
	
	std::string ret = R_STRING_LIST[0];
	for (int i = 1; i < R_STRING_LIST.size (); ++i)
		ret += R_DELIMITER + R_STRING_LIST[i];
	return ret;


}



std::wstring StringUtils::Join (const std::vector<std::wstring>& R_STRING_W_LIST,
	const std::wstring& R_DELIMITER_W) {
	
	std::wstring ret = R_STRING_W_LIST[0];
	for (int i = 1; i < R_STRING_W_LIST.size (); ++i)
		ret += R_DELIMITER_W + R_STRING_W_LIST[i];
	return ret;

}

NS_DLIB_BASIC_END