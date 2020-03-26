#pragma once
#include "../global.h"
#include "LineSeq.h"



NS_DLIB_PATTERN_BEGIN namespace _Internal {

	class LineExtractor : public CvExt::CvProcessBase {
	public:
		LineExtractor (const CvExt::Image8& R_BINARY_IMAGE, bool isDebug) :
			CvProcessBase (isDebug),
			_R_BINARY_IMAGE (R_BINARY_IMAGE),
			_debugImage (_createDebugImage (R_BINARY_IMAGE, false)),
			_debugLineColors (7) {
			for (auto i = 0; i < _debugLineColors.size (); ++i) {
				auto hValue = 360.0f * i / _debugLineColors.size ();
				_debugLineColors[i] = CvExt::Color::HSV (hValue, 1.0f, 0.8f) (CvExt::EColor_BGR) * 255;
			}
		}
		~LineExtractor () { }

		bool run (std::vector<LineSeq*>& o_lines);

	private:
		CvExt::Image _debugImage;
		std::vector<CvExt::Color> _debugLineColors;
		const CvExt::Image8& _R_BINARY_IMAGE;

		void _extractLines (std::vector<LineSeq*>& o_lines);

		void _extractLinePoints (int radius, cv::Mat1b& io_typeMap, cv::Mat_<cv::Vec<float, 7>>& io_accFeatureMap);

		/*
		void _extractEndPoints (std::vector<LinePoint>& o_endPoints);

		void _extractEndPoints1 (int radius, cv::Mat1b& io_typeMap, cv::Mat4f& io_accFeatureMap);

		void _extractLines (const std::vector<LinePoint>& R_ENDPOINTS,
			std::vector<LineSeq>& o_lines);

		void _postprocessLines (std::vector<LineSeq>& io_lines);

		void _fuseEndPointInfo (cv::Mat1b& r_typeMap, cv::Mat4f& r_accFeatureMap,
			CvExt::BasicIterator<uchar> iter, uchar type, const LinePoint::Port& R_PORT);
		*/

		void _fuseEndPointInfo (cv::Mat1b& r_typeMap, cv::Mat_<cv::Vec<float, 7>>& r_accFeatureMap,
			CvExt::BasicIterator<uchar> iter, uchar type,
			const LinePoint::Port& R_IN, const LinePoint::Port& R_OUT);

		LinePoint _feature2Lp (cv::Point2f p,
			const cv::Mat_<cv::Vec<float, 7>>& R_ACC_FEATURE_MAP);

		LinePoint::Port _calculateOut (const LinePoint::Port& R_O,
			const LinePoint::Port& R_RAW_OUT, const cv::Mat_<ushort>& R_CC_LABEL_MAP,
			ushort label, bool& o_guess);

		void _afterShowDebugWindow () override { cv::waitKey (); }

		void _debugShowStep (const LineSeq& r_line, int lineId, int radius = -1);

	};

} NS_DLIB_PATTERN_END