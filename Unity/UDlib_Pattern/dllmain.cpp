#include "../../Dlib_Unity/All.h"
#include "../../Dlib_Pattern/Charuco.h"
#include "../../Dlib_Pattern/LineGrid.h"
#include "../../Dlib_Cam/Frame.h"

using namespace Dlib::Basic;
using namespace Dlib::Pattern;
using namespace Dlib::Unity;
using namespace Dlib::CvExt;
using namespace Dlib::Cam;



extern "C" {

UDLIB_INTERFACE(Charuco* Charuco_new (Vec2i patternSize, float squareLength)) {
	
	return new Charuco (patternSize, squareLength);
	
} END_UDLIB_INTERFACE



UDLIB_INTERFACE(LineGrid* LineGrid_new (Vec4f patternRegion, Vec2i patternSize)) {

	return new LineGrid (patternRegion, patternSize);

} END_UDLIB_INTERFACE



UDLIB_INTERFACE(void Pattern_delete (Pattern* p_inst)) {

	delete p_inst;

} END_UDLIB_INTERFACE



UDLIB_INTERFACE(void Pattern_draw (Pattern* p_inst, UImage img, Vec4b color)) {

	Image img1 = img;
	p_inst->DRAW (img1, color);

} END_UDLIB_INTERFACE



UDLIB_INTERFACE(Detector* Pattern_getDetector (Pattern* p_inst)) {

	return p_inst->GET_DETECTOR ();

} END_UDLIB_INTERFACE



UDLIB_INTERFACE (void Pattern_getCorners (Pattern* p_inst,
	ConstArrayRef<cv::Point2f>& o_corners)) {

	o_corners = p_inst->CORNERS ();

} END_UDLIB_INTERFACE



UDLIB_INTERFACE (void GridPattern_getXGrids (GridPattern* p_inst,
	ConstArrayRef<float>& o_values)) {

	o_values = p_inst->X_GRIDS;

} END_UDLIB_INTERFACE



UDLIB_INTERFACE (void GridPattern_getYGrids (GridPattern* p_inst,
	ConstArrayRef<float>& o_values)) {

	o_values = p_inst->Y_GRIDS;

} END_UDLIB_INTERFACE



UDLIB_INTERFACE(void Detector_delete (Detector* p_inst)) {

	delete p_inst;

} END_UDLIB_INTERFACE



UDLIB_INTERFACE (bool Detector_run (Detector* p_inst, UImage img,
	ArrayRef<cv::Point2f> resultsOut, UImage drawLayerOut, bool isDebug)) {

	return p_inst->run (img, resultsOut, drawLayerOut, isDebug);

} END_UDLIB_INTERFACE



UDLIB_INTERFACE (bool Detector_run2 (Detector* p_inst, Frame* p_frame, bool useUndistorted,
	ArrayRef<cv::Point2f> resultsOut, UImage drawLayerOut, bool isDebug)) {

	return p_inst->run (useUndistorted ? p_frame->UNDISTORTED_IMAGE () : p_frame->IMAGE (),
		resultsOut, drawLayerOut, isDebug);

} END_UDLIB_INTERFACE

}