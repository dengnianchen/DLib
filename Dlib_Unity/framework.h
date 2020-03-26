#pragma once
#include "../Dlib_Basic/Basic.h"



NS_DLIB_UNITY_BEGIN

class DLIB_UNITY_INTERFACE Framework {
public:
	typedef void (*ExceptionHandle) (const char*);
	typedef void (*ExceptionHandleW) (const wchar_t*);

	static void RegisterExHandler (ExceptionHandle h, ExceptionHandleW hw);
	static void ThrowEx (const char* message);
	static void ThrowEx (const std::exception& ex);
	static void ThrowEx (const wchar_t* messageW);
	static void ThrowEx (const Basic::Exception::Exception& ex);

private:
	static ExceptionHandle _ExceptionHandler;
	static ExceptionHandleW _ExceptionHandlerW;

};

NS_DLIB_UNITY_END