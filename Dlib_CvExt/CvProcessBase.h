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

	/* ��������ͼ��emptyָ������ȫ��ͼ���������ͼ�� */
	ImageBGR24 _createDebugImage (const Image& R_IMAGE, bool empty);

	/* ��ʾ���Դ��ڲ�����ָ��ͼ�� */
	void _showDebugWindow (const std::string& R_WIN_NAME, cv::InputArray image);
	void _showDebugWindow (const std::string& R_WIN_NAME, std::initializer_list<Image> images);

	virtual void _afterShowDebugWindow () { }

private:
	bool _isDebug;
	std::set<std::string> _debugWindows;

	/* �ر����е��Դ��� */
	void _closeDebugWindows ();

};

NS_DLIB_CVEXT_END
