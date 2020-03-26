#include "Exception.h"
#include "StringUtils.h"



NS_DLIB_BASIC_EX_BEGIN

Exception::Exception (const std::wstring& R_MESSAGE_W) :
_messageW (R_MESSAGE_W) {
}



const std::wstring& Exception::MESSAGE_W () const {
	
	return _messageW;

}

NS_DLIB_BASIC_EX_END