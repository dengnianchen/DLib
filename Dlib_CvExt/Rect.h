#pragma once
#include "global.h"



NS_DLIB_CVEXT_BEGIN

enum Povit {
	Povit_Center,
	Povit_TopLeft, Povit_TopRight,
	Povit_BottomLeft, Povit_BottomRight
};



template< class T >
cv::Rect_<T> operator* (const cv::Rect_<T>& a, const cv::Size_<T>& b) {
	return cv::Rect_<T> (a.x, a.y, a.width * b.width, a.height * b.height);
}



template< class T >
cv::Rect_<T> operator* (const cv::Rect_<T>& a, T b) {
	return cv::Rect_<T> (a.x, a.y, a.width * b, a.height * b);
}



template< class T >
cv::Rect_<T>& operator*= (cv::Rect_<T>& a, const cv::Size_<T>& b) {
	a.width *= b.width;
	a.height *= b.height;
	return a;
}



template< class T >
cv::Rect_<T>& operator*= (cv::Rect_<T>& a, T b) {
	a.width *= b;
	a.height *= b;
	return a;
}



template< class T >
cv::Rect_<T> operator/ (const cv::Rect_<T>& a, const cv::Size_<T>& b) {
	return cv::Rect_<T> (a.x, a.y, a.width / b.width, a.height / b.height);
}



template< class T >
cv::Rect_<T> operator/ (const cv::Rect_<T>& a, T b) {
	return cv::Rect_<T> (a.x, a.y, a.width / b, a.height / b);
}



template< class T >
cv::Rect_<T>& operator/= (cv::Rect_<T>& a, const cv::Size_<T>& b) {
	a.width /= b.width;
	a.height /= b.height;
	return a;
}



template< class T >
cv::Rect_<T>& operator/= (cv::Rect_<T>& a, T b) {
	a.width /= b;
	a.height /= b;
	return a;
}



template< class T >
class Rect {
public:
	Rect (cv::Rect_<T>& r_data) : _p_data (&r_data) {}
	Rect (const cv::Rect_<T>& r_data) : _private_data (r_data), _p_data (&_private_data) {}
	Rect (const Rect<T>& a) : _private_data (*a._p_data), _p_data (&_private_data), _povitLocal (a._povitLocal) {}

	Rect& operator= (const Rect<T>& a) {
		_private_data = *a._p_data;
		_p_data = &_private_data;
		_povitLocal = a._povitLocal;
		return *this;
	}

	operator cv::Point_<T> () const {
		return cv::Point_<T> (
			_p_data->x + T (_p_data->width * _povitLocal[0]),
			_p_data->y + T (_p_data->height * _povitLocal[1])
		);
	}
	
	operator cv::Rect_<T>& () {
		return *_p_data;
	}

	operator const cv::Rect_<T>& () const {
		return *_p_data;
	}

	cv::Rect_<T>* operator-> () {
		return _p_data;
	}

	const cv::Rect_<T>* operator-> () const {
		return _p_data;
	}

	Rect<T>& operator+= (const cv::Point_<T>& b) {
		_p_data->x += b.x;
		_p_data->y += b.y;
		return *this;
	}

	Rect<T>& operator+= (const cv::Size_<T>& b) {
		_p_data->x -= T (b.width * _povitLocal[0]);
		_p_data->y -= T (b.height * _povitLocal[1]);
		_p_data->width += b.width;
		_p_data->height += b.height;
		return *this;
	}

	Rect<T>& operator-= (const cv::Point_<T>& b) {
		return *this += -b;
	}

	Rect<T>& operator-= (const cv::Size_<T>& b) {
		return *this += cv::Size_<T> (-b.width, -b.height);
	}

	Rect<T>& operator*= (const cv::Size_<T>& b) {
		cv::Point_<T> povit = *this;
		_p_data->width *= b.width;
		_p_data->height *= b.height;
		_p_data->x = povit.x - T (_p_data->width * _povitLocal[0]);
		_p_data->y = povit.y - T (_p_data->height * _povitLocal[1]);
		return *this;
	}

