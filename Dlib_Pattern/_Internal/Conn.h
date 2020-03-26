#pragma once
#include "../global.h"
#include "LineSeq.h"



NS_DLIB_PATTERN_BEGIN namespace _Internal {

	class Conn {
	public:
		cv::Point2f p;
		LineSeq* p_line1;
		int idx1;
		LineSeq* p_line2;
		int idx2;

		Conn (cv::Point2f p, LineSeq* p_line1, LineSeq* p_line2) :
			p (p), p_line1 (p_line1), p_line2 (p_line2), idx1 (-1), idx2 (-1) { }

		LineSeq* getCrossLine (LineSeq* p_line) const { return p_line1 == p_line ? p_line2 : p_line1; }

		int& idxInLine (LineSeq* p_line) { return p_line1 == p_line ? idx1 : idx2; }

		int IDX_IN_LINE (LineSeq* p_line) const { return p_line1 == p_line ? idx1 : idx2; }

		bool IS_VALID () const { return idx1 >= 0 && idx2 >= 0; }

		cv::Point COORD () const {
			return IS_VALID () ?
				cv::Point (p_line2->coord, p_line1->coord) :
				cv::Point (LineSeq::INVALID_COORD, LineSeq::INVALID_COORD);
		}

		bool IS_COORD_VALID () const {
			return IS_VALID () && p_line1->coord != LineSeq::INVALID_COORD &&
				p_line2->coord != LineSeq::INVALID_COORD;
		}

	};

} NS_DLIB_PATTERN_END