#pragma once
#include "global.h"
#include "Image.h"



NS_DLIB_CVEXT_BEGIN

class CvProcessBase {
public:
	CvProcessBase (bool isDebug = false) : _isDebug (isDebug) { }
	virtual ~CvProcessBase () { _closeDebugWindows (); }

protected:
	void _setDebug (bool isDebug) { if (!(_isDebug = isDebug)) _closeDebugWindows (); }
	bool IS_DEBUG () const { return _isDebug; }

	/* 创建调试图像，empty指定创建全黑图像或复制输入图像 */
	ImageBGR24 _createDebugImage (const Image& R_IMAGE, bool empty);

	/* 显示调试窗口并绘制指定图像 */
	void _showDebugWindow (const std::string& R_WIN_NAME, cv::InputArray image);
	void _showDebugWindow (const std::string& R_WIN_NAME, std::initializer_list<Image> images);

	virtual void _afterShowDebugWindow () { }

private:
	bool _isDebug;
	std::set<std::string> _debugWindows;

	/* 关闭所有调试窗口 */
	void _closeDebugWindows ();

};

NS_DLIB_CVEXT_END
