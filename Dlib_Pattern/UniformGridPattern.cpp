#include "UniformGridPattern.h"



NS_DLIB_PATTERN_BEGIN

UniformGridPattern::UniformGridPattern (cv::Rect2f patternRegion, cv::Size patternSize) :
GridPattern (_GetGrids (patternRegion.x, patternRegion.width, patternSize.width),
	_GetGrids (patternRegion.y, patternRegion.height, patternSize.height)) {

}



std::vector<float> UniformGridPattern::_GetGrids (
	float start, float range, int size) {

	std::vector<float> grids (size);
	for (int i = 0; i < grids.size (); ++i)
		grids[i] = start + range / (size - 1) * i;
	return grids;

}

NS_DLIB_PATTERN_END