#pragma once
#include "global.h"
#include "Pattern.h"



NS_DLIB_PATTERN_BEGIN

class GridPattern : public Pattern {
public:
	const size_t N_ROWS;
	const size_t N_COLS;
	const size_t N_ALL_CORNERS;
	const std::vector<float> X_GRIDS;
	const std::vector<float> Y_GRIDS;
	
	GridPattern (const std::vector<float>& xGrids, const std::vector<float>& yGrids);
	virtual const std::vector< cv::Point2f >& CORNERS () const override;

private:
	mutable std::vector< cv::Point2f > _corners;

};

NS_DLIB_PATTERN_END