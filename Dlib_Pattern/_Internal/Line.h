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
		/* �Ƿ��ǿ����� */
		bool isEmpty ();
		/* ��ȡ����region��Χ����С�߶����� */
		Line getSubLine (const Line& r_line) const;
		/* ��image����color�������� */
		void draw (Dlib::CvExt::Image& r_image, Dlib::CvExt::Color color);
		/* ��image����color���ư�Χ�� */
		void drawBounding (Dlib::CvExt::Image& r_image, Dlib::CvExt::Color color);
		/* ���һ�����ӵ��¼�������� */
		void addConnection (Connection* p_conn);
		/* �������������Ľ��㣬����1Ϊ��������������2Ϊ�������� */
		static cv::Point2f getIntersectPoint (const Line& r_horLine, const Line& r_verLine);

	private:
		/* ���ַ�����С�ڵ���value�����Ԫ�� */
		int _findFloor (cv::Point2f value) const;
		static void _debugByDraw (Line horLine, Line verLine);

	};

} NS_DLIB_PATTERN_END
