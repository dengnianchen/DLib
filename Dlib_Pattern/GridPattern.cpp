#include "GridPattern.h"



NS_DLIB_PATTERN_BEGIN

GridPattern::GridPattern (const std::vector<float>& xGrids, const std::vector<float>& yGrids) :
X_GRIDS (xGrids),
Y_GRIDS (yGrids),
N_ROWS (yGrids.size ()),
N_COLS (xGrids.size ()),
N_ALL_CORNERS (xGrids.size () * yGrids.size ()) { }



const std::vector<cv::Point2f>& GridPattern::CORNERS () const {

	if (_corners.empty ()) {
		_corners.reserve (N_ALL_CORNERS);
		for (size_t r = 0; r < N_ROWS; ++r)
			for (size_t c = 0; c < N_COLS; ++c)
				_corners.push_back ({ X_GRIDS[c], Y_GRIDS[r] });
	}
	return _corners;

}

NS_DLIB_PATTERN_END