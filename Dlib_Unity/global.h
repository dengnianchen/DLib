#pragma once
#pragma warning (disable:4251)

// 添加要在此处预编译的标头
#include <string>
#include <glm/glm.hpp>
#include "../Dlib_Basic/ArrayRef.h"
#include "../Dlib_CvExt/All.h"

#ifdef DLIBUNITY_EXPORTS
#define DLIB_UNITY_INTERFACE __declspec(dllexport)
#else
#define DLIB_UNITY_INTERFACE __declspec(dllimport)
#endif

#ifndef NS_DLIB_UNITY_DEFINE
#define NS_DLIB_UNITY_DEFINE
#define NS_DLIB_UNITY_BEGIN namespace Dlib { namespace Unity {
#define NS_DLIB_UNITY_END	} }
#define NS_DLIB_UNITY_EX_BEGIN NS_DLIB_UNITY_BEGIN namespace Exception {
#define NS_DLIB_UNITY_EX_END } NS_DLIB_UNITY_END
#endif // !NS_DLIB_UNITY_DEFINE
