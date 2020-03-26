#include "LineGridDetector.h"
#include "_Internal/LineGridDetector.h"

using namespace Dlib::Basic;
using namespace Dlib::CvExt;



NS_DLIB_PATTERN_BEGIN

LineGridDetector::LineGridDetector (const LineGrid& R_PATTERN) :
_p_private (new _Internal::LineGridDetector (R_PATTERN)) { }



LineGridDetector::~LineGridDetector () {

	delete (_Internal::LineGridDetector*)_p_private;

}



bool LineGridDetector::run (const Image& R_IMG,
	ArrayRef<cv::Point2f> r_resultsOut,
	Image drawLayer, bool isDebug) {

	return ((_Internal::LineGridDetector*)_p_private)->run (R_IMG,
		&r_resultsOut[0], drawLayer, isDebug);

}

NS_DLIB_PATTERN_END
