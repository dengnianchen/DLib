#pragma once
#include "global.h"
#include "Cursor.h"



NS_DLIB_CVEXT_BEGIN

template<class T>
class BaseIterator {
public:
	BaseIterator (const Cursor<T>& R_BEGIN) : _cursor (R_BEGIN) { }

	virtual bool moveNext () = 0;
	virtual bool movePrev () = 0;
	virtual bool BEYOND_END () const = 0;
	virtual bool BEFORE_START () const = 0;

	virtual bool moveAhead (int steps) {
		for (auto i = 0; i < steps; ++i)
			if (!moveNext ())
				return false;
		return true;
	}
	virtual bool moveBack (int steps) {
		for (auto i = 0; i < steps; ++i)
			if (!movePrev ())
				return false;
		return true;
	}

	cv::Point POSITION () const { return _cursor.POSITION (); }

	T& operator* () { return *_cursor; }

	Cursor<T> cursor () const { return _cursor; }

	template<class T2>
	Cursor<T2> cursor (const cv::Mat& R_MAT1) const { return _cursor.cursor<T2> (R_MAT1); }

	template<class T2>
	Cursor<T2> cursor (const cv::Mat_<T2>& R_MAT1) const { return _cursor.cursor (R_MAT1); }

	T& valueRef () const { return _cursor.valueRef (); }

	template<class T2>
	T2& valueRef (const cv::Mat& R_MAT1) const { return _cursor.valueRef<T2> (R_MAT1); }

	template<class T2>
	T2& valueRef (const cv::Mat_<T2>& R_MAT1) const { return _cursor.valueRef (R_MAT1); }

	T value (bool checkBoundary = false, T defaultValue = T ()) const {
		return _cursor.value (checkBoundary, defaultValue);
	}

	template<class T2>
	T2 value (const cv::Mat& R_MAT1, bool checkBoundary = false, T2 defaultValue = T2 ()) const {
		return _cursor.value<T2> (R_MAT1, checkBoundary, defaultValue);
	}

	template<class T2>
	T2 value (const cv::Mat_<T2>& R_MAT1, bool checkBoundary = false, T2 defaultValue = T2 ()) const {
		return _cursor.value (R_MAT1, checkBoundary, defaultValue);
	}

	bool IS_VALID () const { return _cursor.IS_VALID (); }

protected:
	Cursor<T> _cursor;

};

template<class T>
class BasicIterator : public BaseIterator<T> {
public:
	BasicIterator (const Cursor<T>& R_BEGIN) :
		BaseIterator<T> (R_BEGIN + cv::Point (R_BEGIN.WIDTH () - 1, -1)) { }
	BasicIterator (const cv::Mat& R_MAT, bool transpose = false) :
		BaseIterator<T> ({ R_MAT, { R_MAT.cols - 1, -1 }, transpose }) { }

	bool moveNext () override {
		this->_cursor += MoveRight;
		if (this->_cursor.X () == this->_cursor.WIDTH ())
			this->_cursor += cv::Point (-this->_cursor.WIDTH (), 1);
		return this->_cursor.Y () < this->_cursor.HEIGHT ();
	}

	bool movePrev () override {
		this->_cursor += MoveLeft;
		if (this->_cursor.X () < 0)
			this->_cursor += cv::Point (this->_cursor.WIDTH (), -1);
		return this->_cursor.Y () >= 0;
	}

	bool BEYOND_END () const override {
		return this->_cursor.Y () >= this->_cursor.HEIGHT ();
	}
	
	bool BEFORE_START () const override {
		return this->_cursor.Y () < 0;
	}
	
	bool moveAhead (int steps) override {
		auto x = this->_cursor.X () + steps;
		cv::Point newPos (x % this->_cursor.WIDTH (),
			this->_cursor.POSITION ().y + x / this->_cursor.WIDTH ());
		if (newPos.y >= this->_cursor.HEIGHT ()) {
			this->_cursor.reset ({ 0, this->_cursor.HEIGHT () });
			return false;
		}
		this->_cursor.reset (newPos);
		return true;
	}

