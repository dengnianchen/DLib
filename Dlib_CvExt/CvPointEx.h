#pragma once
#include "global.h"



NS_DLIB_CVEXT_BEGIN

template<class T>
float magnitude (cv::Point_<T> p) {
	return std::sqrt (p.dot (p));
}

template<class T>
float magnitude (cv::Point3_<T> p) {
	return std::sqrt (p.dot (p));
}

template<class T>
cv::Point2f normalize (cv::Point_<T> p) {
	return p / magnitude (p);
}

template<class T>
cv::Point2f normalize (cv::Point3_<T> p) {
	return p / magnitude (p);
}

NS_DLIB_CVEXT_END