#pragma once
#include "global.h"
#include <string>



NS_DLIB_BASIC_EX_BEGIN

class Exception {
public:
	Exception (const std::wstring& R_MESSAGE_W);
	virtual ~Exception () {}

	const std::wstring& MESSAGE_W () const;

private:
	std::wstring _messageW;
	
};



class InvalidArgument : public Exception {
public:
	InvalidArgument (const std::wstring& R_MESSAGE_W) : Exception (R_MESSAGE_W) {}

};



class FileNotFound : public Exception {
public:
	FileNotFound (const std::wstring& R_FILE_NAME) : Exception (L"File not found: " + R_FILE_NAME) {}

};

NS_DLIB_BASIC_EX_END