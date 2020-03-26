#pragma once
#include "global.h"



NS_DLIB_BASIC_BEGIN

template<class T>
class Ptr {
public:
	Ptr (T* ptr = 0);
	~Ptr ();

	void incrCount ();
	void decrCount ();
	int COUNT () const;
	void release ();
	T& operator*();
	T* operator->();
	operator T*();
	const T& operator*() const;
	const T* operator->() const;
	operator const T*() const;
	bool operator==(void* ptr) const;
	bool operator!=(void* ptr) const;
private:
	T* _ptr;
	int _count;
};

template<class T>
inline Ptr<T>::Ptr (T* ptr) : _ptr (ptr) {
	_count = ptr ? 1 : 0;
}

template<class T>
inline Ptr<T>::~Ptr () {
}

template<class T>
inline void Ptr<T>::incrCount () {
	if (_ptr)
		++_count;
}

template<class T>
inline void Ptr<T>::decrCount () {
	if (_ptr)
		--_count;
	if (_count <= 0) {
		delete _ptr;
		_ptr = 0;
	}
}

template<class T>
inline int Ptr<T>::COUNT () const {
	return _count;
}

template<class T>
inline void Ptr<T>::release () {
	delete _ptr;
	_count = 0;
	_ptr = 0;
}

template<class T>
inline T& Ptr<T>::operator* () {
	return *_ptr;
}

template<class T>
inline T* Ptr<T>::operator-> () {
	return _ptr;
}

template<class T>
inline Ptr<T>::operator T* () {
	return _ptr;
}

template<class T>
inline const T& Ptr<T>::operator* () const {
	return *_ptr;
}

template<class T>
inline const T* Ptr<T>::operator-> () const {
	return _ptr;
}

template<class T>
inline Ptr<T>::operator const T* () const {
	return _ptr;
}

template<class T>
inline bool Ptr<T>::operator== (void* ptr) const {
	return _ptr == ptr;
}

template<class T>
inline bool Ptr<T>::operator!= (void* ptr) const {
	return _ptr != ptr;
}

NS_DLIB_BASIC_END