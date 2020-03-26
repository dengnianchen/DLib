#include "LineExtractor.h"
#include "SegInfo.h"

using namespace Dlib::CvExt;



NS_DLIB_PATTERN_BEGIN namespace _Internal {

	bool LineExtractor::run (std::vector<LineSeq*>& o_lines) {

		_extractLines (o_lines);
		return true;

		//std::vector<LinePoint> endpoints;
		//_extractEndPoints (endpoints);
		//_extractLines (endpoints, o_lines);
		//_postprocessLines (o_lines);
		//return true;

	}



	void LineExtractor::_extractLines (std::vector<LineSeq*>& o_lines) {

		// 计算每个像素的LinePoint类型和特征信息
		cv::Mat1b lpTypeMap;
		// feature = [ in.width, in.x, in.y, out.width, out.x, out.y, weight ]
		cv::Mat_<cv::Vec<float, 7>> lpAccFeatureMap;
		_extractLinePoints (3, lpTypeMap, lpAccFeatureMap);
		_extractLinePoints (5, lpTypeMap, lpAccFeatureMap);
		_extractLinePoints (7, lpTypeMap, lpAccFeatureMap);

		// 计算连通域图
		cv::Mat_<ushort> ccLabelMap;
		cv::Mat_<int> states;
		cv::Mat_<double> centroids;
		auto nLabels = cv::connectedComponentsWithStats (lpTypeMap, ccLabelMap,
			states, centroids, 8, CV_16U);

		// 对每个连通域提取线条
		for (auto label = 1; label < nLabels; ++label) {
			if (states (label, cv::CC_STAT_AREA) < 4)
				continue;

			cv::Point2f centerPosition ((float)centroids (label, 0), (float)centroids (label, 1));

			auto lpCenter = _feature2Lp (centerPosition, lpAccFeatureMap);
			auto lpCenterType = (LinePoint::Type)lpTypeMap (centerPosition);
			if (!lpCenter.out.IS_VALID ())
				continue;

			// Forward
			bool guess;
			LinePoint lpforwardStart (lpCenter, {},
				_calculateOut (lpCenter, lpCenter.out, ccLabelMap, label, guess), lpCenterType);
			auto p_forwardSeq = new LineSeq (lpforwardStart);
			while (true) {
				auto& r_lastLp = (*p_forwardSeq)[-1];
				r_lastLp.type = guess ? LinePoint::Guess : (LinePoint::Type)lpTypeMap (r_lastLp);
				if (r_lastLp.type == LinePoint::Guess) break;
				auto rawLp = _feature2Lp (r_lastLp, lpAccFeatureMap);
				if ((rawLp.out - rawLp).dot (r_lastLp - r_lastLp.in) < 0) {
					if (r_lastLp.type == LinePoint::Endpoint) break;
					std::swap (rawLp.out, rawLp.in);
				}
				r_lastLp.type = LinePoint::Normal;
				auto out = _calculateOut (rawLp, rawLp.out, ccLabelMap, label, guess);
				if (!out.IS_VALID () || out == r_lastLp) break;
				p_forwardSeq->append (out);
			}

			if (lpCenter.in.IS_VALID ()) {
				// Backward
				LinePoint lpBackwardStart (lpCenter, {},
					_calculateOut (lpCenter, lpCenter.in, ccLabelMap, label, guess), lpCenterType);
				auto p_backwardSeq = new LineSeq (lpBackwardStart);
				while (true) {
					auto& r_lastLp = (*p_backwardSeq)[-1];
					r_lastLp.type = guess ? LinePoint::Guess : (LinePoint::Type)lpTypeMap (r_lastLp);
					if (r_lastLp.type == LinePoint::Guess) break;
					auto rawLp = _feature2Lp (r_lastLp, lpAccFeatureMap);
					if ((rawLp.out - rawLp).dot (r_lastLp - r_lastLp.in) < 0) {
						if (r_lastLp.type == LinePoint::Endpoint) break;
						std::swap (rawLp.out, rawLp.in);
					}
					r_lastLp.type = LinePoint::Normal;
					auto out = _calculateOut (rawLp, rawLp.out, ccLabelMap, label, guess);
					if (!out.IS_VALID () || out == r_lastLp) break;
					p_backwardSeq->append (out);
				}

				// merge
				p_backwardSeq->lps.pop_front ();
				p_forwardSeq->reverse ().merge (*p_backwardSeq);
				delete p_backwardSeq;
			}
			if (p_forwardSeq->SIZE () <= 1)
				delete p_forwardSeq;
			else
				o_lines.push_back (p_forwardSeq);
		}

		if (IS_DEBUG ()) {
			// 绘制类型图
			BasicIterator<uchar> it_type (lpTypeMap);
			while (it_type.moveNext ()) {
				if (!*it_type) continue;
				auto& r_accFeature = it_type.valueRef (lpAccFeatureMap);
				auto alpha = std::min (r_accFeature[6] / 3, 1.0f);
				cv::Vec3b debugColor = *it_type == LinePoint::Normal ?
					cv::Vec3b (0, 150, 0) :
					cv::Vec3b (0, 0, 255);
				auto& r_pixel = it_type.valueRef<cv::Vec3b> (_debugImage);
				r_pixel = r_pixel * (1.0f - alpha) + debugColor * alpha;
			}

			// 绘制连通域图
			Image debugLabelImage (_debugImage.FORMAT (), _debugImage.SIZE ());
			Painter painter (debugLabelImage);
			painter.fill (Color::Black);
			std::vector<cv::Vec3b> debugLabelColor (nLabels);
			for (auto i = 0; i < nLabels; ++i) {
				debugLabelColor[i] = {
					(uchar)(std::rand () / (float)RAND_MAX * 155 + 50),
					(uchar)(std::rand () / (float)RAND_MAX * 155 + 50),
					(uchar)(std::rand () / (float)RAND_MAX * 155 + 50)
				};
			}
			BasicIterator<ushort> it_label (ccLabelMap);
			while (it_label.moveNext ()) {
				if (*it_label == 0 || states (*it_label, cv::CC_STAT_AREA) < 4) continue;
				it_label.valueRef<cv::Vec3b> (debugLabelImage) = debugLabelColor[*it_label];
			}
			for (int i = 1, l = 0; i < nLabels; ++i) {
				if (states (i, cv::CC_STAT_AREA) < 4) continue;
				cv::Point2f centerPosition ((float)centroids (i, 0), (float)centroids (i, 1));
				debugLabelImage.set (centerPosition, Color::White);
				std::ostringstream sout; sout << l;
				painter.text (sout.str (), centerPosition + cv::Point2f (-3.0f, 12.0f),
					cv::FONT_HERSHEY_PLAIN, 0.75, Color::Red);
				++l;
			}

			// 绘制提取的线条
			Image debugLineImage (_debugImage.FORMAT (), _debugImage.SIZE () * 2);
			cv::resize (_debugImage, debugLineImage, debugLineImage.SIZE (), 0, 0, cv::INTER_NEAREST);
			Painter painter1 (debugLineImage);
			for (auto i = 0; i < o_lines.size (); ++i) {
				o_lines[i]->drawLines (painter1, Color::BGR (255, 92, 64), 2.0f);
				o_lines[i]->drawPoints (painter1, Color::BGR (255, 92, 64), 2.0f);
			}

			_showDebugWindow ("LineExtractor - Debug", { debugLineImage,
				Image::concat ({ _debugImage, debugLabelImage }, false) });
		}

	}



	void LineExtractor::_extractLinePoints (int radius, cv::Mat1b& io_typeMap,
		cv::Mat_<cv::Vec<float, 7>>& io_accFeatureMap) {

		// Feature: type(Endpoint/T-Endpoint), lineWidth, outSeg_centerX, outSeg_centerY
		if (io_typeMap.empty ()) {
			io_typeMap = cv::Mat1b (_R_BINARY_IMAGE.SIZE (), LinePoint::Unknown);
			io_accFeatureMap = cv::Mat_<cv::Vec<float, 7>> (_R_BINARY_IMAGE.SIZE (), { });
		}

		BasicIterator<uchar> it_binary (_R_BINARY_IMAGE);
		while (it_binary.moveNext ()) {
			if (*it_binary == 0) continue;
			// TODO Debug
			auto p = it_binary.POSITION ();
			// 遍历的像素若不在图像边缘，则不需要进行边界检查从而可以得到性能优化
			bool checkBoundary = it_binary.POSITION ().x < radius ||
				it_binary.POSITION ().y < radius ||
				_R_BINARY_IMAGE.WIDTH () - it_binary.POSITION ().x <= radius ||
				_R_BINARY_IMAGE.HEIGHT () - it_binary.POSITION ().y <= radius;

			SegInfo segInfo (it_binary.cursor (), radius, 1, checkBoundary);
			if (segInfo.segLengths.size () == 2) {
				// 端点：分段数为2（黑、白各1）且白色分段长度小于方形总长的1/4
				int whiteSegIndex = segInfo.segStarts[0].value (checkBoundary) > 0 ? 0 : 1;
				if (segInfo.segLengths[whiteSegIndex] < radius * 2) {
					_fuseEndPointInfo (io_typeMap, io_accFeatureMap, it_binary,
						LinePoint::Endpoint, {}, segInfo.SEG_TO_PORT (whiteSegIndex));
				}
			} else if (segInfo.segLengths.size () == 4) {
				// 线上点：分段数为4（黑、白各2）、每段白色分段长度小于方形总长的1/4、
				// 白色分段长度差值小于radius、夹角大于150度
				bool firstSegIsWhite = segInfo.segStarts[0].value (checkBoundary) > 0;
				int inSegIdx = !firstSegIsWhite;
				int outSegIdx = inSegIdx + 2;
				bool isLinePoint = (segInfo.segLengths[inSegIdx] < radius * 2 &&
					segInfo.segLengths[outSegIdx] < radius * 2 &&
					std::abs (segInfo.segLengths[inSegIdx] - segInfo.segLengths[outSegIdx]) < radius);
				if (isLinePoint) {
					auto in = segInfo.SEG_TO_PORT (inSegIdx);
					auto out = segInfo.SEG_TO_PORT (outSegIdx);
					auto vin = normalize ((cv::Point2f)it_binary.POSITION () - in);
					auto vout = normalize (out - (cv::Point2f)it_binary.POSITION ());
					if (vin.dot (vout) > 0.86f) // 夹角阈值：约±30度
						_fuseEndPointInfo (io_typeMap, io_accFeatureMap,
							it_binary, LinePoint::Normal, in, out);
				}
			}
		}
	}



	/*
	void LineExtractor::_extractEndPoints (std::vector<LinePoint>& o_endPoints) {

		// basic = [ type, weight ]
		cv::Mat1b rawEpTypeMap;
		// feature = [ width, out.x, out.y, weight ]
		cv::Mat4f rawEpAccFeatureMap;
		_extractEndPoints1 (3, rawEpTypeMap, rawEpAccFeatureMap);
		_extractEndPoints1 (5, rawEpTypeMap, rawEpAccFeatureMap);
		_extractEndPoints1 (7, rawEpTypeMap, rawEpAccFeatureMap);

		cv::Mat_<ushort> labelMap;
		cv::Mat_<int> states;
		cv::Mat_<double> centroids;
		auto nLabels = cv::connectedComponentsWithStats (rawEpTypeMap, labelMap,
			states, centroids, 4, CV_16U);

		std::vector<uchar> typeOfLabels (nLabels, LinePoint::Unknown);
		std::vector<cv::Vec4f> accFeatureOfLabels (nLabels);
		std::vector<cv::Point3f> centerOfLabels (nLabels);
		BasicIterator<ushort> it_label (labelMap);
		while (it_label.moveNext ()) {
			if (!*it_label || states (*it_label, cv::CC_STAT_AREA) < 4)
				continue;
			auto& r_type = it_label.valueRef (rawEpTypeMap);
			auto& r_accFeature = it_label.valueRef (rawEpAccFeatureMap);
			typeOfLabels[*it_label] = std::max (typeOfLabels[*it_label], r_type);
			accFeatureOfLabels[*it_label] += r_accFeature;
			centerOfLabels[*it_label] += cv::Point3f (
			(float)it_label.POSITION ().x * r_type,
				(float)it_label.POSITION ().y * r_type, r_type);
			if (IS_DEBUG ()) {
				auto alpha = std::min (r_accFeature[3] / 4, 1.0f);
				cv::Vec3b debugColor = r_type == LinePoint::Endpoint ?
					cv::Vec3b (0, 0, 255) : cv::Vec3b (255, 0, 0);
				auto& r_pixel = it_label.valueRef<cv::Vec3b> (_debugImage);
				r_pixel = r_pixel * (1.0f - alpha) + debugColor * alpha;
			}
		}

		o_endPoints.clear ();
		// 先填入T形端点，后填入端点，与T形端点距离过近的端点会被剔除
		for (auto i = 1; i < nLabels; ++i) {
			if (states (i, cv::CC_STAT_AREA) < 4 || typeOfLabels[i] != LinePoint::TEnd)
				continue;

			auto feature = accFeatureOfLabels[i] / accFeatureOfLabels[i][3];
			cv::Point2f center (centerOfLabels[i].x / centerOfLabels[i].z, centerOfLabels[i].y / centerOfLabels[i].z);
			cv::Point2f out (feature[1], feature[2]);
			// center后移半个线宽
			center += normalize (center - out) * feature[0] / 2;
			o_endPoints.push_back ({ { center, feature[0] }, { }, { out, feature[0] }, LinePoint::TEnd });

			if (IS_DEBUG ()) {
				Cursor<cv::Vec3b> centroidCursor (_debugImage, center);
				// 青色方框：T形端点的平均宽度指示
				SquareIterator<cv::Vec3b> iter2 (centroidCursor, cvRound (feature[0] / 2.0f));
				while (iter2.moveNext ())
					if (iter2.IS_VALID ()) *iter2 = cv::Vec3b (150, 150, 0);
				Painter (_debugImage).line (center, out, Color::BGR (0, 150, 0));
			}
		}
		size_t nTEps = o_endPoints.size ();
		for (auto i = 1; i < nLabels; ++i) {
			if (states (i, cv::CC_STAT_AREA) < 4 || typeOfLabels[i] != LinePoint::Endpoint)
				continue;

			cv::Point2f center (centerOfLabels[i].x / centerOfLabels[i].z, centerOfLabels[i].y / centerOfLabels[i].z);
			if (std::any_of (o_endPoints.begin (), o_endPoints.begin () + nTEps,
				[center](const LinePoint& R_T_EP) { return cv::normL2Sqr<float> (R_T_EP.o.p - center) <= 7; }))
				continue;

			auto feature = accFeatureOfLabels[i] / accFeatureOfLabels[i][3];
			cv::Point2f out (feature[1], feature[2]);
			o_endPoints.push_back ({ { center, feature[0] }, { }, { out, feature[0] }, LinePoint::Endpoint });

			if (IS_DEBUG ()) {
				Cursor<cv::Vec3b> centroidCursor (_debugImage, center);
				// 紫色方框：端点的平均宽度指示
				SquareIterator<cv::Vec3b> iter2 (centroidCursor, cvRound (feature[0] / 2.0f));
				while (iter2.moveNext ())
					if (iter2.IS_VALID ()) *iter2 = cv::Vec3b (150, 0, 150);
				Painter (_debugImage).line (center, out, Color::BGR (0, 150, 0));
			}
		}
		_showDebugWindow ("LineExtractor - Debug", _debugImage);

	}



	void LineExtractor::_extractEndPoints1 (int radius, cv::Mat1b& io_typeMap,
		cv::Mat4f& io_accFeatureMap) {

		// Feature: type(Endpoint/T-Endpoint), lineWidth, outSeg_centerX, outSeg_centerY
		if (io_typeMap.empty ()) {
			io_typeMap = cv::Mat1b (_R_BINARY_IMAGE.SIZE (), LinePoint::Unknown);
			io_accFeatureMap = cv::Mat4f (_R_BINARY_IMAGE.SIZE (), { });
		}

		BasicIterator<uchar> it_binary (_R_BINARY_IMAGE);
		while (it_binary.moveNext ()) {
			if (*it_binary == 0) continue;

			// 遍历的像素若不在图像边缘，则不需要进行边界检查从而可以得到性能优化
			bool checkBoundary = it_binary.POSITION ().x < radius ||
				it_binary.POSITION ().y < radius ||
				_R_BINARY_IMAGE.WIDTH () - it_binary.POSITION ().x <= radius ||
				_R_BINARY_IMAGE.HEIGHT () - it_binary.POSITION ().y <= radius;

			SegInfo segInfo (it_binary.cursor (), radius, 1, checkBoundary);
			if (segInfo.segLengths.size () == 2) {
				// 端点：分段数为2（黑、白各1）且白色分段长度小于方形总长的1/4
				int whiteSegIndex = segInfo.segStarts[0].value (checkBoundary) > 0 ? 0 : 1;
				if (segInfo.segLengths[whiteSegIndex] < radius * 2)
					_fuseEndPointInfo (io_typeMap, io_accFeatureMap, it_binary, LinePoint::Endpoint,
						segInfo.SEG_TO_PORT (whiteSegIndex));
			} else if (segInfo.segLengths.size () == 6) {
				// T形端点：分段数为6（黑、白各3）且每段白色分段长度小于方形总长的1/4
				bool firstSegIsWhite = segInfo.segStarts[0].value (checkBoundary) > 0;
				bool isT_EP = true;
				for (int whiteSegIdx = !firstSegIsWhite; whiteSegIdx < segInfo.SIZE (); whiteSegIdx += 2) {
					if (segInfo.segLengths[whiteSegIdx] >= radius * 2) {
						isT_EP = false;
						break;
					}
				}
				if (isT_EP) {
					int blackSegWithMaxLength = -1;
					int maxLength = 0;
					for (int blackSegIdx = firstSegIsWhite; blackSegIdx < segInfo.SIZE (); blackSegIdx += 2) {
						if (segInfo.segLengths[blackSegIdx] > maxLength) {
							blackSegWithMaxLength = blackSegIdx;
							maxLength = segInfo.segLengths[blackSegIdx];
						}
					}
					int outSegIdx = (blackSegWithMaxLength + 3) % segInfo.SIZE ();
					_fuseEndPointInfo (io_typeMap, io_accFeatureMap, it_binary, LinePoint::TEnd,
						segInfo.SEG_TO_PORT (outSegIdx));
				}
			}
		}

	}



	void LineExtractor::_extractLines (const std::vector<LinePoint>& R_ENDPOINTS,
		std::vector<LineSeq>& o_lines) {

		// 求端点处的平均线宽，作为参考尺寸
		float averageLineWidth = 0.0f;
		for (auto i = 0; i < R_ENDPOINTS.size (); ++i)
			averageLineWidth += R_ENDPOINTS[i].o.width;
		averageLineWidth /= R_ENDPOINTS.size ();

		Painter painter (_debugImage);

		std::vector<bool> endpointsUsed (R_ENDPOINTS.size (), false);
		for (auto i = 0; i < R_ENDPOINTS.size (); ++i) {
			if (endpointsUsed[i])
				continue;

			// 开始一个新的线条
			o_lines.push_back (R_ENDPOINTS[i]);
			auto& r_line = o_lines[o_lines.size () - 1];
			auto endLine = false;
			endpointsUsed[i] = true;

			if (IS_DEBUG ())
				painter.circle (R_ENDPOINTS[i].o.p, 2.0f, Color::Green, -1);

			while (!endLine) {
				auto& R_CURR_LP = r_line[-1];
				auto radius = std::max (R_CURR_LP.o.width, averageLineWidth) * 1.5f;
				if (R_CURR_LP.type == LinePoint::Guess) radius *= 2;
				Cursor<uchar> centerCursor (_R_BINARY_IMAGE, (cv::Point2f)R_CURR_LP);

				_debugShowStep (r_line, (int)o_lines.size () - 1, (int)radius);

				// 为后继点查找一个候选端点
				int candidateEndpointIndex = -1;
				float candidateEndpointDistance;
				float candidateEndpointDot;
				for (auto j = i + 1; j < R_ENDPOINTS.size (); ++j) {
					if (endpointsUsed[j])
						continue;

					// 判断前序点和端点之间的距离是否符合条件
					auto v = R_ENDPOINTS[j] - R_CURR_LP;
					if (std::abs (v.x) > radius || std::abs (v.y) > radius)
						continue;
					auto mag_v = magnitude (v);
					if (mag_v > radius)
						continue;

					candidateEndpointIndex = j;
					candidateEndpointDistance = mag_v;
					candidateEndpointDot = (v / mag_v).dot (normalize (R_CURR_LP - R_CURR_LP.in.p));

					// 若前序点的out和端点很近，则认为端点为线条终点（替换前序点）
					if (candidateEndpointDistance < 3.0f) {
						r_line.modify (-1, R_ENDPOINTS[j], LinePoint::Endpoint);
						endLine = endpointsUsed[j] = true;
						_debugShowStep (r_line, (int)o_lines.size () - 1);
						break;
					}

					// 若前序方向和端点方向相近（约±30度），则追加该端点为线条终点
					if (candidateEndpointDot > 0.86f) {
						r_line.append (R_ENDPOINTS[j], LinePoint::Endpoint);
						endLine = endpointsUsed[j] = true;
						_debugShowStep (r_line, (int)o_lines.size () - 1);
						break;
					}
				}
				if (endLine)
					break;

				// 遍历的像素若不在图像边缘，则不需要进行边界检查从而可以得到性能优化
				bool checkBoundary = centerCursor.POSITION ().x < radius ||
					centerCursor.POSITION ().y < radius ||
					_R_BINARY_IMAGE.WIDTH () - centerCursor.POSITION ().x <= radius ||
					_R_BINARY_IMAGE.HEIGHT () - centerCursor.POSITION ().y <= radius;

				// 前序线段方向（归一化）
				auto inDirection = normalize (R_CURR_LP - R_CURR_LP.in.p);

				// 线段：存在一个小于两倍半径的白色分段，且其方向与前序线段方向近似相同
				SegInfo segInfo (centerCursor, (int)radius, 2, checkBoundary);
				if (segInfo.segLengths.size () < 2) {
					endLine = true;
				} else {
					// 遍历所有符合长度条件的白色分段，选取方向与前序线段最为接近的分段
					LinePoint::Port candidateOut;
					auto maxDot = -std::numeric_limits<float>::max ();
					for (auto i = segInfo.segStarts[0].value (checkBoundary) > 0 ? 0 : 1;
						i < segInfo.segLengths.size (); i += 2) {
						if (segInfo.segLengths[i] >= radius * 2)
							continue;

						// 计算当前分段首尾连线的中点位置（图中标 * 位置）和宽度
						//      ___
						//     |  /
						//     | * <- 
						//     |/
						//
						auto startMiddleEnd = segInfo.SEG_START_MIDDLE_END (i);
						cv::Point2f segStart (startMiddleEnd[0], startMiddleEnd[1]);
						cv::Point2f segEnd (startMiddleEnd[4], startMiddleEnd[5]);
						auto segCenter = (segStart + segEnd) / 2.0f;
						auto width = magnitude (segEnd - segStart);

						if (width > R_CURR_LP.o.width + std::max (3.0f, std::ceil (radius / 3.0f)))
							continue;

						auto dot = normalize (segCenter - R_CURR_LP).dot (inDirection);
						if (dot > maxDot) {
							maxDot = dot;
							candidateOut = { segCenter, width };
						}
					} // for segInfo
					if (maxDot <= 0.86f) // 夹角阈值：约±30度
						endLine = true;
					else
						r_line.append (candidateOut);
				}
				if (endLine) {
					// 猜一个后续节点
					cv::Point2f outDirections[3];
					if (std::abs (inDirection.x) < std::abs (inDirection.y)) {
						outDirections[0] = inDirection / std::abs (inDirection.y) * radius;
						outDirections[1] = outDirections[0] - cv::Point2f (1, 0);
						outDirections[2] = outDirections[0] + cv::Point2f (1, 0);
					} else {
						outDirections[0] = inDirection / std::abs (inDirection.x) * radius;
						outDirections[1] = outDirections[0] - cv::Point2f (0, 1);
						outDirections[2] = outDirections[0] + cv::Point2f (0, 1);
					}
					for (int j = 0; j < 3; ++j) {
						if ((centerCursor + outDirections[j]).value (checkBoundary) != 0) {
							r_line.append ({ R_CURR_LP.o.p + outDirections[j], R_CURR_LP.o.width },
								LinePoint::Guess);
							endLine = false;
							break;
						}
					}
				}
				if (endLine) {
					// 连接候选端点
					if (candidateEndpointIndex >= 0) {
						r_line.append (R_ENDPOINTS[candidateEndpointIndex]);
						endpointsUsed[candidateEndpointIndex] = true;
						_debugShowStep (r_line, (int)o_lines.size () - 1);
					}
				}
			} // while true

		} // for endpoints

	}



	void LineExtractor::_postprocessLines (std::vector<LineSeq>& io_lines) {

		// 删除所有guess=true的点
		for (auto i = 0; i < io_lines.size (); ++i)
			io_lines[i].removeGuessPoints ();

		// 两端不全是Endpoint的线认为可能是中断的，根据这一条件对现有线条进行分类
		std::vector<LineSeq*> closedLines, openingLines;
		for (auto i = 0; i < io_lines.size (); ++i) {
			if (io_lines[i][0].type == LinePoint::Endpoint &&
				io_lines[i][-1].type == LinePoint::Endpoint)
				closedLines.push_back (&io_lines[i]);
			else
				openingLines.push_back (&io_lines[i]);
		}

		// 尝试与其他中断的线进行连接
		for (auto i = 0; i < openingLines.size (); ++i) {
			if (openingLines[i]->SIZE () <= 1)
				continue;
			auto& r_thisLine = *openingLines[i];
			auto& r_lastLP = r_thisLine[-1];
			for (auto j = 0; j < openingLines.size (); ++j) {
				if (i == j || openingLines[j]->SIZE () <= 1)
					continue;

				auto& r_anotherLine = *openingLines[j];

				// 从尾部向首部搜索，尝试找到连接点的索引
				int connectIndex = -1;
				for (int k = r_anotherLine.SIZE () - 1; k > 0; --k) {
					auto& lp = r_anotherLine[k];
					cv::Rect2f r (lp.in.p, lp.o.p);
					r.x -= 2; r.y -= 2; r.width += 4; r.height += 4;
					if (!r.contains (r_lastLP))
						continue;
					auto v_o2in = lp.in - lp.o;
					auto v_o2in_norm = normalize (v_o2in);
					auto v_o2p = r_lastLP.o - lp.o;
					if (v_o2p.dot (v_o2p) - v_o2p.dot (v_o2in_norm) > 4)
						continue;
					auto v_pIn_norm = normalize (r_lastLP.o - r_lastLP.in);
					if (v_pIn_norm.dot (v_o2in_norm) < 0.7f)
						continue;
					connectIndex = k + 1;
					break;
				}
				if (connectIndex < 0)
					continue;

				// 合并线条，并从openingLines数组中移除
				for (int k = connectIndex; k >= 0; --k)
					r_thisLine.append (r_anotherLine[k], r_anotherLine[k].type);
				r_anotherLine.lps.clear ();
				openingLines.erase (openingLines.begin () + i);
				if (i < j) --j;
				openingLines.erase (openingLines.begin () + j);
				closedLines.push_back (&r_thisLine);
				--i;
				break;
			}
		}

		// 根据点数筛选线条
		std::vector<LineSeq> outLines;
		for (auto i = 0; i < io_lines.size (); ++i)
			if (io_lines[i].SIZE () > 2)
				outLines.push_back (io_lines[i]);
		io_lines = outLines;

	}



	void LineExtractor::_fuseEndPointInfo (cv::Mat1b& r_typeMap, cv::Mat4f& r_accFeatureMap,
		CvExt::BasicIterator<uchar> iter, uchar type, const LinePoint::Port& R_PORT) {

		auto& r_type = iter.valueRef (r_typeMap);
		auto& r_accFeature = iter.valueRef (r_accFeatureMap);
		cv::Vec4f info (R_PORT.width, R_PORT.p.x, R_PORT.p.y, 1.0f);

		// 若要融合的类型与已有类型不同，则：类型会按如下优先级覆盖：端点 > T-端点
		r_type = std::max (r_type, type);

		// 累加特征数值和权重
		r_accFeature += info;

	}
	*/


	void LineExtractor::_fuseEndPointInfo (cv::Mat1b& r_typeMap,
		cv::Mat_<cv::Vec<float, 7>>& r_accFeatureMap,
		CvExt::BasicIterator<uchar> iter, uchar type,
		const LinePoint::Port& R_IN, const LinePoint::Port& R_OUT) {

		auto& r_type = iter.valueRef (r_typeMap);
		auto& r_accFeature = iter.valueRef (r_accFeatureMap);
		cv::Vec<float, 7> info (R_IN.width, R_IN.x, R_IN.y, R_OUT.width, R_OUT.x, R_OUT.y, 1.0f);

		// 若要融合的类型与已有类型不同，则：类型会按如下优先级覆盖：端点 > 线上点 > T-端点
		if (r_type > type)
			return;
		if (r_type < type) {
			r_type = type;
			r_accFeature = info;
			return;
		}

		if (type == LinePoint::Normal) {
			// 线上点：若in和out方向与已有信息不符，则交换in和out
			auto vout = cv::Point2f (r_accFeature[4], r_accFeature[5]) / r_accFeature[6] -
				(cv::Point2f)iter.POSITION ();
			auto voutNew = R_OUT - (cv::Point2f)iter.POSITION ();
			if (vout.dot (voutNew) < 0) {
				info = { R_OUT.width, R_OUT.x, R_OUT.y, R_IN.width, R_IN.x, R_IN.y, 1.0f };
			}
		} else if (type == LinePoint::Endpoint) {
			// 端点：若in和out方向与已有信息不符，则忽略
			auto vout = cv::Point2f (r_accFeature[4], r_accFeature[5]) / r_accFeature[6] -
				(cv::Point2f)iter.POSITION ();
			auto voutNew = R_OUT - (cv::Point2f)iter.POSITION ();
			if (vout.dot (voutNew) < 0)
				return;
		}

		// 累加特征数值和权重
		r_accFeature += info;
	}



	LinePoint LineExtractor::_feature2Lp (cv::Point2f p,
		const cv::Mat_<cv::Vec<float, 7>>& R_ACC_FEATURE_MAP) {

		auto feature = R_ACC_FEATURE_MAP (p);
		if (feature[6] != 0.0f)
			feature /= feature[6];
		LinePoint lp;
		lp.in = { { feature[1], feature[2] }, feature[0] };
		lp.out = { { feature[4], feature[5] }, feature[3] };
		lp = { p, (lp.in.width + lp.out.width) / 2.0f };
		return lp;

	}



	LinePoint::Port LineExtractor::_calculateOut (const LinePoint::Port& R_O,
		const LinePoint::Port& R_RAW_OUT, const cv::Mat_<ushort>& R_CC_LABEL_MAP,
		ushort label, bool& o_guess) {
		
		o_guess = false;
		auto out = R_RAW_OUT;
		if (R_CC_LABEL_MAP (out) == label)
			return out;
		float rawVoutLengthSqr = cv::normL2Sqr<float> (R_RAW_OUT - R_O);
		float voutLengthSqr = rawVoutLengthSqr;
		while (true) {
			voutLengthSqr /= 4;
			(cv::Point2f&)out = (R_O + R_RAW_OUT) / 2;
			out.width = (R_O.width + R_RAW_OUT.width) / 2;
			if (voutLengthSqr < 4.0f)
				break;
			if (R_CC_LABEL_MAP (out) == label)
				return out;
		}
		o_guess = true;
		out = R_RAW_OUT;
		voutLengthSqr = rawVoutLengthSqr;
		while (voutLengthSqr >= 4.0f) {
			if (Cursor<uchar> (_R_BINARY_IMAGE, out).valueRef () > 0)
				return out;
			voutLengthSqr /= 4;
			(cv::Point2f&)out = (R_O + R_RAW_OUT) / 2;
			out.width = (R_O.width + R_RAW_OUT.width) / 2;
		}
		return { };

	}



	void LineExtractor::_debugShowStep (const LineSeq& r_line, int lineId, int radius) {
		if (!IS_DEBUG ())
			return;
		auto& r_lineColor = _debugLineColors[lineId % _debugLineColors.size ()];
		Painter painter (_debugImage);
		switch (r_line[-1].type) {
		case LinePoint::Guess:
			painter.line (r_line[-2], r_line[-1], r_lineColor * 0.5)
				.circle (r_line[-1], 1.5f, r_lineColor * 0.5, -1);
			break;
		case LinePoint::Endpoint:
			painter.line (r_line[-2], r_line[-1], r_lineColor)
				.circle (r_line[-1], 2.0f, Color::Red, -1);
			break;
		default:
			painter.line (r_line[-2], r_line[-1], r_lineColor)
				.circle (r_line[-1], 1.5f, r_lineColor, -1);
			break;
		}
		cv::Mat viewImage;
		if (radius > 0) {
			auto debugImage1 = _debugImage.CLONE ();
			SquareIterator<cv::Vec3b> debug_iter (
				Cursor<cv::Vec3b> (debugImage1, r_line[-1]), radius);
			while (debug_iter.moveNext ()) {
				if (debug_iter.IS_VALID ())
					*debug_iter = *debug_iter / 2 + cv::Vec3b (0, 128, 128);
			}
			cv::resize (debugImage1, viewImage, _debugImage.SIZE () * 2);
		} else
			cv::resize (_debugImage, viewImage, _debugImage.SIZE () * 2);
		_showDebugWindow ("LineExtractor - Debug", viewImage);
	}

} NS_DLIB_PATTERN_END