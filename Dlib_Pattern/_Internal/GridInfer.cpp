#include "GridInfer.h"

using namespace Dlib::CvExt;



NS_DLIB_PATTERN_BEGIN namespace _Internal {

	bool GridInfer::run (cv::Point2f& o_origin) {
		if (!_determineHorizontalOrVertical ())
			return false;
		_updateConns ();
		_updateSeqOfLines ();
		_inferCoords ();
		cv::Point coordOfOrigin;
		if (!_determineOrigin (o_origin, coordOfOrigin))
			return false;
		_updateCoords (coordOfOrigin);
		return true;
	}



	bool GridInfer::_determineHorizontalOrVertical () {

		std::queue<LineSeq*> queue;

		// ѡһ�����С��30�ȵ�����Ϊ��׼ˮƽ�ߣ��Դ����������ߵ�ˮƽ/��ֱ״̬
		for (auto i = 0; i < _r_lines.size (); ++i) {
			auto& r_line = *_r_lines[i];
			cv::Point2f start = r_line[0], end = r_line[-1];
			cv::Point2f v_abs (std::abs (end.x - start.x), std::abs (end.y - start.y));
			if (v_abs.x > v_abs.y&& v_abs.y / v_abs.x <= 0.578f) { // Լ30��
				r_line.type = LineSeq::Horizontal;
				queue.push (&r_line);
				break;
			}
		}

		// ��û�����С��30�ȵ��ߣ���ѡȡ���С��45�ȵ�����Ϊ��׼ˮƽ��
		if (queue.empty ()) {
			for (auto i = 0; i < _r_lines.size (); ++i) {
				auto& r_line = *_r_lines[i];
				cv::Point2f start = r_line[0], end = r_line[-1];
				cv::Point2f v_abs (std::abs (end.x - start.x), std::abs (end.y - start.y));
				if (v_abs.x > v_abs.y) { // Լ30��
					r_line.type = LineSeq::Horizontal;
					queue.push (&r_line);
					break;
				}
			}
			// ��û�����С��45�ȵ��ߣ��򷵻�false
			if (queue.empty ())
				return false;
		}

		// ���ݱ�׼ˮƽ�ߣ��Թ�ȱ����Ƶ�����������ˮƽ/��ֱ״̬
		while (!queue.empty ()) {
			auto p_line = queue.front ();
			queue.pop ();
			for (auto i = 0; i < p_line->connPtrs.size (); ++i) {
				auto p_crossLine = p_line->getCrossLine (i);
				if (p_crossLine->type != LineSeq::Unknown &&
					p_crossLine->type == p_line->type) {
					p_line->type = LineSeq::Invalid;
					break;
				}
			}
			if (p_line->type == LineSeq::Invalid)
				continue;
			for (auto i = 0; i < p_line->connPtrs.size (); ++i) {
				// ����ǰ����Ϊ���ߣ��򽻲�����Ϊ���ߣ���֮ͬ��
				auto p_crossLine = p_line->getCrossLine (i);
				if (p_crossLine->type != LineSeq::Unknown)
					continue;
				p_crossLine->type = p_line->type == LineSeq::Horizontal ?
					LineSeq::Vertical : LineSeq::Horizontal;
				queue.push (p_crossLine);
			}
		}

		// ��֤�Ƿ���ڹ���δ���Ƶ����������������򷵻�false������������������Ӧ��ͨ��
		std::vector<LineSeq*> filteredLines;
		for (auto i = 0; i < _r_lines.size (); ++i) {
			if (_r_lines[i]->type == LineSeq::Unknown ||
				_r_lines[i]->type == LineSeq::Invalid)
				delete _r_lines[i];
			else
				filteredLines.push_back (_r_lines[i]);
		}
		_r_lines = filteredLines;
		for (auto p_line : _r_lines)
			p_line->clearInvalidConnections ();
		std::vector<Conn*> filteredConnPtrs;
		for (auto p_conn : _r_connPtrs) {
			if (p_conn->IS_VALID ())
				filteredConnPtrs.push_back (p_conn);
			else
				delete p_conn;
		}
		_r_connPtrs = filteredConnPtrs;
		return true;

	}



	void GridInfer::_updateConns () {

		// Ϊ�˺������㴦�����½����������ָ��Ĵ洢˳��Ϊp_line1=���ߣ�p_line2=����
		for (auto p_conn : _r_connPtrs) {
			if (p_conn->p_line1->type == LineSeq::Vertical) {
				std::swap (p_conn->p_line1, p_conn->p_line2);
				std::swap (p_conn->idx1, p_conn->idx2);
			}
		}

	}



	void GridInfer::_updateSeqOfLines () {

		for (auto i = 0; i < _r_lines.size (); ++i) {
			if (_r_lines[i]->SIZE () <= 1) continue;
			if (_r_lines[i]->type == LineSeq::Horizontal) {
				if ((*_r_lines[i])[0].x > (*_r_lines[i])[-1].x)
					_r_lines[i]->reverse ();
			} else {
				if ((*_r_lines[i])[0].y > (*_r_lines[i])[-1].y)
					_r_lines[i]->reverse ();
			}
		}

	}



	void GridInfer::_inferCoords () {

		if (_r_lines.size () == 0)
			return;

		// Ѱ�Һ�����ཻ�������ߣ��Դ����ϵĽ����Ϊ��׼��������֮����ĺ�������
		auto p_baseVLine = *std::max_element (_r_lines.begin (), _r_lines.end (),
			[] (const LineSeq* val1, const LineSeq* val2) {
				auto nCrosses1 = val1->type == LineSeq::Horizontal ? 0 : val1->connPtrs.size ();
				auto nCrosses2 = val2->type == LineSeq::Horizontal ? 0 : val2->connPtrs.size ();
				return nCrosses1 < nCrosses2;
			}
		);
		for (auto i = 0; i < p_baseVLine->connPtrs.size (); ++i)
			p_baseVLine->getCrossLine (i)->coord = i;

		if (p_baseVLine->connPtrs.size () == 0)
			return;

		// Ѱ���ݻ����Ϻ�����ཻ���ĺ��ߣ��Դ����ϵĽ����Ϊ��׼��������֮�������������
		auto& p_baseHLine = *(*std::max_element (p_baseVLine->connPtrs.begin (),
			p_baseVLine->connPtrs.end (),
			[&p_baseVLine](const Conn* val1, const Conn* val2) {
				auto nCrosses1 = val1->getCrossLine (p_baseVLine)->connPtrs.size ();
				auto nCrosses2 = val2->getCrossLine (p_baseVLine)->connPtrs.size ();
				return nCrosses1 < nCrosses2;
			}
		))->getCrossLine (p_baseVLine);
		for (auto i = 0; i < p_baseHLine.connPtrs.size (); ++i)
			p_baseHLine.getCrossLine (i)->coord = i;

		// �����Ƶ���������������
		std::queue<LineSeq*> queue;
		for (auto i = 0; i < _r_lines.size (); ++i)
			if (_r_lines[i]->coord != LineSeq::INVALID_COORD)
				queue.push (_r_lines[i]);
		while (!queue.empty ()) {
			auto& r_line = *queue.front ();
			queue.pop ();
			int before = LineSeq::INVALID_COORD;
			int beforeIdx = -1;
			for (auto i = 0; i < r_line.connPtrs.size (); ++i) {
				auto& r_crossLine = *r_line.getCrossLine (i);
				if (r_crossLine.coord == LineSeq::INVALID_COORD)
					continue;
				if (before == LineSeq::INVALID_COORD) {
					// �˽�����֮ǰ�Ľ����߶�û��ȷ�����꣬����ݴ˽����ߵ�����ǰ��
					// �Ƶ�ǰ�� ?   ?   ?   x
					//         <----------
					// �Ƶ���x-3 x-2 x-1  x
					for (int j = i - 1, coord = r_crossLine.coord - 1;
						j >= 0; --j, --coord) {
						auto p_crossLine1 = r_line.getCrossLine (j);
						p_crossLine1->coord = coord;
						queue.push (p_crossLine1);
					}
				} else if (i - beforeIdx == r_crossLine.coord - before) {
					// ��������ߵ��������ڼ����������������β�����ߵ������ƶ��м佻���ߵ�����
					// �Ƶ�ǰ�� x  ?   ?  x+3
					//         -------->
					// �Ƶ��� x x+1 x+2 x+3
					for (int j = beforeIdx + 1, coord = before + 1;
						j < i; ++j, ++coord) {
						auto p_crossLine1 = r_line.getCrossLine (j);
						p_crossLine1->coord = coord;
						queue.push (p_crossLine1);
					}
				}
				before = r_crossLine.coord;
				beforeIdx = i;				
			}
			if (beforeIdx >= 0) {
				// �Ƶ�ĩβ����������
				// �Ƶ�ǰ�� x  ?   ?   ?
				//         -------->
				// �Ƶ��� x x+1 x+2 x+3
				for (int j = beforeIdx + 1, coord = before + 1;
					j < r_line.connPtrs.size (); ++j, ++coord) {
					auto p_crossLine1 = r_line.getCrossLine (j);
					p_crossLine1->coord = coord;
					queue.push (p_crossLine1);
				}
			}
		}
	}



	bool GridInfer::_determineOrigin (cv::Point2f& o_origin, cv::Point& o_coordOfOrigin) {

		cv::Rect coordBounding = _getCoordBounding ();

		// ���������������ͼ���޽����Ľڵ�ֵΪ������
		cv::Mat_<int> crossIndexMap (coordBounding.size ());
		memset (crossIndexMap.data, -1, coordBounding.width * coordBounding.height * sizeof(int));
		for (auto i = 0; i < _r_connPtrs.size (); ++i) {
			if (_r_connPtrs[i]->IS_COORD_VALID ())
				crossIndexMap (_r_connPtrs[i]->COORD () - coordBounding.tl ()) = i;
		}
		
		// ѡȡԭ�㼰����ԭ���Ӧ����������
		float minDistanceToBlob = std::numeric_limits<float>::max ();
		BasicIterator<int> iter (crossIndexMap);
		while (iter.moveNext ()) {
			if (*iter >= 0) continue;
			// ��ѡԭ��Ϊ�����������ͼ�еĿն��ڵ㣨����Χ���н������ڣ�
			//
			//      ****o***o
			//      **o******
			//      *********
			//
			bool isCandidateCenter = ((iter.cursor () + MoveUp).value (true, 0) >= 0) &&
				((iter.cursor () + MoveRight).value (true, 0) >= 0) &&
				((iter.cursor () + MoveDown).value (true, 0) >= 0) &&
				((iter.cursor () + MoveLeft).value (true, 0) >= 0);
			if (!isCandidateCenter) continue;
			auto idxUp = (iter.cursor () + MoveUp).value (true, -1);
			auto idxRight = (iter.cursor () + MoveRight).value (true, -1);
			auto idxDown = (iter.cursor () + MoveDown).value (true, -1);
			auto idxLeft = (iter.cursor () + MoveLeft).value (true, -1);
			Line2D l1, l2;
			float distanceThreshold = 0.0f;
			if (idxUp >= 0 && idxDown >= 0) {
				auto p1 = (*_r_connPtrs[idxUp]->p_line2)[-1];
				auto p2 = (*_r_connPtrs[idxDown]->p_line2)[0];
				l1 = Line2D (p1, p2);
				distanceThreshold = magnitude (_r_connPtrs[idxUp]->p - _r_connPtrs[idxDown]->p);
			} else if (idxUp >= 0) {
				auto& r_lp = (*_r_connPtrs[idxUp]->p_line2)[-1];
				l1 = Line2D (r_lp, r_lp.out);
				distanceThreshold = magnitude (_r_connPtrs[idxUp]->p -
					_r_connPtrs[idxLeft >= 0 ? idxLeft : idxRight]->p) * 1.4f;
			} else if (idxDown >= 0) {
				auto& r_lp = (*_r_connPtrs[idxDown]->p_line2)[0];
				l1 = Line2D (r_lp.in, r_lp);
				distanceThreshold = magnitude (_r_connPtrs[idxDown]->p -
					_r_connPtrs[idxLeft >= 0 ? idxLeft : idxRight]->p) * 1.4f;
			}
			if (idxLeft >= 0 && idxRight >= 0) {
				auto p1 = (*_r_connPtrs[idxLeft]->p_line1)[-1];
				auto p2 = (*_r_connPtrs[idxRight]->p_line1)[0];
				l2 = Line2D (p1, p2);
				distanceThreshold += magnitude (_r_connPtrs[idxLeft]->p - _r_connPtrs[idxRight]->p);
			} else if (idxLeft >= 0) {
				auto& r_lp = (*_r_connPtrs[idxLeft]->p_line1)[-1];
				l1 = Line2D (r_lp, r_lp.out);
				distanceThreshold += magnitude (_r_connPtrs[idxLeft]->p -
					_r_connPtrs[idxUp >= 0 ? idxUp : idxDown]->p) * 1.4f;
			} else if (idxRight >= 0) {
				auto& r_lp = (*_r_connPtrs[idxRight]->p_line1)[0];
				l1 = Line2D (r_lp.in, r_lp);
				distanceThreshold += magnitude (_r_connPtrs[idxRight]->p -
					_r_connPtrs[idxUp >= 0 ? idxUp : idxDown]->p) * 1.4f;
			}
			distanceThreshold /= 4.0f;
			auto intersectPoint = Line2D::Intersect (l1, l2);

			// ����ֵ��Χ�ڼ�¼��������ĵ���Ϊԭ��
			for (auto i = 0; i < _r_blobs.size (); ++i) {
				auto distanceToBlob = magnitude (_r_blobs[i].pt - intersectPoint);
				if (distanceToBlob >= distanceThreshold || distanceToBlob >= minDistanceToBlob)
					continue;
				minDistanceToBlob = distanceToBlob;
				o_origin = _r_blobs[i].pt;
				o_coordOfOrigin = iter.POSITION () + coordBounding.tl ();
			}

		}

		return minDistanceToBlob != std::numeric_limits<float>::max ();

	}



	void GridInfer::_updateCoords (cv::Point coordOfOrigin) {

		cv::Point newCoordOfOrigin (_gridSize.width / 2, _gridSize.height / 2);
		auto offset = newCoordOfOrigin - coordOfOrigin;
		for (auto i = 0; i < _r_lines.size (); ++i)
			if (_r_lines[i]->coord != LineSeq::INVALID_COORD)
				_r_lines[i]->coord += _r_lines[i]->type == LineSeq::Horizontal ?
					offset.y : offset.x;

	}



	cv::Rect GridInfer::_getCoordBounding () {

		auto minX = (*std::min_element (_r_lines.begin (), _r_lines.end (),
			[](const LineSeq* val1, const LineSeq* val2) {
				auto coord1 = val1->type == LineSeq::Horizontal || val1->coord == LineSeq::INVALID_COORD ? 1000 : val1->coord;
				auto coord2 = val2->type == LineSeq::Horizontal || val2->coord == LineSeq::INVALID_COORD ? 1000 : val2->coord;
				return coord1 < coord2;
			}))->coord;
		auto maxX = (*std::max_element (_r_lines.begin (), _r_lines.end (),
			[](const LineSeq* val1, const LineSeq* val2) {
				auto coord1 = val1->type == LineSeq::Horizontal || val1->coord == LineSeq::INVALID_COORD ? -1000 : val1->coord;
				auto coord2 = val2->type == LineSeq::Horizontal || val2->coord == LineSeq::INVALID_COORD ? -1000 : val2->coord;
				return coord1 < coord2;
			}))->coord;
		auto minY = (*std::min_element (_r_lines.begin (), _r_lines.end (),
			[](const LineSeq* val1, const LineSeq* val2) {
				auto coord1 = val1->type == LineSeq::Vertical || val1->coord == LineSeq::INVALID_COORD ? 1000 : val1->coord;
				auto coord2 = val2->type == LineSeq::Vertical || val2->coord == LineSeq::INVALID_COORD ? 1000 : val2->coord;
				return coord1 < coord2;
			}))->coord;
		auto maxY = (*std::max_element (_r_lines.begin (), _r_lines.end (),
			[](const LineSeq* val1, const LineSeq* val2) {
				auto coord1 = val1->type == LineSeq::Vertical || val1->coord == LineSeq::INVALID_COORD ? -1000 : val1->coord;
				auto coord2 = val2->type == LineSeq::Vertical || val2->coord == LineSeq::INVALID_COORD ? -1000 : val2->coord;
				return coord1 < coord2;
			}))->coord;
		return cv::Rect (cv::Point (minX, minY), cv::Point (maxX + 1, maxY + 1));

	}

} NS_DLIB_PATTERN_END