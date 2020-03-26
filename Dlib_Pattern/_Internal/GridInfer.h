#pragma once
#include "../global.h"
#include "LineSeq.h"



NS_DLIB_PATTERN_BEGIN namespace _Internal {

	class GridInfer {
	public:
		GridInfer (std::vector<LineSeq*>& r_lines,
			std::vector<Conn*>& r_connPtrs,
			std::vector<cv::KeyPoint>& r_blobs,
			cv::Size gridSize) :
			_r_lines (r_lines),
			_r_connPtrs (r_connPtrs),
			_r_blobs (r_blobs),
			_gridSize (gridSize) { }

		bool run (cv::Point2f& o_origin);

	private:
		std::vector<LineSeq*>& _r_lines;
		std::vector<Conn*>& _r_connPtrs;
		std::vector<cv::KeyPoint>& _r_blobs;
		cv::Size _gridSize;

		bool _determineHorizontalOrVertical ();
		void _updateConns ();
		void _updateSeqOfLines ();
		void _inferCoords ();
		bool _determineOrigin (cv::Point2f& o_origin,
			cv::Point& o_coordOfOrigin);
		void _updateCoords (cv::Point coordOfOrigin);
		cv::Rect _getCoordBounding ();

	};

} NS_DLIB_PATTERN_END