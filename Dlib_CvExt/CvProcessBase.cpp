#include "stdafx.h"
#include "CvProcessBase.h"



NS_DLIB_CVEXT_BEGIN

ImageBGR24 CvProcessBase::_createDebugImage (const Image& R_IMAGE, bool empty) {

	if (!_isDebug) return ImageBGR24 ();
	if (empty)
		return ImageBGR24 (R_IMAGE.SIZE (), Color::Black);
	ImageBGR24 debugImage;
	R_IMAGE.CONVERT_TO (debugImage);
	return debugImage;

}



void CvProcessBase::_showDebugWindow (const std::string& R_WIN_NAME,
	cv::InputArray image) {

	if (!_isDebug) return;
	cv::imshow (R_WIN_NAME, image);
	_debugWindows.insert (R_WIN_NAME);
	_afterShowDebugWindow ();

}



void CvProcessBase::_showDebugWindow (const std::string& R_WIN_NAME,
	std::initializer_list<Image> images) {

	if (!_isDebug) return;
	cv::imshow (R_WIN_NAME, Image::concat (images));
	_debugWindows.insert (R_WIN_NAME);
	_afterShowDebugWindow ();

}



void CvProcessBase::_closeDebugWindows () {

	for (auto it = _debugWindows.begin (); it != _debugWindows.end (); ++it)
		cv::destroyWindow (*it);

}

NS_DLIB_CVEXT_END