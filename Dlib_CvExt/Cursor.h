#pragma once
#include "global.h"



NS_DLIB_CVEXT_BEGIN

enum MoveDirection { // 从0点钟方向开始顺时针8个方向
	MoveUp, MoveUpRight, MoveRight, MoveDownRight,
	MoveDown, MoveDownLeft, MoveLeft, MoveUpLeft
};



template<class T>
class Cursor {
public:
	Cursor (const cv::Mat& R_MAT, cv::Point initPos, bool transpose = false) :
	_P_MAT (&R_MAT) {
		_rowSteps = (int)((transpose ? R_MAT.step[1] : R_MAT.step[0]) / sizeof (T));
		_colSteps = (int)((transpose ? R_MAT.step[0] : R_MAT.step[1]) / sizeof (T));
		_matSize = transpose ? cv::Size (R_MAT.rows, R_MAT.cols) : R_MAT.size ();
		reset (initPos);
	}
	
	Cursor<T>& operator+= (cv::Point offset) {
		_ptr += _rowSteps * offset.y + _colSteps * offset.x;
		_position += offset;
		return *this;
	}

	Cursor<T>& operator+= (MoveDirection d) {
		switch (d) {
		case MoveUp: _ptr -= _rowSteps; _position.y--; break;
		case MoveUpRight: _ptr += -_rowSteps + _colSteps; _position.x--; _position.y--; break;
		case MoveRight: _ptr += _colSteps; _position.x++; break;
		case MoveDownRight: _ptr += _rowSteps + _colSteps; _position.x++; _position.y++; break;
		case MoveDown: _ptr += _rowSteps; _position.y++; break;
		case MoveDownLeft: _ptr += _rowSteps - _colSteps; _position.x--; _position.y++; break;
		case MoveLeft: _ptr -= _colSteps; _position.x--; break;
		case MoveUpLeft: _ptr -= _rowSteps + _colSteps; _position.x--; _position.y--; break;
		}
		return *this;
	}

	Cursor<T>& operator-= (cv::Point offset) {
		return (*this) += -offset;
	}

	Cursor<T>& operator-= (MoveDirection d) {
		switch (d) {
		case MoveUp: return (*this) += MoveDown;
		case MoveUpRight: return (*this) += MoveDownLeft;
		case MoveRight: return (*this) += MoveLeft;
		case MoveDownRight: return (*this) += MoveUpLeft;
		case MoveDown: return (*this) += MoveUp;
		case MoveDownLeft:(*this) += MoveUpRight;
		case MoveLeft: return (*this) += MoveRight;
		case MoveUpLeft: return (*this) += MoveDownRight;
		default: return *this;
		}
	}

	Cursor<T> operator+ (cv::Point offset) const {
		return Cursor<T> (*this) += offset;
	}

	Cursor<T> operator+ (MoveDirection d) const {
		return Cursor<T> (*this) += d;
	}

	Cursor<T> operator- (cv::Point offset) const {
		return Cursor<T> (*this) -= offset;
	}

	Cursor<T> operator- (MoveDirection d) const {
		return Cursor<T> (*this) -= d;
	}

	T& operator* () const {
		CV_Assert (_ptr >= (T*)_P_MAT->data && _ptr < (T*)_P_MAT->datalimit);
		return *_ptr;
	}

	void reset (int x, int y) {
		_ptr = (T*)_P_MAT->data + _rowSteps * y + _colSteps * x;
		_position = { x, y };
	}

	void reset (cv::Point p = { }) {
		_ptr = (T*)_P_MAT->data + _rowSteps * p.y + _colSteps * p.x;
		_position = p;
	}

	void reset (int offset) {
		if (_colSteps < _rowSteps) {
			if (offset < 0)
				_position = { _matSize.width + offset % _rowSteps, offset / _rowSteps - 1 };
			else
				_position = { offset % _rowSteps, offset / _rowSteps };
			_position.x /= _colSteps;
		} else {
			if (offset < 0)
				_position = { offset / _colSteps - 1, _matSize.width + offset % _colSteps };
			else
				_position = { offset / _colSteps, offset % _colSteps };
			_position.y /= _rowSteps;
		}
		_ptr = (T*)_P_MAT->data + offset;
	}

	int OFFSET () const { return _ptr - (T*)_P_MAT->data; }

	template<class T2>
	Cursor<T2> cursor (const cv::Mat& R_MAT1) const {
		CV_Assert (_P_MAT->size () == R_MAT1.size ());
		return Cursor<T2> (R_MAT1, POSITION (), _matSize.width != _P_MAT->cols);
	}
	template<class T2>
	Cursor<T2> cursor (const cv::Mat_<T2>& R_MAT1) const {
		return cursor<T2> ((const cv::Mat&)R_MAT1);
	}

	T& valueRef () const { return **this; }

	template<class T2>
	T2& valueRef (const cv::Mat& R_MAT1) const {
		CV_Assert (_P_MAT->size () == R_MAT1.size ());
		auto ptr = (T2*)R_MAT1.data + (_ptr - (T*)_P_MAT->data);
		CV_Assert (ptr >= (T2*)R_MAT1.data && ptr < (T2*)R_MAT1.datalimit);
		return *ptr;
	}
	
	template<class T2>
	T2& valueRef (const cv::Mat_<T2>& R_MAT1) const {
		return valueRef<T2> ((const cv::Mat&)R_MAT1);
	}

	T value (bool checkBoundary = false, T defaultValue = T ()) const {
		if (checkBoundary && !IS_VALID ())
			return defaultValue;
		return *_ptr;
	}

	template<class T2>
	T2 value (const cv::Mat& R_MAT1, bool checkBoundary = false,
		T2 defaultValue = T2 ()) const {
		CV_Assert (_P_MAT->size () == R_MAT1.size ());
		if (checkBoundary && !IS_VALID ())
			return defaultValue;
		return valueRef<T2> (R_MAT1);
	}

	template<class T2>
	T2 value (const cv::Mat_<T2>& R_MAT1, bool checkBoundary = false,
		T2 defaultValue = T2 ()) const {
		CV_Assert (_P_MAT->size () == R_MAT1.size ());
		if (checkBoundary && !IS_VALID ())
			return defaultValue;
		return valueRef<T2> ((const cv::Mat&)R_MAT1);
	}

	cv::Point POSITION () const { return _position;	}
	int X () const { return _position.x; }
	int Y () const { return _position.y; }

	cv::Size MAT_SIZE () const { return _matSize; }
	int WIDTH () const { return _matSize.width; }
	int HEIGHT () const { return _matSize.height; }

	const cv::Mat& MATRIX () const { return *_P_MAT; }

	bool IS_VALID () const {
		return _position.x >= 0 && _position.y >= 0 &&
			_position.x < _matSize.width &&
			_position.y < _matSize.height;
	}

private:
	const cv::Mat* _P_MAT;
	T* _ptr;
	int _rowSteps;
	int _colSteps;
	cv::Point _position;
	cv::Size _matSize;

};

NS_DLIB_CVEXT_END