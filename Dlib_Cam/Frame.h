#pragma once
#include "global.h"
#include "../Dlib_CvExt/All.h"
#include "Intrinsic.h"



NS_DLIB_CAM_BEGIN

class Camera;

/**
 * @brief Frame�����ڱ�ʾ����Ļ���֡������֡����ͼ��ʱ�����
 *        Ϊ�˷���ʹ�ã�Frame���ౣ���˶���������������á�
 */
class DLIBCAM_INTERFACE Frame {
public:
	/**
	 * @brief ������ĳ�������
	 */
	const Camera& R_CAMERA;

	/**
	 * @brief ��ȡ��ǰ����֡��ʱ�������ֵ���豸��صĵ�����SDK�ṩ��
	 * @return ��ǰ����֡��ʱ���
	 */
	uint64 TIMESTAMP () const;
	/**
	 * @brief ��ȡ��ǰ֡��ͼ�����ݡ�
	 * @return ��ǰ֡��ͼ������
	 */
	const CvExt::Image& IMAGE () const;
	/**
	 * @brief ��ȡ������������ĵ�ǰ֡ͼ������ǰ���ڲα궨���ݣ��򷵻�ԭʼͼ��
	 * @return ������������ĵ�ǰ֡ͼ��
	 */
	const CvExt::Image& UNDISTORTED_IMAGE () const;

private:
	friend class Camera;

	/**
	 * @brief ͼ���С
	 */
	cv::Size _size;
	/**
	 * @brief �������õ�ͼ��
	 */
	CvExt::Image _cvImg;
	/**
	 * @brief ��������û���֡ʱ��ʱ���
	 */
	uint64 _timestamp;
	/**
	 * @brief ����������֡ͼ��
	 */
	mutable CvExt::Image _cvUndistortedImg;
	/**
	 * @brief �Ƿ���Ҫ����_cvUndistortedImg
	 */
	mutable bool _undistortedImageDirty;

	/**
	 * @brief ����һ������֡���÷�����SpinnakerCamera����ã��ⲿ���贴��Frame����
	 * @param R_CAMERA ����������
	 */
	Frame (const Camera& R_CAMERA);

};

NS_DLIB_CAM_END