	virtual bool moveBack (int steps) {
		auto x = this->_cursor.X () - steps;
		if (x >= 0) {
			this->_cursor += cv::Point (-steps, 0);
			return true;
		}
		cv::Point newPos (this->_cursor.WIDTH () + x % this->_cursor.WIDTH (),
			this->_cursor.POSITION ().y + x / this->_cursor.WIDTH () - 1);
		if (newPos.y < 0) {
			this->_cursor.reset ({ this->_cursor.WIDTH () - 1, -1 });
			return false;
		}
		this->_cursor.reset (newPos);
		return true;
	}

};



template<class T>
class SquareIterator : public BaseIterator<T> {
public:
	SquareIterator (const Cursor<T>& R_CENTER, int radius, int radiusY = -1) :
		BaseIterator<T> (R_CENTER + cv::Point (-radius, (radiusY == -1 ? radius : radiusY) + 1)),
		_radiusX (radius),
		_radiusY (radiusY == -1 ? radius : radiusY),
		_d ((MoveDirection)0),
		_indexInSeg (-1),
		_loop (false),
		_centerCursor (R_CENTER) { }

	bool moveNext () override {
		this->_cursor += _d;
		_indexInSeg++;
		if (_indexInSeg >= _lengthAtDirection (_d)) {
			_d = (MoveDirection)(_loop && _d == 6 ? 0 : _d + 2);
			_indexInSeg = 0;
		}
		return _d < 8;
	}

	bool movePrev () override {
		if (_indexInSeg == 0 && (_d != 0 || _loop)) {
			_d = (MoveDirection)(_d == 0 ? 6 : _d - 2);
			_indexInSeg = _lengthAtDirection (_d);
		}
		_indexInSeg--;
		this->_cursor -= _d;
		return _indexInSeg >= 0;
	}

	bool moveAhead (int steps) override {
		if (steps <= 3)
			return BaseIterator<T>::moveAhead (steps);
		setOffset (OFFSET () + steps);
		return !BEYOND_END ();
	}
	
	bool moveBack (int steps) override {
		if (steps <= 3)
			return BaseIterator<T>::moveBack (steps);
		setOffset (OFFSET () - steps);
		return !BEFORE_START ();
	}
	
	bool BEYOND_END () const override { return _d >= 8; };

	bool BEFORE_START () const override { return _indexInSeg < 0; }

	void setLoop (bool loop) { _loop = loop; }

	int OFFSET () const {
		int baseOffset = 0;
		switch (_d) {
		case 8: baseOffset += _radiusX + _radiusX;
		case 6: baseOffset += _radiusY + _radiusY;
		case 4: baseOffset += _radiusX + _radiusX;
		case 2: baseOffset += _radiusY + _radiusY;
		}
		return baseOffset + _indexInSeg;
	}

	void setOffset (int offset) {
		auto length = (_radiusX + _radiusY) * 4;
		_d = (MoveDirection)0;
		if (offset >= length) {
			if (_loop) offset %= length;
			else {
				_d = (MoveDirection)8;
				offset = 0;
			}
		} else if (offset < 0) {
			if (_loop) offset = length + offset % length;
			else offset = -1;
		}
		while (offset >= _lengthAtDirection (_d)) {
			offset -= _lengthAtDirection (_d);
			_d = (MoveDirection)(_d + 2);
		}
		_indexInSeg = offset;
		switch (_d) {
		case 0: this->_cursor = _centerCursor + cv::Point (-_radiusX, _radiusY - _indexInSeg); break;
		case 2: this->_cursor = _centerCursor + cv::Point (-_radiusX + _indexInSeg, -_radiusY); break;
		case 4: this->_cursor = _centerCursor + cv::Point (_radiusX, -_radiusY + _indexInSeg); break;
		case 6: this->_cursor = _centerCursor + cv::Point (_radiusX - _indexInSeg, _radiusY); break;
		}
	}

private:
	int _radiusX;
	int _radiusY;
	MoveDirection _d;
	int _indexInSeg;
	bool _loop;
	Cursor<T> _centerCursor;

