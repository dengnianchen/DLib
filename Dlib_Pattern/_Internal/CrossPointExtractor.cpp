#include "CrossPointExtractor.h"
#include "Conn.h"

using namespace Dlib::CvExt;



NS_DLIB_PATTERN_BEGIN namespace _Internal {

	CrossPointExtractor::CrossPointExtractor (const Image8& R_PATTERN_IMAGE,
		std::vector<LineSeq*>& r_lines, bool isDebug) :
		CvProcessBase (isDebug), _r_lines (r_lines),
		_lineLabelMap (R_PATTERN_IMAGE.SIZE ()),
		_debugImage (_createDebugImage (R_PATTERN_IMAGE, false)) {

		BasicIterator<cv::Vec4i> iter ({ _lineLabelMap });
		while (iter.moveNext ())
			*iter = cv::Vec4i (-1, -1, -1, -1);

	}



	std::vector<Conn*> CrossPointExtractor::run () {

		return _extractCrossPoint ();
		/*
		for (auto i = 0; i < _r_lines.size (); ++i) {
			auto& r_line = *_r_lines[i];
			for (auto j = 0; j < r_line.SIZE () - 1; ++j) {
				cv::Point p1 = r_line[j];
				cv::Point p2 = r_line[j].out;
				if (std::abs (p2.x - p1.x) > std::abs (p2.y - p1.y))
					_drawLine (p1, p2, i, j, false);
				else
					_drawLine ({ p1.y, p1.x }, { p2.y, p2.x }, i, j, true);
			}
		}
		std::vector<std::map<int, Conn*>> connPtrs (_r_lines.size ());
		std::map<int, Conn*> connectionMap;
		BasicIterator<cv::Vec4i> iter ({ _lineLabelMap });
		while (iter.moveNext ()) {
			if ((*iter)[0] < 0 || (*iter)[2] < 0)
				continue;
			int connectionId = ((*iter)[0] << 16) + (*iter)[2];
			if (connectionMap.find (connectionId) != connectionMap.end ())
				continue;

			auto& r_line1 = *_r_lines[(*iter)[0]];
			auto& r_line2 = *_r_lines[(*iter)[2]];
			auto& r_p0 = r_line1[(*iter)[1]];
			auto& r_p1 = r_line1[(*iter)[1]].out;
			auto& r_q0 = r_line2[(*iter)[3]];
			auto& r_q1 = r_line2[(*iter)[3]].out;
			auto l1 = cv::Point3f (r_p0.x, r_p0.y, 1.0f).cross ({ r_p1.x, r_p1.y, 1.0f });
			auto l2 = cv::Point3f (r_q0.x, r_q0.y, 1.0f).cross ({ r_q1.x, r_q1.y, 1.0f });
			auto cross3 = l1.cross (l2);
			cv::Point2f cross (cross3.x / cross3.z, cross3.y / cross3.z);
			auto p_conn = new Conn (cross, &r_line1, &r_line2);
			connPtrs[(*iter)[0]].insert ({ (*iter)[1], p_conn });
			connPtrs[(*iter)[2]].insert ({ (*iter)[3], p_conn });
			connectionMap.insert ({ connectionId, p_conn });
		}
		std::vector<Conn*> connections;
		for (auto it = connectionMap.begin (); it != connectionMap.end (); ++it)
			connections.push_back (it->second);
		for (auto i = 0; i < _r_lines.size (); ++i) {
			for (auto it = connPtrs[i].begin (); it != connPtrs[i].end (); ++it)
				_r_lines[i]->addConnection (it->second);
		}
		return connections;
		*/
	}



	std::vector<Conn*> CrossPointExtractor::_extractCrossPoint () {

		// 收集所有的Guess点组成LSEP集，并计算线宽均值
		LSEPGroup eps;
		std::map<LineSeq*, std::pair<LSEP*, LSEP*>> epOfLines;
		cv::Vec2f avgLineWidth;
		for (auto i = 0; i < _r_lines.size (); ++i) {
			auto& r_lineSeq = *_r_lines[i];
			for (auto& r_lp : r_lineSeq.lps) {
				if (r_lp.type == LinePoint::Guess)
					continue;
				avgLineWidth += cv::Vec2f (r_lp.width, 1.0f);
			}
			if (r_lineSeq[0].type == LinePoint::Guess) {
				eps.push_back (new LSEP { r_lineSeq, true });
				epOfLines[&r_lineSeq].first = eps.back ();
			}
			if (r_lineSeq[-1].type == LinePoint::Guess) {
				eps.push_back (new LSEP { r_lineSeq, false });
				epOfLines[&r_lineSeq].second = eps.back ();
			}
		}
		avgLineWidth /= avgLineWidth[1];

		// 对LSEP集按距离分组
		std::vector<LSEPGroup> epGroups;
		_groupLineSeqEPs (eps, epGroups, { avgLineWidth[0] * 3, avgLineWidth[0] * 3 });

		// 移除所有Guess点
		_removeGuessPoints ();

		// 从分组中计算线条联结和提取交叉点
		std::vector<Conn*> conns;
		_extractCrossPointByGroup (epGroups, conns);

		Image debugImage;
		if (IS_DEBUG ()) {
			debugImage = Image (_debugImage.FORMAT ());
			cv::resize (_debugImage, debugImage, _debugImage.SIZE () * 2, 0, 0, cv::INTER_NEAREST);
			Painter painter (debugImage);
			for (auto& r_group : epGroups) {
				for (auto p_ep : r_group) {
					if (p_ep->linkTarget)
						painter.line (**p_ep * 2.0f, **p_ep->linkTarget * 2.0f, Color::Red, 2);
				}
			}
			for (auto p_line : _r_lines) {
				p_line->drawLines (painter, Color::BGR (150, 0, 50), 2.0f, true);
				p_line->drawPoints (painter, Color::BGR (150, 0, 50), 2.0f, true);
			}
			for (auto& r_group : epGroups) {
				auto c = Color::RandomHSV ({ 0.0f, 0.6f, 0.5f }, { 360.0f, 1.0f, 1.0f });
				for (auto p_ep : r_group)
					painter.circle (**p_ep * 2.0f, 2.0f, c (EColor_BGR) * 255, -1);
			}
			for (auto p_conn : conns)
				painter.marker (p_conn->p * 2.0f, cv::MARKER_CROSS, 7, Color::BGR (0, 0, 50));
		}

		std::set<LineSeq*> checkedLines;
		std::vector<LineSeq*> linkedLines;
		for (auto p_line : _r_lines) {
			if (checkedLines.find (p_line) != checkedLines.end ()) continue;

			// 判断条件：有且仅有一个连接到其他线条的端点
			auto epOfLine = epOfLines[p_line];
			int nLinkTargets = epOfLine.first && epOfLine.first->linkTarget;
			nLinkTargets += epOfLine.second && epOfLine.second->linkTarget;
			if (nLinkTargets == 2)
				continue;

			// 没有连接到其他线条的线条直接加入目标集合中
			if (nLinkTargets == 0) {
				linkedLines.push_back (p_line);
				checkedLines.insert (p_line);
				if (IS_DEBUG ()) {
					Image debugImage1 = debugImage.CLONE ();
					Painter painter (debugImage1);
					for (auto p_linkedLine : linkedLines) {
						p_linkedLine->drawLines (painter, Color::BGR (0, 150, 255), 2.0f);
						p_linkedLine->drawPoints (painter, Color::BGR (0, 150, 255), 2.0f);
					}
					_showDebugWindow ("CrossPointExtractor - Debug", debugImage1);
				}
				continue;
			}

			// 创建连接的线条
			linkedLines.push_back (new LineSeq);
			auto p_linkedLine = linkedLines.back ();

			// 循环连接线条
			LineSeq* p_lineToLink = p_line;
			LSEP* p_nextEp = epOfLine.first && epOfLine.first->linkTarget ?
				epOfLine.second : epOfLine.first;
			while (p_lineToLink && checkedLines.find (p_lineToLink) == checkedLines.end ()) {
				bool reverse;
				epOfLine = epOfLines[p_lineToLink];
				if (epOfLine.first == p_nextEp) {
					p_nextEp = epOfLine.second ? epOfLine.second->linkTarget : NULL;
					reverse = false;
				} else {
					p_nextEp = epOfLine.first ? epOfLine.first->linkTarget : NULL;
					reverse = true;
				}
				p_linkedLine->merge (*p_lineToLink, reverse);
				checkedLines.insert (p_lineToLink);
				delete p_lineToLink;
				p_lineToLink = p_nextEp ? &p_nextEp->r_lineSeq : NULL;

				if (IS_DEBUG ()) {
					Image debugImage1 = debugImage.CLONE ();
					Painter painter (debugImage1);
					for (auto p_linkedLine : linkedLines) {
						p_linkedLine->drawLines (painter, Color::BGR (0, 150, 255), 2.0f);
						p_linkedLine->drawPoints (painter, Color::BGR (0, 150, 255), 2.0f);
					}
					_showDebugWindow ("CrossPointExtractor - Debug", debugImage1);
				}
			}
		}

		for (auto p_ep : eps) delete p_ep;
		_r_lines = linkedLines;
		return conns;

	}



	void CrossPointExtractor::_groupLineSeqEPs (const LSEPGroup& R_EPS,
		std::vector<LSEPGroup>& o_epGroups, cv::Size2f maxGroupSize) {
		
		std::vector<cv::Rect2f> groupBoundings;

#ifdef DLIB_PATTERN_DYNAMIC_DEBUG
		std::vector<Color> debugGroupColor;
		Image debugImage;
		if (IS_DEBUG ()) {
			debugImage = Image (_debugImage.FORMAT ());
			cv::resize (_debugImage, debugImage, _debugImage.SIZE () * 2, 0, 0, cv::INTER_NEAREST);
			Painter painter (debugImage);
			painter.rectangle (cv::Rect (cv::Point (10, 10), maxGroupSize * 2.0f), Color::Yellow);
			for (auto p_line : _r_lines) {
				p_line->drawLines (painter, Color::BGR (150, 0, 50), 2.0f);
				p_line->drawPoints (painter, Color::BGR (150, 0, 50), 2.0f);
			}
		}
#endif

		for (auto P_EP : R_EPS) {
			bool addToGroup = false;
			auto elp = **P_EP;
			for (auto& r_group : o_epGroups) {
				auto& r_bounding = groupBoundings[&r_group - &o_epGroups[0]];
				if (elp.x - r_bounding.x < -maxGroupSize.width ||
					elp.y - r_bounding.y < -maxGroupSize.height ||
					elp.x - r_bounding.x >= r_bounding.width + maxGroupSize.width ||
					elp.y - r_bounding.y >= r_bounding.height + maxGroupSize.height)
					continue;
				for (auto p_epInGroup : r_group) {
					if (&P_EP->r_lineSeq == &p_epInGroup->r_lineSeq)
						continue;
					auto v = **p_epInGroup - elp;
					if (std::abs (v.x) > maxGroupSize.width &&
						std::abs (v.y) > maxGroupSize.height)
						continue;
					r_group.push_back (P_EP);
					r_bounding = cv::Rect2f (
						cv::Point2f (std::min (r_bounding.x, elp.x),
							std::min (r_bounding.y, elp.y)),
						cv::Point2f (std::max (r_bounding.br ().x, elp.x + 0.1f),
							std::max (r_bounding.br ().y, elp.y + 0.1f))
					);
					addToGroup = true;
#ifdef DLIB_PATTERN_DYNAMIC_DEBUG
					if (IS_DEBUG ()) {
						auto debugImage1 = debugImage.CLONE ();
						Painter painter (debugImage1);
						for (auto& r_group : o_epGroups) {
							auto c = debugGroupColor[&r_group - &o_epGroups[0]];
							for (auto p_ep : r_group)
								painter.circle (**p_ep * 2.0f, 2.0f, c (EColor_BGR) * 255, -1);
						}
						for (auto i = 0; i < groupBoundings.size (); ++i)
							painter.rectangle (groupBoundings[i].tl () * 2.0f,
								groupBoundings[i].br () * 2.0f, Color::BGR (0, 50, 200));
						_showDebugWindow ("CrossPointExtractor - Debug", debugImage1);
					}
#endif
					break;
				}
				if (addToGroup) break;
			}
			if (!addToGroup) {
				o_epGroups.push_back ({ P_EP });
				groupBoundings.push_back ({ elp, cv::Size2f (0.1f, 0.1f) });
#ifdef DLIB_PATTERN_DYNAMIC_DEBUG
				if (IS_DEBUG ())
					debugGroupColor.push_back (Color::RandomHSV ({ 0.0f, 0.6f, 0.5f }, { 360.0f, 1.0f, 1.0f }));
#endif
			}
		}

		/*
		for (auto gid = 0; gid < o_epGroups.size (); ++gid) {
			auto& r_group = o_epGroups[gid];
			std::vector<int> counts (r_group.size (), 0);
			for (auto pid0 = 1; pid0 < r_group.size (); ++pid0)
			for (auto pid1 = pid0 + 1; pid1 < r_group.size (); ++pid1) {
				auto v = **r_group[pid0] - **r_group[pid1];
				if (std::abs (v.x) <= maxGroupSize.width &&
					std::abs (v.y) <= maxGroupSize.height)
					counts[pid0]++, counts[pid1]++;
			}
			for (auto pid0 = 1; pid0 < r_group.size (); ++pid0) {
				if (counts[pid0] > counts.size () / 2) continue;

				auto p_epToMove = r_group[pid0];
				r_group.erase (r_group.begin () + pid0--);

				bool addToGroup = false;
				for (auto gid1 = 0; gid1 < o_epGroups.size (); ++gid1) {
					if (gid1 < gid) {
						if (std::all_of (o_epGroups[gid1].begin (), o_epGroups[gid1].end (),
							[p_epToMove, maxGroupSize](LSEP* p_ep) {
								auto v = **p_epToMove - **p_ep;
								return std::abs (v.x) <= maxGroupSize.width &&
									std::abs (v.y) <= maxGroupSize.height;
							}
						)) {
							o_epGroups[gid1].push_back (p_epToMove);
							addToGroup = true;
							break;
						}
					} else if (gid1 > gid) {
						auto v = **o_epGroups[gid1][0] - **p_epToMove;
						if (std::abs (v.x) <= maxGroupSize.width &&
							std::abs (v.y) <= maxGroupSize.height) {
							o_epGroups[gid1].push_back (p_epToMove);
							addToGroup = true;
							break;
						}
					}
				}
				if (!addToGroup)
					o_epGroups.push_back ({ p_epToMove });
			} // for r_group -> p_epToMove
		} // for o_epGroups -> r_group
		*/
		
		if (IS_DEBUG ()) {
			Image debugImage (_debugImage.FORMAT ());
			cv::resize (_debugImage, debugImage, _debugImage.SIZE () * 2, 0, 0, cv::INTER_NEAREST);
			Painter painter (debugImage);
			painter.rectangle (cv::Rect (cv::Point (10, 10), maxGroupSize * 2.0f), Color::Yellow);
			for (auto p_line : _r_lines) {
				p_line->drawLines (painter, Color::BGR (150, 0, 50), 2.0f);
				p_line->drawPoints (painter, Color::BGR (150, 0, 50), 2.0f);
			}
			for (auto& r_group : o_epGroups) {
				auto c = Color::RandomHSV ({ 0.0f, 0.6f, 0.5f }, { 360.0f, 1.0f, 1.0f });
				for (auto p_ep : r_group)
					painter.circle (**p_ep * 2.0f, 2.0f, c(EColor_BGR) * 255, -1);
			}
			for (auto i = 0; i < groupBoundings.size (); ++i)
				painter.rectangle (groupBoundings[i].tl () * 2.0f,
					groupBoundings[i].br () * 2.0f, Color::BGR (0, 50, 200));
			_showDebugWindow ("CrossPointExtractor - Debug", debugImage);
		}

	}



	void CrossPointExtractor::_removeGuessPoints () {

		for (auto p_lineSeq : _r_lines) {
			if ((*p_lineSeq)[0].type == LinePoint::Guess)
				p_lineSeq->lps.pop_front ();
			if ((*p_lineSeq)[-1].type == LinePoint::Guess)
				p_lineSeq->lps.pop_back ();
		}

	}



	void CrossPointExtractor::_extractCrossPointByGroup (
		const std::vector<LSEPGroup>& R_EP_GROUPS, std::vector<Conn*>& o_conns) {

		for (auto& R_GROUP : R_EP_GROUPS) {
			switch (R_GROUP.size ()) {
			case 2: {
				auto p_ep0 = R_GROUP[0]; auto l0 = p_ep0->GET_LINE ();
				auto p_ep1 = R_GROUP[1]; auto l1 = p_ep1->GET_LINE ();
				// 若端点方向相反（夹角在180度±30度之间）则连接端点，否则创建交叉点
				if (l0.V0 ().dot (l1.V0 ()) < -0.86f) {
					p_ep0->linkTarget = p_ep1;
					p_ep1->linkTarget = p_ep0;
				} else {
					auto p_newConn = new Conn (Line2D::Intersect (l0, l1),
						&p_ep0->r_lineSeq, &p_ep1->r_lineSeq);
					o_conns.push_back (p_newConn);
					p_ep0->r_lineSeq.addConnection (p_newConn, !p_ep0->isHead);
					p_ep1->r_lineSeq.addConnection (p_newConn, !p_ep1->isHead);
				}
			} break;
			case 3: {
				auto p_ep0 = R_GROUP[0]; auto l0 = p_ep0->GET_LINE ();
				auto p_ep1 = R_GROUP[1]; auto l1 = p_ep1->GET_LINE ();
				auto p_ep2 = R_GROUP[2]; auto l2 = p_ep2->GET_LINE ();
				// TODO Debug
				auto p0 = **p_ep0;
				auto p1 = **p_ep1;
				auto p2 = **p_ep2;
				auto dot0 = l0.V0 ().dot (l1.V0 ());
				auto dot1 = l1.V0 ().dot (l2.V0 ());
				auto dot2 = l2.V0 ().dot (l0.V0 ());
				if (dot0 < dot2) {
					std::swap (p_ep1, p_ep2);
					std::swap (l1, l2);
				} else if (dot1 < dot2) {
					std::swap (p_ep0, p_ep1);
					std::swap (l0, l1);
				}
				
				// 连接：0-2
				p_ep0->linkTarget = p_ep2;
				p_ep2->linkTarget = p_ep0;

				// 创建交叉点
				Line2D l02 (**p_ep0, **p_ep2);
				auto p_newConn = new Conn (Line2D::Intersect (l02, l1),
					&p_ep0->r_lineSeq, &p_ep1->r_lineSeq);
				o_conns.push_back (p_newConn);
				p_ep0->r_lineSeq.addConnection (p_newConn, !p_ep0->isHead);
				p_ep1->r_lineSeq.addConnection (p_newConn, !p_ep1->isHead);
			} break;
			case 4: {
				auto p_ep0 = R_GROUP[0];
				auto p_ep1 = R_GROUP[1];
				auto p_ep2 = R_GROUP[2];
				auto p_ep3 = R_GROUP[3];
				auto center = (**p_ep0 + **p_ep1 + **p_ep2 + **p_ep3) / 4.0f;
				auto v0 = normalize (**p_ep0 - center);
				auto v1 = normalize (**p_ep1 - center);
				auto v2 = normalize (**p_ep2 - center);
				auto v3 = normalize (**p_ep3 - center);
				if (v0.dot (v1) < v0.dot (v2))
					std::swap (p_ep1, p_ep2), std::swap (v1, v2);
				if (v0.dot (v3) < v0.dot (v2))
					std::swap (p_ep3, p_ep2), std::swap (v3, v2);

				// 连接：0-2，1-3
				p_ep0->linkTarget = p_ep2;
				p_ep1->linkTarget = p_ep3;
				p_ep2->linkTarget = p_ep0;
				p_ep3->linkTarget = p_ep1;

				// 创建交叉点
				Line2D l02 (**p_ep0, **p_ep2);
				Line2D l13 (**p_ep1, **p_ep3);
				auto p_newConn = new Conn (Line2D::Intersect (l02, l13),
					&p_ep0->r_lineSeq, &p_ep1->r_lineSeq);
				o_conns.push_back (p_newConn);
				p_ep0->r_lineSeq.addConnection (p_newConn, !p_ep0->isHead);
				p_ep1->r_lineSeq.addConnection (p_newConn, !p_ep1->isHead);
			} break;
			}
		}

		if (IS_DEBUG ()) {
			Image debugImage (_debugImage.FORMAT ());
			cv::resize (_debugImage, debugImage, _debugImage.SIZE () * 2, 0, 0, cv::INTER_NEAREST);
			Painter painter (debugImage);
			for (auto& r_group : R_EP_GROUPS) {
				for (auto p_ep : r_group) {
					if (p_ep->linkTarget)
						painter.line (**p_ep * 2.0f, **p_ep->linkTarget * 2.0f, Color::Red, 2);
				}
			}
			for (auto p_line : _r_lines) {
				p_line->drawLines (painter, Color::BGR (150, 0, 50), 2.0f);
				p_line->drawPoints (painter, Color::BGR (150, 0, 50), 2.0f);
			}
			for (auto& r_group : R_EP_GROUPS) {
				auto c = Color::RandomHSV ({ 0.0f, 0.6f, 0.5f }, { 360.0f, 1.0f, 1.0f });
				for (auto p_ep : r_group)
					painter.circle (**p_ep * 2.0f, 2.0f, c (EColor_BGR) * 255, -1);
			}
			for (auto p_conn : o_conns)
				painter.marker (p_conn->p * 2.0f, cv::MARKER_CROSS, 7, Color::BGR (0, 0, 50));
			_showDebugWindow ("CrossPointExtractor - Debug", debugImage);
		}
	}



	void CrossPointExtractor::_drawLine (cv::Point p1, cv::Point p2,
		int lineId, int lpId, bool transpose) {

		if (p2.x < p1.x) std::swap (p1, p2);
		cv::Point2f v = p2 - p1;
		v /= v.x;
		for (int x = p1.x; x < p2.x; ++x) {
			Cursor<cv::Vec4i> p (_lineLabelMap,
				{ x, cvRound (p1.y + v.y * (x - p1.x)) }, transpose);
			_fillPixel (p, lineId, lpId);
			if (x < p2.x - 1) {
				cv::Point pNext (x + 1, cvRound (p1.y + v.y * (x + 1 - p1.x)));
				if (pNext.y < p.POSITION ().y)
					_fillPixel (p + MoveDown, lineId, lpId);
				if (pNext.y > p.POSITION ().y)
					_fillPixel (p + MoveUp, lineId, lpId);
			}
		}

	}


} NS_DLIB_PATTERN_END