#pragma once
#include "../global.h"
#include "LineSeq.h"



NS_DLIB_PATTERN_BEGIN namespace _Internal {

	class CrossPointExtractor : public CvExt::CvProcessBase {
	public:
		CrossPointExtractor (const CvExt::Image8& R_PATTERN_IMAGE,
			std::vector<LineSeq*>& r_lines, bool isDebug = false);

		std::vector<Conn*> run ();

	private:
		class LSEP {
		public:
			LineSeq& r_lineSeq;
			bool isHead;
			LSEP* linkTarget;

			LSEP (LineSeq& r_lineSeq, bool isHead) :
				r_lineSeq (r_lineSeq), isHead (isHead), linkTarget (NULL) { }

			LinePoint* operator-> () { return isHead ? &r_lineSeq[0] : &r_lineSeq[-1]; }
			const LinePoint* operator-> () const { return isHead ? &r_lineSeq[0] : &r_lineSeq[-1]; }

			LinePoint& operator* () { return isHead ? r_lineSeq[0] : r_lineSeq[-1]; }
			const LinePoint& operator* () const { return isHead ? r_lineSeq[0] : r_lineSeq[-1]; }

			CvExt::Line2D GET_LINE () const {
				return isHead ?
					((*this)->HAS_OUT () ? -(*this)->LINE_OUT () : -(*this)->LINE_IN ()) :
					((*this)->HAS_IN () ? (*this)->LINE_IN () : (*this)->LINE_OUT ());
			}

		};

		typedef std::vector<LSEP*> LSEPGroup;
		cv::Mat_<cv::Vec4i> _lineLabelMap;
		std::vector<LineSeq*>& _r_lines;
		CvExt::Image _debugImage;

		std::vector<Conn*> _extractCrossPoint ();

		void _groupLineSeqEPs (const LSEPGroup& R_EPS,
			std::vector<LSEPGroup>& o_epGroups,
			cv::Size2f maxGroupSize);

		void _removeGuessPoints ();

		void _extractCrossPointByGroup (const std::vector<LSEPGroup>& R_EP_GROUPS,
			std::vector<Conn*>& o_conns);

		void _drawLine (cv::Point p1, cv::Point p2,
			int lineId, int lpId, bool transpose);

		void _fillPixel (const CvExt::Cursor<cv::Vec4i>& R_POINT,
			int lineId, int lpId) {
			if ((*R_POINT)[0] == lineId || (*R_POINT)[2] == lineId) return;
			if ((*R_POINT)[0] < 0) (*R_POINT)[0] = lineId, (*R_POINT)[1] = lpId;
			else (*R_POINT)[2] = lineId, (*R_POINT)[3] = lpId;
		}

		void _afterShowDebugWindow () override { cv::waitKey (); }

	};

} NS_DLIB_PATTERN_END