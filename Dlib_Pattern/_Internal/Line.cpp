#include "Line.h"
#include "../../Dlib_Basic/MathExt.h"

using namespace Dlib::CvExt;
using namespace Dlib::Basic;



NS_DLIB_PATTERN_BEGIN namespace _Internal {

cv::Size Line::imageSize;



Line::Line () {

	points = NULL;
	count = 0;
	valid = INVALID;
	copy = false;
	coord = -1000;
	p_center = NULL;

}



Line::Line (cv::Point2f* points, int count, bool horizontal, bool copy) {

	if (copy) {
		this->points = new cv::Point2f[count];
		memcpy (this->points, points, count * sizeof (cv::Point2f));
	} else
		this->points = points;
	this->count = count;
	this->horizontal = horizontal;
	this->valid = UNKNOWN;
	this->copy = copy;
	this->coord = -1000;
	this->p_center = NULL;

	if (isEmpty ())
		return;
	if (horizontal) {
		bounding.x = points[0].x;
		bounding.width = points[count - 1].x - points[0].x;
		float minVal = std::numeric_limits<float>::max ();
		float maxVal = -std::numeric_limits<float>::max ();
		for (int i = 0; i < count; ++i) {
			if (points[i].y < minVal)
				minVal = points[i].y;
			if (points[i].y > maxVal)
				maxVal = points[i].y;
		}
		bounding.y = minVal;
		bounding.height = maxVal - minVal;
	} else {
		bounding.y = points[0].y;
		bounding.height = points[count - 1].y - points[0].y;
		float minVal = std::numeric_limits<float>::max ();
		float maxVal = -std::numeric_limits<float>::max ();
		for (int i = 0; i < count; ++i) {
			if (points[i].x < minVal)
				minVal = points[i].x;
			if (points[i].x > maxVal)
				maxVal = points[i].x;
		}
		bounding.x = minVal;
		bounding.width = maxVal - minVal;
	}

}


Line::~Line () {

	if (copy)
		delete[] points;

}



/* 是否是空线条 */
bool Line::isEmpty () {

	return !points;

}



/* 获取包含region范围的最小线段区间 */
Line Line::getSubLine (const Line& r_line) const {

	if (r_line.count == 2) {
		auto& r_p0 = r_line.points[0];
		auto& r_p1 = r_line.points[1];

		if (r_line.horizontal) {
			float prevDy = 0.0f;
			for (int i = 0; i < count; ++i) {
				auto& r_p = points[i];
				float dy = r_p.y - (r_p1.y - r_p0.y) / (r_p1.x - r_p0.x) * (r_p.x - r_p0.x) - r_p0.y;
				if (i > 0 && dy * prevDy <= 0.0f)
					return Line (points + i - 1, 2, horizontal);
				prevDy = dy;
			}
			return Line ();
		} else {
			float prevDx = 0.0f;
			for (int i = 0; i < count; ++i) {
				auto& r_p = points[i];
				float dx = r_p.x - (r_p1.x - r_p0.x) / (r_p1.y - r_p0.y) * (r_p.y - r_p0.y) - r_p0.x;
				if (i > 0 && dx * prevDx <= 0.0f)
					return Line (points + i - 1, 2, horizontal);
				prevDx = dx;
			}
			return Line ();
		}
	}
	int subLineStart = _findFloor (r_line.bounding.tl ());
	int subLineEnd = _findFloor (r_line.bounding.br ()) + 1;
	if (subLineEnd >= count)
		subLineEnd = count - 1;
	if (subLineEnd <= subLineStart)
		return Line ();
	return Line (points + subLineStart,
		subLineEnd - subLineStart + 1, horizontal);

}



/* 在image上用color绘制线条 */
void Line::draw (Image& r_image, Color color) {

	Painter painter (r_image);
	for (int i = 0; i < count - 1; ++i)
		painter.line (points[i], points[i + 1], color, 1);

}



/* 在image上用color绘制包围盒 */
void Line::drawBounding (Image& r_image, Color color) {

	Painter (r_image).rectangle (bounding, color, 1);

}



/* 添加一个连接点记录（并排序） */
void Line::addConnection (Connection* p_conn) {

	connPtrs.push_back (p_conn);
	if (horizontal) {
		// 按connection的x轴排序
		for (int i = (int)connPtrs.size () - 1; i > 0; --i) {
			if (connPtrs[i]->p.x >= connPtrs[i - 1]->p.x)
				return;
			std::swap (connPtrs[i], connPtrs[i - 1]);
		}
	} else {
		// 按connection的y轴排序
		for (int i = (int)connPtrs.size () - 1; i > 0; --i) {
			if (connPtrs[i]->p.y >= connPtrs[i - 1]->p.y)
				return;
			std::swap (connPtrs[i], connPtrs[i - 1]);
		}
	}
	return;

}



/* 计算两个线条的交点，参数1为横向线条，参数2为纵向线条 */
cv::Point2f Line::getIntersectPoint (const Line& r_horLine, const Line& r_verLine) {

	//_debugByDraw (r_horLine, r_verLine);
	if (r_horLine.count == 2 && r_verLine.count == 2) {
		auto& r_p0 = r_horLine.points[0];
		auto& r_p1 = r_horLine.points[1];
		float dy0 = r_verLine.points[0].y - (r_p1.y - r_p0.y) / (r_p1.x - r_p0.x) * (r_verLine.points[0].x - r_p0.x) - r_p0.y;
		float dy1 = r_verLine.points[1].y - (r_p1.y - r_p0.y) / (r_p1.x - r_p0.x) * (r_verLine.points[1].x - r_p0.x) - r_p0.y;
		float k = -dy0 / (dy1 - dy0);
		auto intersectPoint = (r_verLine.points[1] - r_verLine.points[0]) * k + r_verLine.points[0];
		if (intersectPoint.x >= r_p0.x &&
			intersectPoint.x <= r_p1.x &&
			intersectPoint.y >= r_verLine.points[0].y &&
			intersectPoint.y <= r_verLine.points[1].y)
			return intersectPoint;
		return { -1.0f, -1.0f };
	} else {
		auto subline2 = r_verLine.getSubLine (r_horLine);
		if (subline2.isEmpty ())
			return { -1.0f, -1.0f };
		//_debugByDraw (r_horLine, subline2);
		auto subline1 = r_horLine.getSubLine (subline2);
		if (subline1.isEmpty ())
			return { -1.0f, -1.0f };
		if (subline1.count == r_horLine.count && subline2.count == r_verLine.count) {
			for (int i = 0; i < subline1.count - 1; ++i) {
				for (int j = 0; j < subline2.count - 1; ++j) {
					auto p = getIntersectPoint (
						Line (subline1.points + i, 2, subline1.horizontal),
						Line (subline2.points + j, 2, subline2.horizontal));
					if (p.x >= 0)
						return p;
				}
			}
			return { -1.0f, -1.0f };
		}
		return getIntersectPoint (subline1, subline2);
	}
}



/* 二分法查找小于等于value的最大元素 */
int Line::_findFloor (cv::Point2f value) const {

	if (horizontal) {
		if (points[0].x > value.x)
			return 0;
		if (points[count - 1].x < value.x)
			return count - 1;
		int start = 0, end = count;
		while (end - start > 1) {
			int center = (start + end) / 2;
			if (Math::approximately (points[center].x, value.x))
				return center;
			if (points[center].x < value.x)
				start = center;
			else
				end = center;
		}
		return start;
	} else {
		if (points[0].y > value.y)
			return 0;
		if (points[count - 1].y < value.y)
			return count - 1;
		int start = 0, end = count;
		while (end - start > 1) {
			int center = (start + end) / 2;
			if (Math::approximately (points[center].y, value.y))
				return center;
			if (points[center].y < value.y)
				start = center;
			else
				end = center;
		}
		return start;
	}

}



void Line::_debugByDraw (Line horLine, Line verLine) {

	if (imageSize.empty ())
		return;
	ImageBGR24 debugImage (imageSize, Color::BGR (0, 0, 0));
	horLine.draw (debugImage, Color::BGR (0, 0, 255));
	verLine.draw (debugImage, Color::BGR (0, 255, 0));
	horLine.drawBounding (debugImage, Color::BGR (0, 128, 255));
	verLine.drawBounding (debugImage, Color::BGR (255, 255, 0));
	cv::imshow ("step debug", debugImage);
	cv::waitKey (0);

}

} NS_DLIB_PATTERN_END