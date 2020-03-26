#pragma once
#include "global.h"
#include "../Dlib_Basic/Basic.h"



NS_DLIB_CVEXT_BEGIN

/**
 * ��ɫ�ռ�ö�ٱ���
 */
enum EColorSpace {
	EColor_Unknown = -1,	/* δ֪ */
	EColor_Mono,			/* ��ɫ���Ҷȣ���֧�ֵ�ͨ����ʽ */
	EColor_BGR,				/* ��ɫ�ռ䣨B��λ˳�򣩣�֧��3-4ͨ����ʽ */
	EColor_RGB,				/* ��ɫ�ռ䣨R��λ˳�򣩣�֧��3-4ͨ����ʽ */
	EColor_HSV				/* HSV�ռ䣬֧��3ͨ����������ʽ */
};



/**
 * ���OpenCV���������Ƿ�֧��ָ������ɫ�ռ䡣
 *
 * ĿǰOpenCV�������ͽ�֧����õ�8λ��ȣ���CV_8UC[1/3/4]��HSV��ɫ�ռ��֧��CV_32FC3����
 * δ֪����ɫ�ռ䣨EColor_Unknown������Ϊ֧������OpenCV�������͡�
 * δ֪��OpenCV�������ͣ�-1������Ϊ֧��������ɫ�ռ䡣
 *
 * @param EColorSpace colorSpace	��ɫ�ռ�ö������
 * @param int cvType				OpenCV��������
 *
 * @return bool OpenCV���������Ƿ�֧��ָ������ɫ�ռ�
 * @author Nianchen Deng
 */
bool ColorSpaceSupport (EColorSpace colorSpace, int cvType);



/**
 * ö������EFormat�İ�װ�࣬������ͼ���б�ʾ��ͬ�����ظ�ʽ��
 */
class PixelFormat {
public:
	/**
	 * ���ظ�ʽö�ٱ���
	 */
	enum EFormat {
		EFormat_Unknown = -1,	/* δ֪ */
		EFormat_Mono8,			/* 8λ��ɫ���Ҷȣ�����ӦCV_8UC1 */
		EFormat_BGR,			/* 24λBGR��ɫ����ӦCV_8UC3 */
		EFormat_BGR32,			/* 32λBGR��ɫ����Alphaͨ��������ӦCV_8UC4 */
		EFormat_RGB,			/* 24λRGB��ɫ����ӦCV_8UC3 */
		EFormat_RGB32,			/* 32λRGB��ɫ����Alphaͨ��������ӦCV_8UC4 */
		EFormat_HSV				/* 3ͨ��������HSV��ɫ����ӦCV_32FC3 */
	};

	/**
	 * EFormatö��ֵ����PixelFormat��
	 *
	 * @param EFormat value ����ѡ��ֵ��Ĭ��ΪEFormat_Unknown
	 *
	 * @author Nianchen Deng
	 */
	PixelFormat (EFormat value = EFormat_Unknown);
	/**
	 * ������ɫ�ռ��OpenCV���������Ƶ�����PixelFormat��
	 *
	 * ��ָ������ɫ�ռ䲻֧��ָ����OpenCV�������ͣ���PixelFormat��ֵΪEFormat_Unknown
	 *
	 * @param EColorSpace colorSpace	��ɫ�ռ�
	 * @param int cvType				����ѡ��OpenCV�������͡���δָ�����Զ��ƶ����ظ�ʽ��
	 *									������BGR��RGBĬ��Ϊ4ͨ�����͡�
	 *
	 * @author Nianchen Deng
	 */
	PixelFormat (EColorSpace colorSpace, int cvType = -1);
	/**
	 * ����OpenCV���������Ƶ�����PixelFormat��
	 *
	 * ����������Ϊδ֪��-1����Ϊ����֧�ֵ����ͣ���PixelFormatֵΪEFormat_Unknown
	 *
	 * @param int cvType OpenCV�������ͣ�����CV_8UC3/CV_8UC4��Ĭ��ΪBGR/BGR32��ʽ
	 *
	 * @author Nianchen Deng
	 */
	PixelFormat (int cvType);
	/**
	 * ���ظ�ֵ��������
	 *
	 * @param const PixelFormat& right ��ֵ����������ֵ
	 *
	 * @return PixelFormat&
	 * @author Nianchen Deng
	 */
	PixelFormat& operator= (const PixelFormat& right) = default;
	/**
	 * ���ظ�ֵ��������
	 *
	 * @param const PixelFormat& right ��ֵ����������ֵ
	 *
	 * @return PixelFormat&
	 * @author Nianchen Deng
	 */
	PixelFormat& operator= (EFormat right);

	/**
	 * EFormat��ʽת����������
	 *
	 * �˲�������֤PixelFormat�ɱ�������ͨ��ö������EFormatʹ���ڵ�ֵ�жϻ�switch����С�
	 *
	 * @return EFormat ��Ӧ��ö��ֵ 
	 * @author Nianchen Deng
	 */
	operator EFormat () const;

	/**
	 * �ж����ظ�ʽ�Ƿ���ָ��OpenCV��������ƥ�䡣
	 *
	 * δ֪���ظ�ʽ��EFormat_Unknown����OpenCV�������ͣ�-1������Ϊ���κ�OpenCV
	 * �������ͻ����ظ�ʽƥ�䡣
	 *
	 * @param int cvType ָ��OpenCV��������
	 *
	 * @return bool ���ظ�ʽ�Ƿ���ָ��OpenCV��������ƥ��
	 * @author Nianchen Deng
	 */
	bool MATCH (int cvType) const;
	/**
	 * ��ȡ���ظ�ʽ����Ӧ����ɫ�ռ䡣
	 *
	 * @return EColorSpace ��Ӧ����ɫ�ռ�
	 * @author Nianchen Deng
	 */
	EColorSpace COLOR_SPACE () const;
	/**
	 * ��ȡ��Ӧ��OpenCV�������͡�
	 *
	 * �����ظ�ʽΪδ֪��EFormat_Unknown�����򷵻�-1��
	 *
	 * @return int ��Ӧ��OpenCV��������
	 * @author Nianchen Deng
	 */
	int CV_TYPE () const;

	/**
	 * ����ָ�������ظ�ʽִ��ɫ��ת����
	 *
	 * �÷����޷�������ͬ���ظ�ʽ֮���ת����Ӧ�ɵ��÷������ж�Դ��Ŀ������ظ�ʽ�Ƿ���ͬ��
	 *
	 * @param const cv::Mat&	R_SRC		Դͼ������
	 * @param cv::Mat&			r_dst		�������Ŀ��ͼ������
	 * @param PixelFormat		srcFormat	Դͼ�����ظ�ʽ
	 * @param PixelFormat		dstFormat	Ŀ��ͼ�����ظ�ʽ��������Դͼ�����ظ�ʽ��ͬ
	 *
	 * @throw Dlib::Basic::Exception::InvalidArgument ����ָ���ĸ�ʽ��������Ͳ�ƥ����໥֮���޷�����ת��
	 * @author Nianchen Deng
	 */
	static void CvtColor (const cv::Mat& R_SRC, cv::Mat& r_dst,
		PixelFormat srcFormat, PixelFormat dstFormat);
	
private:
	EFormat _value;

};

NS_DLIB_CVEXT_END