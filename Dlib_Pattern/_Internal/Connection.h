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
		/* ��ȡ��ߵĵ�һ������ */
		Connection* getLeft ();
		/* ��ȡ�ϱߵĵ�һ������ */
		Connection* getUp ();
		/* ��ȡ�ұߵĵ�һ������ */
		Connection* getRight ();
		/* ��ȡ�±ߵĵ�һ������ */
		Connection* getDown ();

	};

} NS_DLIB_PATTERN_END