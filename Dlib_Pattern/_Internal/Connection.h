#pragma once
#include "../global.h"



NS_DLIB_PATTERN_BEGIN namespace _Internal {

	class Line;

	class Connection {
	public:
		cv::Point2f p;
		cv::Point coord;
		Line* p_horLine;
		int connIndexInHorLine;
		Line* p_verLine;
		int connIndexInVerLine;

		Connection (cv::Point2f p, Line* p_horLine, Line* p_verLine);
		bool isValid ();
		bool isCoordSet ();
		/* 获取左边的第一个交点 */
		Connection* getLeft ();
		/* 获取上边的第一个交点 */
		Connection* getUp ();
		/* 获取右边的第一个交点 */
		Connection* getRight ();
		/* 获取下边的第一个交点 */
		Connection* getDown ();

	};

} NS_DLIB_PATTERN_END