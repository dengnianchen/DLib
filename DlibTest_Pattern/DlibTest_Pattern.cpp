#include <iostream>
#include "../Dlib_CvExt/All.h"
#include "../Dlib_Pattern/Charuco.h"
#include "../Dlib_Pattern/LineGrid.h"

using namespace Dlib::Pattern;
using namespace Dlib::CvExt;



void test_charuco () {

	Pattern* p_pattern = new Charuco ({ 8, 6 }, 10.0f);
	ImageBGR24 patternImage (800, 600);
	p_pattern->DRAW (patternImage);
	std::vector<cv::Point2f> results (48);
	ImageBGR32 drawLayer (800, 600, Color::Black);
	p_pattern->GET_DETECTOR ()->run (patternImage, results, drawLayer);
	for (int i = 0; i < 48; ++i) {
		if (results[i].x == NAN)
			continue;
		std::ostringstream sout;
		sout << i;
		cv::putText (patternImage, sout.str (), results[i],
			cv::FONT_HERSHEY_PLAIN, 1.0f, { 0, 0, 255 });
		Painter (patternImage, 2).circle (
			results[i], 2.0f, Color::BGR (0, 0, 255));
	}
	//cv::imshow ("Charuco 6x8", patternImage);
	cv::imshow ("Charuco 6x8 draw", drawLayer);
	cv::waitKey ();
	//cv::destroyWindow ("Charuco 6x8");
	cv::destroyWindow ("Charuco 6x8 draw");
	delete p_pattern;

}



void test_lineGrid () {

	Pattern* p_pattern = new LineGrid ({ 10, 10, 780, 580 }, { 13, 9 });
	ImageBGR24 patternImage (800, 600);
	p_pattern->DRAW (patternImage);
	std::vector<cv::Point2f> results (13 * 9);
	ImageBGR32 drawLayer (800, 600, Color::Black);
	p_pattern->GET_DETECTOR ()->run (patternImage, results, drawLayer, true);
	for (int i = 0; i < 13 * 9; ++i) {
		if (results[i].x == NAN)
			continue;
		std::ostringstream sout;
		sout << i;
		cv::putText (patternImage, sout.str (), results[i],
			cv::FONT_HERSHEY_PLAIN, 1.0f, { 0, 0, 255 });
		Painter (patternImage).circle (
			results[i], 2.0f, Color::BGR (0, 0, 255));
	}
	cv::imshow ("LineGrid 13x9", patternImage);
	cv::imshow ("LineGrid 13x9 draw", drawLayer);
	cv::waitKey ();
	cv::destroyWindow ("LineGrid 13x9");
	cv::destroyWindow ("LineGrid 13x9 draw");
	delete p_pattern;
}



void test_lineGrid1 (int i) {

	Pattern* p_pattern = new LineGrid ({ 10, 15, 60, 370 }, { 3, 7 });
	ImageBGR24 patternImage (80, 400);
	p_pattern->DRAW (patternImage);

	std::ostringstream sout; sout << "frame_" << i << ".png";
	Image cameraImage (cv::imread(sout.str ()));
	std::vector<cv::Point2f> results (3 * 7);
	ImageBGR32 drawLayer (cameraImage.SIZE (), Color::Black);
	p_pattern->GET_DETECTOR ()->run (cameraImage, results, drawLayer, true);
	for (int i = 0; i < results.size (); ++i) {
		if (results[i].x == NAN)
			continue;
		std::ostringstream sout;
		sout << i;
		cv::putText (cameraImage, sout.str (), results[i],
			cv::FONT_HERSHEY_PLAIN, 1.0f, { 0, 0, 255 });
		Painter (cameraImage).circle (results[i], 2.0f, Color::BGR (0, 0, 255));
	}
	//cv::imshow ("LineGrid 3x7 camera", cameraImage);
	cv::imshow ("LineGrid 3x7 draw: " + sout.str (), drawLayer);
	cv::waitKey ();
	//cv::destroyWindow ("LineGrid 3x7 camera");
	cv::destroyWindow ("LineGrid 3x7 draw: " + sout.str ());
	delete p_pattern;
}


int main () {
	// test_charuco ();
	//test_lineGrid ();
	for (int i = 1; i < 19; ++i)
	test_lineGrid1 (i);
	return 0;

}