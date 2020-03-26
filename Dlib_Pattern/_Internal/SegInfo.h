#pragma once
#include "../global.h"
#include "LineSeq.h"



NS_DLIB_PATTERN_BEGIN namespace _Internal {

	class SegInfo {
	public:
		/**
		 * @brief �ֶγ������顣
		 */
		std::vector<int> segLengths;
		/**
		 * @brief �ֶ���㣨�����������顣
		 */
		std::vector<Dlib::CvExt::SquareIterator<uchar>> segStarts;

		/**
		 * @brief ����ָ�����ص��ܱ߷ֶ���Ϣ���÷ֶ���Ϣ��¼��ָ������center��Χ�뾶
		 *        Ϊradius�ķ��ι켣�Ϻڰ����ر仯�����
		 *
		 * @param center				��������
		 * @param radius				�����뾶
		 * @param segLengthThreshold	�ֶγ�����ֵ��С�ڸ�ֵ�ķֶν��ᱻ��ȥ
		 * @param checkBoundary			�ڱ���ʱ�Ƿ�Ҫ���߽��������ڲ��漰ͼ��߽�
		 *								�ı����ϣ������߽��������ṩ�ɹ۵��ٶ��Ż���
		 */
		SegInfo (Dlib::CvExt::Cursor<uchar> center, int radius,
			int segLengthThreshold, bool checkBoundary = true) :
			_checkBoundary (checkBoundary) {
			_getRawSegs (Dlib::CvExt::SquareIterator<uchar> (center, radius),
				segLengths, segStarts);
			_filterSegs (segLengths, segStarts, segLengthThreshold);
		}

		/**
		 * @brief ��ȡָ���ֶε��ס��С�ĩ�������ꡣ
		 *
		 * @param index ָ���ֶ�����
		 *
		 * @return ָ���ֶζ�Ӧ�׵㡢�е㡢ĩ��
		 */
		cv::Vec6f SEG_START_MIDDLE_END (int index) const {
			auto start = _START_OF_SEG (index);
			auto middle = _MIDDLE_OF_SEG (index);
			auto end = _END_OF_SEG (index);
			return cv::Vec6f (start.x, start.y, middle.x, middle.y, end.x, end.y);
		}

		/**
		 * @brief ��ȡָ���ֶζ�Ӧ��LinePoint::Port��Ϣ������ָ���ֶ���β���ߵ����ĵ�λ�úͿ�ȡ�
		 *
		 * @param index ָ���ֶ�����
		 *
		 * @return ָ���ֶζ�Ӧ��LinePoint::Port��Ϣ
		 */
		LinePoint::Port SEG_TO_PORT (int index) const {
			auto start = _START_OF_SEG (index);
			auto end = _END_OF_SEG (index);
			return { (start + end) / 2.0f, CvExt::magnitude (end - start) + 1.0f };
		}

		/**
		 * @brief ��ȡ�����ֶ�����
		 *
		 * @return �ֶ���
		 */
		size_t SIZE () const { return segLengths.size (); }

	private:
		bool _checkBoundary;

		/**
		 * @brief ��ȡָ�����ι켣�ϵ�ԭʼ�ֶ���Ϣ��
		 *
		 * @param iter			���ε�����ָ��Ҫ�����Ĺ켣
		 * @param o_segLengths	[���] ���ֶεĳ���
		 * @param o_segStarts	[���] ���ֶε����
		 */
		void _getRawSegs (Dlib::CvExt::SquareIterator<uchar> iter,
			std::vector<int>& o_segLengths,
			std::vector<Dlib::CvExt::SquareIterator<uchar>>& o_segStarts);

		/**
		 * @brief ����ָ��������ֵ���˺ϲ��ֶ���Ϣ������С����ֵ�ķֶν��ᱻ��ȥ����ǰ��ֶν���ϲ���
		 *
		 * ԭʼ�ֶΣ�****.**...*..
		 *               V
		 * �˺�ֶΣ�*******......
		 *
		 * @param io_segLengths		[���] ���ֶεĳ���
		 * @param io_segStarts		[���] ���ֶε����
		 * @param lengthThreshold	������ֵ
		 */
		void _filterSegs (std::vector<int>& io_segLengths,
			std::vector<Dlib::CvExt::SquareIterator<uchar>>& io_segStarts,
			int lengthThreshold);

		cv::Point2f _START_OF_SEG (int index) const {
			return segStarts[index].POSITION ();
		}

		cv::Point2f _MIDDLE_OF_SEG (int index) const {
			auto segLength = segLengths[index];
			auto segIter = segStarts[index];
			segIter.setLoop (true);
			cv::Point2f middle;
			if (segLength % 2 == 0) {
				segIter.moveAhead (segLength / 2 - 1);
				middle = segIter.POSITION ();
				segIter.moveNext ();
				middle = (middle + (cv::Point2f)segIter.POSITION ()) / 2;
			} else {
				segIter.moveAhead (segLength / 2);
				middle = segIter.POSITION ();
			}
			return middle;
		}

		cv::Point2f _END_OF_SEG (int index) const {
			auto segEndIter = segStarts[(index + 1) % segStarts.size ()];
			segEndIter.setLoop (true);
			segEndIter.movePrev ();
			return segEndIter.POSITION ();
		}

	};

} NS_DLIB_PATTERN_END