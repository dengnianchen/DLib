#pragma once
#include "../global.h"
#include "Connection.h"



NS_DLIB_PATTERN_BEGIN namespace _Internal {

	class Line {
	public:
		enum Valid { UNKNOWN, VALID, INVALID };
		static cv::Size imageSize;

		cv::Point2f* points;
		int coord;
		int count;
		bool horizontal;
		bool copy;
		cv::Rect2f bounding;
		Valid valid;
		std::vector<Connection*> connPtrs;
		Connection* p_center;

		Line ();
		Line (cv::Point2f* points, int count, bool horizontal, bool copy = false);
		~Line ();
		/* 是否是空线条 */
		bool isEmpty ();
		/* 获取包含region范围的最小线段区间 */
		Line getSubLine (const Line& r_line) const;
		/* 在image上用color绘制线条 */
		void draw (Dlib::CvExt::Image& r_image, Dlib::CvExt::Color color);
		/* 在image上用color绘制包围盒 */
		void drawBounding (Dlib::CvExt::Image& r_image, Dlib::CvExt::Color color);
		/* 添加一个连接点记录（并排序） */
		void addConnection (Connection* p_conn);
		/* 计算两个线条的交点，参数1为横向线条，参数2为纵向线条 */
		static cv::Point2f getIntersectPoint (const Line& r_horLine, const Line& r_verLine);

	private:
		/* 二分法查找小于等于value的最大元素 */
		int _findFloor (cv::Point2f value) const;
		static void _debugByDraw (Line horLine, Line verLine);

	};

} NS_DLIB_PATTERN_END
