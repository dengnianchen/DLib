#pragma once
#include "global.h"
#include <assert.h>
#include <vector>
#include <opencv2/opencv.hpp>



NS_DLIB_BASIC_BEGIN

enum ArrayRefType {
	ArrayRef_Native_Raw,
	ArrayRef_Native_StdVector,
	ArrayRef_Unity_Array,
	ArrayRef_Unity_SelfOwn
};



template<class T>
struct ConstArrayRef {
protected:
	T* _data;
	int _size;
	ArrayRefType _refType;
	void* _unityUseField;

public:
	ConstArrayRef () = default;
	ConstArrayRef (const T* data, int n) :
		_data ((T*)data), _size (n), _refType (ArrayRef_Native_Raw),
		_unityUseField (NULL) {}
	ConstArrayRef (const std::vector<T>& R__data) :
		_data ((T*)&R__data[0]), _size ((int)R__data.size ()),
		_refType (ArrayRef_Native_StdVector), _unityUseField (NULL) {}

	const T* DATA () const {
		return _data;
	}

	int SIZE () const {
		return _size;
	}

	const T& operator[] (int i) const {
		assert (i < _size);
		return _data[i];
	}

	operator cv::_InputArray () const {
		return cv::_InputArray (_data, _size);
	}

	std::vector<T> GET_COPY () const {
		std::vector<T> copy (_size);
		if (_size > 0)
			memcpy_s (&copy[0], _size * sizeof(T), _data, _size * sizeof(T));
		return copy;
	}

};



template<class T>
struct ArrayRef : public ConstArrayRef<T> {
public:
	ArrayRef (T* data, int n) : ConstArrayRef<T> (data, n) { }
	ArrayRef (std::vector<T>& r_data) : ConstArrayRef<T> (r_data) {}

	T* data () {
		return this->_data;
	}

	T& operator[] (int i) {
		assert (i < this->_size);
		return this->_data[i];
	}
	
	operator cv::Mat_<T> () {
		return cv::Mat_<T> (this->_size, 1, this->_data);
	}

	operator cv::_OutputArray () {
		return cv::_OutputArray (this->_data, this->_size);
	}

	operator cv::_InputOutputArray () {
		return cv::_InputOutputArray (this->_data, this->_size);
	}

};



template<class T>
struct Array2Ref {
private:
	T ** _data;
	const int _SIZE;
	const int* _SIZE1;
	const ArrayRefType _REF_TYPE;
	void *const _unityUseField;

public:
	Array2Ref () = default;

	Array2Ref (T** data, int size, const int* size1) :
		_data (data), _SIZE (size), _SIZE1 (size1),
		_REF_TYPE (ArrayRef_Native_Raw), _unityUseField (NULL) {}

	Array2Ref (const std::vector<std::vector<T>>& R_DATA) :
		_data ((T**)&R_DATA[0]), _SIZE (R_DATA.size ()),
		_REF_TYPE (ArrayRef_Native_StdVector), _unityUseField (NULL) {
		_SIZE1 = new const int[R_DATA.size ()];
		for (size_t i = 0; i < R_DATA.size (); ++i)
			(int&)_SIZE1[i] = (int)R_DATA[i].size ();
	}

	Array2Ref (const Array2Ref& R_SRC) :
		_data (R_SRC._data), _SIZE (R_SRC._SIZE),
		_REF_TYPE (R_SRC._REF_TYPE), _unityUseField (NULL) {
		if (_REF_TYPE == ArrayRef_Native_StdVector) {
			_SIZE1 = new const int[_SIZE];
			memcpy_s ((void*)_SIZE1, _SIZE * sizeof (int),
				R_SRC._SIZE1, _SIZE * sizeof (int));
		}
	}

	~Array2Ref () {
		if (_REF_TYPE == ArrayRef_Native_StdVector)
			delete[] _SIZE1;
	}

	ArrayRef<T> operator[] (size_t i) {
		assert (i < _SIZE);
		return ArrayRef<T> (_data[i], _SIZE1[i]);
	}

	ConstArrayRef<T> operator[] (size_t i) const {
		assert (i < _SIZE);
		return ConstArrayRef<T> (_data[i], _SIZE1[i]);
	}

	operator std::vector<cv::Mat_<T>> () {
		std::vector<cv::Mat_<T>> ret (_SIZE);
		for (int i = 0; i < _SIZE; ++i)
			ret[i] = (*this)[i];
		return ret;
	}

	std::vector<std::vector<T>> GET_COPY () const {
		std::vector<std::vector<T>> copy (_SIZE);
		for (int i = 0; i < _SIZE; ++i)
			copy[i] = (*this)[i].GET_COPY ();
		return copy;
	}

private:
	Array2Ref& operator= (const Array2Ref& R_SRC) = default;

};

NS_DLIB_BASIC_END