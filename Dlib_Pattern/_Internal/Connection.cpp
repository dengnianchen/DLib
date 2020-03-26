#include "Connection.h"
#include "Line.h"



NS_DLIB_PATTERN_BEGIN namespace _Internal {

Connection::Connection (cv::Point2f p, Line* p_horLine, Line* p_verLine) {

	this->coord = {
		std::numeric_limits<int>::max (),
		std::numeric_limits<int>::max ()
	};
	this->p = p;
	this->p_horLine = p_horLine;
	this->p_verLine = p_verLine;
	p_horLine->addConnection (this);
	p_verLine->addConnection (this);

}



bool Connection::isValid () {

	return p_horLine->valid == Line::VALID &&
		p_verLine->valid == Line::VALID;

}



bool Connection::isCoordSet () {

	return coord.x >= 0 && coord.y >= 0;

}



/* 获取左边的第一个交点 */
Connection* Connection::getLeft () {

	if (connIndexInHorLine <= 0)
		return NULL;
	return p_horLine->connPtrs[connIndexInHorLine - 1];

}



/* 获取上边的第一个交点 */
Connection* Connection::getUp () {

	if (connIndexInVerLine <= 0)
		return NULL;
	return p_verLine->connPtrs[connIndexInVerLine - 1];

}



/* 获取右边的第一个交点 */
Connection* Connection::getRight () {

	if (connIndexInHorLine >= p_horLine->connPtrs.size () - 1)
		return NULL;
	return p_horLine->connPtrs[connIndexInHorLine + 1];

}



/* 获取下边的第一个交点 */
Connection* Connection::getDown () {

	if (connIndexInVerLine >= p_verLine->connPtrs.size () - 1)
		return NULL;
	return p_verLine->connPtrs[connIndexInVerLine + 1];

}

} NS_DLIB_PATTERN_END