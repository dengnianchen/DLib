#pragma once
#include "global.h"
#include <cmath>



NS_DLIB_BASIC_BEGIN

class Math {
public:

	template< class T >
	static bool approximately (T val1, T val2) {
		return std::abs (val1 - val2) < 10e-5;
	}

};

NS_DLIB_BASIC_END