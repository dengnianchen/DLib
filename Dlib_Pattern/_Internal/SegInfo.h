#pragma once
#include "../global.h"
#include "LineSeq.h"



NS_DLIB_PATTERN_BEGIN namespace _Internal {

	class SegInfo {
	public:
		/**
		 * @brief 分段长度数组。
		 */
		std::vector<int> segLengths;
		/**
		 * @brief 分段起点（迭代器）数组。
		 */
		std::vector<Dlib::CvExt::SquareIterator<uchar>> segStarts;

		/**
		 * @brief 构造指定像素的周边分段信息。该分段信息记录了指定像素center周围半径
		 *        为radius的方形轨迹上黑白像素变化情况。
		 *
		 * @param center				中心像素
		 * @param radius				遍历半径
		 * @param segLengthThreshold	分段长度阈值，小于该值的分段将会被略去
		 * @param checkBoundary			在遍历时是否要检查边界条件。在不涉及图像边界
		 *								的遍历上，不检查边界条件能提供可观的速度优化。
		 */
		SegInfo (Dlib::CvExt::Cursor<uchar> center, int radius,
			int segLengthThreshold, bool checkBoundary = true) :
			_checkBoundary (checkBoundary) {
			_getRawSegs (Dlib::CvExt::SquareIterator<uchar> (center, radius),
				segLengths, segStarts);
			_filterSegs (segLengths, segStarts, segLengthThreshold);
		}

		/**
		 * @brief 获取指定分段的首、中、末三点坐标。
		 *
		 * @param index 指定分段索引
		 *
		 * @return 指定分段对应首点、中点、末点
		 */
		cv::Vec6f SEG_START_MIDDLE_END (int index) const {
			auto start = _START_OF_SEG (index);
			auto middle = _MIDDLE_OF_SEG (index);
			auto end = _END_OF_SEG (index);
			return cv::Vec6f (start.x, start.y, middle.x, middle.y, end.x, end.y);
		}

		/**
		 * @brief 获取指定分段对应的LinePoint::Port信息，包括指定分段首尾连线的中心点位置和宽度。
		 *
		 * @param index 指定分段索引
		 *
		 * @return 指定分段对应的LinePoint::Port信息
		 */
		LinePoint::Port SEG_TO_PORT (int index) const {
			auto start = _START_OF_SEG (index);
			auto end = _END_OF_SEG (index);
			return { (start + end) / 2.0f, CvExt::magnitude (end - start) + 1.0f };
		}

		/**
		 * @brief 获取所含分段数。
		 *
		 * @return 分段数
		 */
		size_t SIZE () const { return segLengths.size (); }

	private:
		bool _checkBoundary;

		/**
		 * @brief 获取指定方形轨迹上的原始分段信息。
		 *
		 * @param iter			方形迭代器指定要遍历的轨迹
		 * @param o_segLengths	[输出] 各分段的长度
		 * @param o_segStarts	[输出] 各分段的起点
		 */
		void _getRawSegs (Dlib::CvExt::SquareIterator<uchar> iter,
			std::vector<int>& o_segLengths,
			std::vector<Dlib::CvExt::SquareIterator<uchar>>& o_segStarts);

		/**
		 * @brief 根据指定长度阈值过滤合并分段信息。长度小于阈值的分段将会被略去，其前后分段将会合并。
		 *
		 * 原始分段：****.**...*..
		 *               V
		 * 滤后分段：*******......
		 *
		 * @param io_segLengths		[入出] 各分段的长度
		 * @param io_segStarts		[入出] 各分段的起点
		 * @param lengthThreshold	长度阈值
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