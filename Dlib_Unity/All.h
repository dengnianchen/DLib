#pragma once
#include "global.h"
#include "Framework.h"
#include "Str.h"
#include "Vec.h"
#include "UImage.h"
#include "UMatrix.h"



#define UDLIB_INTERFACE(__define__) __declspec(dllexport) __define__ try
#define END_UDLIB_INTERFACE \
	catch (const std::exception& ex) { \
		Dlib::Unity::Framework::ThrowEx (ex); \
		abort (); \
	} catch (const Dlib::Basic::Exception::Exception& ex) { \
		Dlib::Unity::Framework::ThrowEx (ex); \
		abort (); \
	}