	Rect<T>& operator*= (T b) {
		return *this *= cv::Size_<T> (b, b);
	}

	Rect<T>& operator/= (const cv::Size_<T>& b) {
		cv::Point_<T> povit = *this;
		_p_data->width /= b.width;
		_p_data->height /= b.height;
		_p_data->x = povit.x - T (_p_data->width * _povitLocal[0]);
		_p_data->y = povit.y - T (_p_data->height * _povitLocal[1]);
		return *this;
	}

	Rect<T>& operator/= (T b) {
		return *this /= cv::Size_<T> (b, b);
	}

	Rect<T>& operator&= (const Rect<T>& b) {
		*_p_data &= *b._p_data;
		return *this;
	}

	Rect<T>& operator|= (const Rect<T>& b) {
		*_p_data |= *b._p_data;
		return *this;
	}

	bool operator== (const Rect<T>& b) const {
		return *_p_data == *b._p_data;
	}

	bool operator!= (const Rect<T>& b) const {
		return *_p_data != *b._p_data;
	}

	Rect<T> operator+ (const cv::Point_<T>& b) const {
		auto r = *this;
		return r += b;
	}

	Rect<T> operator+ (const cv::Size_<T>& b) const {
		auto r = *this;
		return r += b;
	}

	Rect<T> operator- (const cv::Point_<T>& b) const {
		auto r = *this;
		return r -= b;
	}

	Rect<T> operator- (const cv::Size_<T>& b) const {
		auto r = *this;
		return r -= b;
	}

	Rect<T> operator* (const cv::Size_<T>& b) const {
		auto r = *this;
		return r *= b;
	}

	Rect<T> operator* (T b) const {
		auto r = *this;
		return r *= b;
	}

	Rect<T> operator/ (const cv::Size_<T>& b) const {
		auto r = *this;
		return r /= b;
	}

	Rect<T> operator/ (T b) const {
		auto r = *this;
		return r /= b;
	}

	Rect<T> operator& (const Rect<T>& b) const {
		auto r = *this;
		return r &= b;
	}

	Rect<T> operator| (const Rect<T>& b) const {
		auto r = *this;
		return r |= b;
	}

private:
	template< class T1 >
	friend Rect<T1> operator>> (cv::Rect_<T1>& a, Povit povit);
	template< class T1 >
	friend const Rect<T1> operator>> (const cv::Rect_<T1>& a, Povit povit);

	bool _is_ref;
	cv::Rect_<T>* _p_data;
	cv::Rect_<T> _private_data;
	cv::Vec2f _povitLocal;

	Rect (cv::Rect_<T>& r_data, Povit povit) : _p_data (&r_data), _povitLocal (_PovitLocal (povit)) {}
	Rect (cv::Rect_<T>& r_data, cv::Vec2f povitLocal) : _p_data (&r_data), _povitLocal (povitLocal) {}
	Rect (const cv::Rect_<T>& r_data, Povit povit) : _private_data (r_data), _p_data (&_private_data), _povitLocal (_PovitLocal (povit)) {}
	Rect (const cv::Rect_<T>& r_data, cv::Vec2f povitLocal) : _private_data (r_data), _p_data (&_private_data), _povitLocal (povitLocal) {}

	static cv::Vec2f _PovitLocal (Povit povit) {
		switch (povit) {
		case Povit_Center:		return { 0.5f, 0.5f };
		case Povit_TopLeft:		return { 0.0f, 0.0f };
		case Povit_TopRight:	return { 1.0f, 0.0f };
		case Povit_BottomLeft:	return { 0.0f, 1.0f };
		case Povit_BottomRight:	return { 1.0f, 1.0f };
		default: return {};
		}
	}

};



template< class T >
Rect<T> operator>> (cv::Rect_<T>& a, Povit povit) {
	return Rect<T> (a, povit);
}



template< class T >
const Rect<T> operator>> (const cv::Rect_<T>& a, Povit povit) {
	return Rect<T> (a, povit);
}

NS_DLIB_CVEXT_END