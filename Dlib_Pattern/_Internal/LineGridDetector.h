#pragma once
#include "../global.h"
#include "../LineGrid.h"
#include "Line.h"
#include "LineSeq.h"



NS_DLIB_PATTERN_BEGIN namespace _Internal {

	class LineGridDetector : public CvExt::CvProcessBase {
	public:
		LineGridDetector (const LineGrid& R_PATTERN) :
			_R_PATTERN (R_PATTERN),
			_pattSize ((int)R_PATTERN.N_COLS, (int)R_PATTERN.N_ROWS),
			_debugExtractLines (false), _debugExtractCross (false) {
		}

		virtual ~LineGridDetector () { }

		bool run (const CvExt::Image& R_IMG, cv::Point2f* results,
			CvExt::Image& r_drawLayer, bool isDebug);

	private:
		typedef std::vector<cv::Point> Contour;

		const LineGrid& _R_PATTERN;
		cv::Size _pattSize;
		CvExt::Image8 _lastFrameImage;

		bool _debugExtractLines;
		bool _debugExtractCross;

		CvExt::Image8 _preprocessInputImage (const CvExt::Image& R_IMG);

		CvExt::Image8 _adjustConstrast (CvExt::Image8 gray);

		int _extractContour (const CvExt::Image8& R_BINARY_IMAGE,
			std::vector<Contour>& o_contours,
			std::vector<cv::Vec4i>& o_hierarchy);

		bool _analyzeContours (const CvExt::Image8& R_BINARY_IMAGE,
			const std::vector<Contour>& R_CONTOURS,
			const std::vector<cv::Vec4i>& R_HIERARCHY, int mainContourIdx,
			cv::Rect& o_roi, std::vector<cv::KeyPoint>& o_blobs,
			std::vector<int>& o_patternLineContourIdxs);

		void _drawSubImageOfPatternLines (
			const std::vector<Contour>& R_CONTOURS,
			const std::vector<cv::Vec4i>& R_HIERARCHY,
			const std::vector<int>& R_IDXS, const cv::Rect& R_ROI,
			CvExt::Image8& o_patternImage);

		cv::Mat _extractSubImageOfPatternByContourAndRotate (
			const CvExt::Image8& R_BINARY,
			const CvExt::Image8& R_GRAY,
			CvExt::Image8& o_binary,
			CvExt::Image8& o_gray);

		bool _isLinePixel (const CvExt::Cursor<uchar>& R_CURSOR_CENTER,
			int& o_bgIntensity);

		std::vector<cv::KeyPoint> _detectBlobs (
			const std::vector<Contour>& R_CONTOURS, std::vector<int>& io_idxs);

		/* （调试）绘制网格线检测结果 */
		void _drawGridDetectResult (CvExt::Image& r_drawLayer, cv::Rect roi,
			const std::vector<LineSeq*>& R_LINES,
			const std::vector<cv::KeyPoint>& R_BLOBS);

		void _drawGridDetectResult (CvExt::Image& r_drawLayer, cv::Rect roi,
			const std::vector<LineSeq*>& R_LINES,
			const std::vector<Conn*>& R_CONNS,
			cv::Point2f origin = { -1.0f, -1.0f });

		void _debugDrawExtractLinesResult (CvExt::Painter painter,
			const std::vector<LineSeq*>& R_LINES,
			const std::vector<Conn*>& R_CONNS = {},
			cv::Point2f origin = { -1.0f, -1.0f });
	};

} NS_DLIB_PATTERN_END