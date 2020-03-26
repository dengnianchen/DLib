#pragma once
#pragma warning (disable:4290)

#define DLIBCAM_INTERFACE

#ifndef NS_DLIB_CAM_DEFINE
#define NS_DLIB_CAM_DEFINE
#define NS_DLIB_CAM_BEGIN namespace Dlib { namespace Cam {
#define NS_DLIB_CAM_END	} }
#define NS_DLIB_CAM_EX_BEGIN NS_DLIB_CAM_BEGIN namespace Exception {
#define NS_DLIB_CAM_EX_END } NS_DLIB_CAM_END
#endif // !NS_DLIB_CAM_BEGIN
