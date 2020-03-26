#pragma once
#include "../global.h"
#include "LinePoint.h"



NS_DLIB_PATTERN_BEGIN namespace _Internal {
	
	class Conn;

	class LineSeq {
	public:
		enum Type { Unknown, Horizontal, Vertical, Invalid };
		const static int INVALID_COORD = -1000;
		Type type;
		int coord;
		std::deque<LinePoint> lps;
		std::deque<Conn*> connPtrs;

		LineSeq () : type (Unknown), coord (INVALID_COORD) { }

		LineSeq (const LinePoint& R_START_LP) :
			lps (1, R_START_LP), type (Unknown), coord (INVALID_COORD) {
			if (R_START_LP.out.width > 0.0f)
				lps.push_back ({ R_START_LP.out, R_START_LP, { } });
		}

		~LineSeq ();

		void append (const Port& R_OUT, LinePoint::Type type = LinePoint::Normal) {
			if (R_OUT.width == 0.0f) return;
			if (SIZE () > 0) {
				auto& r_endLP = lps[lps.size () - 1];
				r_endLP.out = R_OUT;
				lps.push_back ({ R_OUT, r_endLP, { }, type });
			} else
				lps.push_back ({ R_OUT, { }, { }, type });
		}

		void modify (int index, const Port& R_NEW, LinePoint::Type type = LinePoint::Normal) {
			index = _IDX (index);
			lps[index] = R_NEW;
			lps[index].type = type;
			if (index > 0) lps[index - 1].out = R_NEW;
			if (index < lps.size () - 1) lps[index + 1].in = R_NEW;
		}

		void removeGuessPoints () {
			std::deque<LinePoint> filtered;
			for (auto i = 0; i < lps.size (); ++i) {
				if (lps[i].type == LinePoint::Guess) {
					if (i > 0)
						filtered[filtered.size () - 1].out = lps[i].out;
					if (i < lps.size () - 1)
						lps[i + 1].in = lps[i].in;
				} else
					filtered.push_back (lps[i]);
			}
			lps = filtered;
		}

		int SIZE () const { return (int)lps.size (); }

		const LinePoint& operator[] (int index) const { return lps[_IDX (index)]; }

		LinePoint& operator[] (int index) { return lps[_IDX (index)]; }

		LineSeq& merge (LineSeq& r_other, bool reverse = false);

		void addConnection (Conn* p_conn, bool append = true);

		void clearInvalidConnections ();

		LineSeq* getCrossLine (int i);

		LineSeq& reverse ();

		void drawLines (CvExt::Painter& r_painter, const CvExt::Color& R_COLOR,
			float scale = 1.0f, bool darken = false) {
			for (auto j = 0; j < lps.size () - 1; ++j)
				r_painter.line (lps[j] * scale, lps[j + 1] * scale,
					darken ? R_COLOR / 2 : R_COLOR);
		}

		void drawPoints (CvExt::Painter& r_painter, const CvExt::Color& R_COLOR,
			float scale = 1.0f, bool darken = false) {
			for (auto j = 0; j < lps.size (); ++j) {
				if (lps[j].type == LinePoint::Endpoint) {
					CvExt::Color c = j == 0 ? CvExt::Color::Green : CvExt::Color::Red;
					if (darken) c /= 2;
					r_painter.circle (lps[j] * scale, 2.0f * scale, c, -1);
					r_painter.circle (lps[j] * scale, 2.5f * scale, CvExt::Color::Black, 1);
				} else if (lps[j].type != LinePoint::Guess) {
					r_painter.circle (lps[j] * scale, 1.5f * scale, R_COLOR / 4, -1);
					r_painter.circle (lps[j] * scale, .5f * scale,
						darken ? R_COLOR / 2 : R_COLOR, -1);
				} else {
					r_painter.circle (lps[j] * scale, 1.5f * scale, CvExt::Color::Black, -1);
					r_painter.circle (lps[j] * scale, .5f * scale, R_COLOR / 4, -1);
				}
			}
		}

	private:
		LineSeq (const LineSeq&) = delete;
		LineSeq& operator= (const LineSeq&) = delete;

		int _IDX (int i) const { return i < 0 ? (int)SIZE () + i : i; }

	};

} NS_DLIB_PATTERN_END

#include "Conn.h"