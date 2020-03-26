#include "LineSeq.h"
#include "Conn.h"



NS_DLIB_PATTERN_BEGIN namespace _Internal {

	LineSeq::~LineSeq () {

		for (auto p_conn : connPtrs) {
			if (p_conn->p_line1 == this) {
				p_conn->p_line1 = NULL;
				p_conn->idx1 = -1;
			} else {
				p_conn->p_line2 = NULL;
				p_conn->idx2 = -1;
			}
		}

	}



	LineSeq& LineSeq::merge (LineSeq& r_other, bool reverse) {

		if (reverse) {
			for (auto it = r_other.connPtrs.rbegin ();
				it != r_other.connPtrs.rend (); ++it) {
				if ((*it)->p_line1 == &r_other) {
					(*it)->p_line1 = this;
					(*it)->idx1 = (int)connPtrs.size ();
				} else {
					(*it)->p_line2 = this;
					(*it)->idx2 = (int)connPtrs.size ();
				}
				connPtrs.push_back (*it);
			}
			for (auto it = r_other.lps.rbegin ();
				it != r_other.lps.rend (); ++it)
				append (*it, it->type);
		} else {
			for (auto it = r_other.connPtrs.begin ();
				it != r_other.connPtrs.end (); ++it) {
				if ((*it)->p_line1 == &r_other) {
					(*it)->p_line1 = this;
					(*it)->idx1 = (int)connPtrs.size ();
				} else {
					(*it)->p_line2 = this;
					(*it)->idx2 = (int)connPtrs.size ();
				}
				connPtrs.push_back (*it);
			}
			for (auto it = r_other.lps.begin ();
				it != r_other.lps.end (); ++it)
				append (*it, it->type);
		}
		r_other.connPtrs.clear ();
		r_other.lps.clear ();
		return *this;

	}



	void LineSeq::addConnection (Conn* p_conn, bool append) {

		if (append) {
			connPtrs.push_back (p_conn);
			p_conn->idxInLine (this) = (int)connPtrs.size () - 1;
		} else {
			for (auto i = 0; i < connPtrs.size (); ++i)
				connPtrs[i]->idxInLine (this)++;
			connPtrs.push_front (p_conn);
			p_conn->idxInLine (this) = 0;
		}

	}



	void LineSeq::clearInvalidConnections () {

		std::deque<Conn*> validConnPtrs;
		for (auto p_conn : connPtrs) {
			if (p_conn->IS_VALID ()) {
				p_conn->idxInLine (this) = (int)validConnPtrs.size ();
				validConnPtrs.push_back (p_conn);
			}
		}
		connPtrs = validConnPtrs;

	}



	LineSeq* LineSeq::getCrossLine (int i) {
		
		return connPtrs[i]->getCrossLine (this);
	
	}



	LineSeq& LineSeq::reverse () {

		std::reverse (lps.begin (), lps.end ());
		for (auto i = 0; i < lps.size (); ++i)
			std::swap (lps[i].in, lps[i].out);
		std::reverse (connPtrs.begin (), connPtrs.end ());
		for (auto i = 0; i < connPtrs.size (); ++i) {
			auto& conn_idxInLine = connPtrs[i]->idxInLine (this);
			conn_idxInLine = (int)lps.size () - 1 - conn_idxInLine;
		}
		return *this;

	}

} NS_DLIB_PATTERN_END