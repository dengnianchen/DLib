#include <iostream>
#include "../Dlib_CvExt/All.h"

using namespace Dlib::CvExt;



template<class T>
void assert_point (const cv::Point_<T>& r_a, T x, T y,const char* msg) {
	if (r_a.x == x && r_a.y == y)
		return;
	std::cerr << "assert point fail: " << msg << std::endl;
	abort ();
}



template<class T>
void assert_rect (const cv::Rect_<T>& r_a, T x, T y, T width, T height, const char* msg) {
	if (r_a.x == x && r_a.y == y && r_a.width == width && r_a.height == height)
		return;
	std::cerr << "assert rect fail: " << msg << std::endl;
	abort ();
}



void test_rect () {
	cv::Rect ri_a (0, 5, 10, 15), ri_b (20, 30, 40, 50);

	assert_rect (ri_a * 2, 0, 5, 20, 30, "cv::rect * s");
	assert_rect (ri_b / 2, 20, 30, 20, 25, "cv::rect / s");

	assert_rect (ri_a * cv::Size (3, 4), 0, 5, 30, 60, "cv::rect * size");
	assert_rect (ri_b / cv::Size (2, 5), 20, 30, 20, 10, "cv::rect / size");

	assert_rect (ri_a *= 10, 0, 5, 100, 150, "cv::rect *= s");
	assert_rect (ri_b /= 10, 20, 30, 4, 5, "cv::rect /= s");

	assert_rect (ri_a /= cv::Size (5, 3), 0, 5, 20, 50, "cv::rect /= size");
	assert_rect (ri_b *= cv::Size (6, 8), 20, 30, 24, 40, "cv::rect *= size");

	assert_point (cv::Point (ri_a >> Povit_Center), 10, 30, "rect povit(center)");
	assert_point (cv::Point (ri_a >> Povit_TopRight), 20, 5, "rect povit(tr)");
	assert_point (cv::Point (ri_b >> Povit_BottomRight), 44, 70, "rect povit(br)");
	assert_point (cv::Point (ri_b >> Povit_BottomLeft), 20, 70, "rect povit(bl)");

	assert_rect (cv::Rect ((ri_a >> Povit_Center) + cv::Point (11, 12) + cv::Size (10, 20)), 6, 7, 30, 70, "rect(center) + point + size");
	assert_rect (cv::Rect ((ri_b >> Povit_BottomRight) - cv::Point (11, 12) - cv::Size (10, 20)), 19, 38, 14, 20, "rect(br) - point - size");

	assert_rect (cv::Rect ((ri_a >> Povit_Center) * 2), -10, -20, 40, 100, "rect(center) * s");
	assert_rect (cv::Rect ((ri_b >> Povit_BottomRight) / 2), 32, 50, 12, 20, "rect(br) / s");

	((ri_a >> Povit_Center) += cv::Point (11, 12)) *= 2;
	((ri_b >> Povit_BottomRight) -= cv::Point (11, 12)) /= 2;
	assert_rect (ri_a, 1, -8, 40, 100, "rect(center) += point *= s");
	assert_rect (ri_b, 21, 38, 12, 20, "rect(center) -= point /= s");

	assert_point (cv::Point (cv::Rect (1, 2, 3, 4) >> Povit_Center), 2, 4, "rect povit(center) with odd size");
	assert_rect (cv::Rect ((cv::Rect (1, 2, 3, 4) >> Povit_Center) + cv::Size (2, 3)), 0, 1, 5, 7, "rect(center) with odd size");
	assert_rect (cv::Rect ((cv::Rect (3, 5, 7, 9) >> Povit_Center) / 2), 5, 7, 3, 4, "rect(center) with odd size");
}

int main () {
	test_rect ();
	return 0;
}