	int _lengthAtDirection (MoveDirection d) {
		return _d == 0 || _d == 4 ? _radiusY + _radiusY : _radiusX + _radiusX;
	}

};



template<class T>
class Square8Iterator : public BaseIterator<T> {
public:
	Square8Iterator (const Cursor<T>& R_CENTER, int radius, int radiusY = -1) :
	BaseIterator<T> (R_CENTER + cv::Point (-radius, radius + 1)),
	_radiusX (radius),
	_radiusY (radiusY == -1 ? radius : radiusY),
	_d ((MoveDirection)0),
	_indexInSeg (-1),
	_loop (false) { }

	bool moveNext () override {
		switch (_d) {
		case 0: this->_cursor += { 0, -_radiusY }; break;
		case 2: this->_cursor += { _radiusX, 0 }; break;
		case 4: this->_cursor += { 0, _radiusY }; break;
		case 6: this->_cursor += { -_radiusX, 0 }; break;
		}
		_indexInSeg++;
		if (_indexInSeg == 2) {
			_d = (MoveDirection)(_loop && _d == 6 ? 0 : _d + 2);
			_indexInSeg = 0;
		}
		return _d < 8;
	}

	bool movePrev () override {
		if (_indexInSeg == 0 && (_d != 0 || _loop)) {
			_d = (MoveDirection)(_d == 0 ? 6 : _d - 2);
			_indexInSeg = 2;
		}
		_indexInSeg--;
		switch (_d) {
		case 0: this->_cursor += { 0, _radiusY }; break;
		case 2: this->_cursor += { -_radiusX, 0 }; break;
		case 4: this->_cursor += { 0, -_radiusY }; break;
		case 6: this->_cursor += { _radiusX, 0 }; break;
		}
		return _indexInSeg >= 0;
	}

	bool BEYOND_END () const override { return _d >= 8; };

	bool BEFORE_START () const override { return _indexInSeg < 0; }

	void setLoop (bool loop) { _loop = loop; }

private:
	int _radiusX;
	int _radiusY;
	MoveDirection _d;
	int _indexInSeg;
	bool _loop;

};


/*
template<class T>
class DiamondIterator : public BaseIterator<T> {
public:
	DiamondIterator (const Cursor<T>& R_CENTER, int radius) :
		BaseIterator<T> (R_CENTER + cv::Point (-radius - 1, 1)),
		_radius (radius),
		_d ((MoveDirection)1),
		_indexInSeg (-1),
		_loop (false) { }

	bool moveNext () override {
		this->_cursor += _d;
		_indexInSeg++;
		if (_indexInSeg >= _radius) {
			_d = (MoveDirection)(_d + 2);
			_indexInSeg = 0;
		}
		if (_loop && _d == 9)
			_d = (MoveDirection)1;
		return _d < 9;
	}

	void setLoop (bool loop) { _loop = loop; }

private:
	int _radius;
	MoveDirection _d;
	int _indexInSeg;
	bool _loop;

};
*/


template<class T>
class LineIterator {
public:
	LineIterator (const cv::Mat& R_MAT, const cv::Point& R_START, const cv::Point& R_END) :
		_mat (R_MAT),
		_start (R_START),
		_v (R_END - R_START),
		_alongX (std::abs (_v.x) > std::abs (_v.y)),
		_i (0) {
	}

	T& operator* () {
		return _mat.at<T> (POSITION ());
	}

	bool moveNext () {
		_i++;
		return _alongX ? _i <= _v.x : _i <= _v.y;
	}
	cv::Point POSITION () const { return _alongX ? _start + _v * _i / _v.x : _start + _v * _i / _v.y; }

private:
	cv::Mat _mat;
	cv::Point _start;
	cv::Point _v;
	int _i;
	bool _alongX;

};

NS_DLIB_CVEXT_END