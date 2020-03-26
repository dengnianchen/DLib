#include "pch.h"
#include "Framework.h"


NS_DLIB_UNITY_BEGIN

Framework::ExceptionHandle Framework::_ExceptionHandler = NULL;
Framework::ExceptionHandleW Framework::_ExceptionHandlerW = NULL;



void Framework::RegisterExHandler (ExceptionHandle h, ExceptionHandleW hw) {

	_ExceptionHandler = h;
	_ExceptionHandlerW = hw;

}



void Framework::ThrowEx (const char* message) {

	if (_ExceptionHandler)
		_ExceptionHandler (message);
	else
		throw std::exception (message);

}



void Framework::ThrowEx (const std::exception& ex) {

	if (_ExceptionHandler)
		_ExceptionHandler (ex.what ());
	else
		throw ex;

}



void Framework::ThrowEx (const wchar_t* messageW) {

	if (_ExceptionHandlerW)
		_ExceptionHandlerW (messageW);
	else
		throw Basic::Exception::Exception (messageW);

}



void Framework::ThrowEx (const Dlib::Basic::Exception::Exception& ex) {

	if (_ExceptionHandlerW)
		_ExceptionHandlerW (ex.MESSAGE_W ().c_str ());
	else
		throw ex;

}

NS_DLIB_UNITY_END

