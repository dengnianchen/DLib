#pragma once
#include "global.h"



NS_DLIB_CVEXT_BEGIN

template<class T>
T getRandom (T min, T max) {
	return min == max ? min : (T)(std::rand () / (double)RAND_MAX * (max - min) + min);
}

NS_DLIB_CVEXT_END