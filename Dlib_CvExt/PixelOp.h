#pragma once
#include "global.h"
#include "../Dlib_Basic/Basic.h"



NS_DLIB_CVEXT_BEGIN

/**
 * @brief ��ȡͼ����ָ��λ�õ�����ֵ��

 * Ŀǰ֧�ֵ�OpenCV���Ͱ���CV_8UC[1-4]��CV_32FC[1-4]��
 *
 * @param const cv::Mat&	R_IMAGE	ͼ��
 * @param cv::Point			p		����
 *
 * @return cv::Scalar ����ֵ
 * @throw Dlib::Basic::Exception::InvalidArgument ����֧��ͼ���OpenCV����
 * @author Nianchen Deng
 */
cv::Scalar GetPixel (const cv::Mat& R_IMAGE, cv::Point p);

/**
 * @brief ��ȡͼ����ָ��λ�õ�����ֵ��
 *
 * cv::Scalar GetPixel (const cv::Mat&, cv::Point)�����ذ汾��
 *
 * @param const cv::Mat&	R_IMAGE	ͼ��
 * @param int				x		������
 * @param int				y		������
 *
 * @return cv::Scalar ����ֵ
 * @throw Dlib::Basic::Exception::InvalidArgument ����֧��ͼ���OpenCV����
 * @author Nianchen Deng
 */
cv::Scalar GetPixel (const cv::Mat& R_IMAGE, int x, int y);

/**
 * @brief ��ȡͼ����ָ��λ�õ�����ֵ��
 *
 * cv::Scalar GetPixel (const cv::Mat&, cv::Point)�����ذ汾��
 *
 * @param const cv::Mat&	R_IMAGE	ͼ��
 * @param int				i		һά����
 *
 * @return cv::Scalar ����ֵ
 * @throw Dlib::Basic::Exception::InvalidArgument ����֧��ͼ���OpenCV����
 * @author Nianchen Deng
 */
cv::Scalar GetPixel (const cv::Mat& R_IMAGE, int i);

/**
 * @brief ����ͼ����ָ��λ�õ�����ֵ��
 *
 * Ŀǰ֧�ֵ�OpenCV���Ͱ���CV_8UC[1-4]��CV_32FC[1-4]��
 *
 * @param const cv::Mat&	R_IMAGE	ͼ��
 * @param cv::Point			p		����
 * @param const cv::Scalar&	R_VALUE	����ֵ
 *
 * @throw Dlib::Basic::Exception::InvalidArgument ����֧��ͼ���OpenCV����
 * @author Nianchen Deng
 */
void SetPixel (cv::Mat& r_image, cv::Point p, const cv::Scalar& R_VALUE);

/**
 * @brief ����ͼ����ָ��λ�õ�����ֵ��
 *
 * void SetPixel (cv::Mat&, cv::Point, const cv::Scalar)�����ذ汾��
 *
 * @param const cv::Mat&	R_IMAGE	ͼ��
 * @param int				x		������
 * @param int				y		������
 * @param const cv::Scalar&	R_VALUE	����ֵ
 *
 * @throw Dlib::Basic::Exception::InvalidArgument ����֧��ͼ���OpenCV����
 * @author Nianchen Deng
 */
void SetPixel (cv::Mat& r_image, int x, int y, const cv::Scalar& R_VALUE);

/**
 * @brief ����ͼ����ָ��λ�õ�����ֵ��
 *
 * void SetPixel (cv::Mat&, cv::Point, const cv::Scalar)�����ذ汾��
 *
 * @param const cv::Mat&	R_IMAGE	ͼ��
 * @param int				i		һά����
 * @param const cv::Scalar&	R_VALUE	����ֵ
 *
 * @throw Dlib::Basic::Exception::InvalidArgument ����֧��ͼ���OpenCV����
 * @author Nianchen Deng
 */
void SetPixel (cv::Mat& r_image, int i, const cv::Scalar& R_VALUE);

NS_DLIB_CVEXT_END