#pragma once
#include "global.h"
#include "Vec.h"



NS_DLIB_UNITY_BEGIN

template<class T>
class UMatrix {
private:
	Dlib::Basic::ArrayRef<T> _array;
	Vec2i _size;
	int _step;

public:
	UMatrix (cv::Mat m) :
		_array ((float*)m.data, (m.datalimit - m.data) / sizeof (T)),
		_size (m.size ()) {
		_step = (int)(m.step / sizeof (T));
	}
	
	operator cv::Mat_<T> () {
		return cv::Mat_<T> (_size.y, _size.x, _array.data (), _step * sizeof(T));
	}

};

NS_DLIB_UNITY_END