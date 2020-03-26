#pragma once
#include "Vec.h"



NS_DLIB_UNITY_BEGIN

class DLIB_UNITY_INTERFACE UImage {
public:
	CvExt::PixelFormat format;
	Vec2i size;
	void* data;

	UImage& operator= (const UImage&) = default;
	/**
	 * ���ָ��ͼ�����ݵ����ø������ò������Ḵ��ͼ�����ݡ�
	 */
	UImage& operator= (const CvExt::Image& R_IMAGE);
	/**
	 * ��ָ��ͼ�����ݸ��ƣ����䣩����ǰͼ���У��ò�������ı䵱ǰͼ��ĸ�ʽ�ʹ�С��
	 * �����ߵĸ�ʽ���С��ͬ����ִ����Ӧ��ת��
	 */
	UImage& operator<< (const CvExt::Image& R_IMAGE);
	/**
	 * ����ǰͼ�����ݵ����ø�����Dlib::CvExt::Image���ͷ��ء��ò������Ḵ��ͼ�����ݡ�
	 */
	operator CvExt::Image ();
	/**
	 * ����ǰͼ�����ݵ����ø�����cv::Mat���ͷ��ء��ò������Ḵ��ͼ�����ݡ�
	 */
	operator cv::Mat ();

};

NS_DLIB_UNITY_END