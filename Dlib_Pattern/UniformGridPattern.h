#pragma once
#include "global.h"
#include "GridPattern.h"


NS_DLIB_PATTERN_BEGIN

class UniformGridPattern : public GridPattern {
public:
    UniformGridPattern (cv::Rect2f patternRegion, cv::Size patternSize);

private:
	static std::vector<float> _GetGrids (float start, float range, int size);

};

NS_DLIB_PATTERN_END