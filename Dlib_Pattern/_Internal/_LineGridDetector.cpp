#include "LineGridDetector.h"
#include "SegInfo.h"
#include "LineExtractor.h"
#include "CrossPointExtractor.h"
#include "GridInfer.h"
#include "BlobDetector.h"

using namespace Dlib::CvExt;



NS_DLIB_PATTERN_BEGIN namespace _Internal {

	int maxLineWidth = 10;
	int minLineConstrast = 40;

	void gradientGray (cv::Mat& src, cv::Mat& mag) {
		const int H = src.rows, W = src.cols;
		cv::Mat Ix (H, W, CV_32S), Iy (H, W, CV_32S);
		//因为计算出的梯度值可能有正有负，且值也可能会很大，故数据类型为整形

		// 求水平方向梯度，处理左右边缘像素
		for (int y = 0; y < H; y++) {
			Ix.at<int> (y, 0) = abs (src.at<uchar> (y, 1) - src.at<uchar> (y, 0)) * 2;
			for (int x = 1; x < W - 1; x++)
				Ix.at<int> (y, x) = abs (src.at<uchar> (y, x + 1) - src.at<uchar> (y, x - 1));
			Ix.at<int> (y, W - 1) = abs (src.at<uchar> (y, W - 1) - src.at<uchar> (y, W - 2)) * 2;
		}
		// 求垂直方向梯度，处理左右边缘像素
		for (int x = 0; x < W; x++) {
			Iy.at<int> (0, x) = abs (src.at<uchar> (1, x) - src.at<uchar> (0, x)) * 2;
			for (int y = 1; y < H - 1; y++)
				Iy.at<int> (y, x) = abs (src.at<uchar> (y + 1, x) - src.at<uchar> (y - 1, x));
			Iy.at<int> (H - 1, x) = abs (src.at<uchar> (H - 1, x) - src.at<uchar> (H - 2, x)) * 2;
		}
		/*for (int j = 0; j < H; j++)
			for (int k = 0; k < W; k++)
			{
				mag.at<byte>(j, k) = min(Ix.at<int>(j,k) + Iy.at<int>(j, k), 255);
			}*/
		cv::convertScaleAbs (min (Ix + Iy, 255), mag); //这句话和上面的for循环是同样的功能
	}



	bool LineGridDetector::run (const Image& R_IMG, cv::Point2f* results,
		Image& r_drawLayer, bool isDebug) {

		bool res = false;

		// 0. 前置操作：初始化结果数组，清空drawLayer
		for (size_t i = 0; i < _R_PATTERN.N_ALL_CORNERS; ++i)
			results[i] = { NAN, NAN };
		_setDebug (isDebug);
		if (!r_drawLayer.IS_EMPTY ())
			Painter (r_drawLayer).fill (Color::Zero);

		// 1. 对输入图像进行预处理，使用自适应二值化方法计算线条的二值图
		auto inputGray = _preprocessInputImage (R_IMG);
		Image8 inputBinary;
		cv::adaptiveThreshold (inputGray, inputBinary, 255.0f, cv::ADAPTIVE_THRESH_MEAN_C,
			cv::THRESH_BINARY, maxLineWidth * 2 + 1, -minLineConstrast / 2.0);

		/*{
			Image8 gradient, gradientBinary;
			gradientGray (inputGray, gradient);
			cv::threshold (gradient, gradientBinary, 0, 255, cv::THRESH_OTSU);
			_showDebugWindow ("Gradient", { gradient, gradientBinary });
			cv::waitKey ();
		}*/

		// 2. 利用连通域方法提取pattern区域
		std::vector<Contour> contours;
		std::vector<cv::Vec4i> hierarchy;
		auto mainContourIdx = _extractContour (inputBinary, contours, hierarchy);
		res = (mainContourIdx >= 0);
		cv::Rect roi;
		std::vector<cv::KeyPoint> keypoints;
		std::vector<int> patternLineContourIdxs;
		res = res && _analyzeContours (inputBinary, contours, hierarchy, mainContourIdx,
			roi, keypoints, patternLineContourIdxs);

		if (IS_DEBUG ()) {
			auto debugImage = _createDebugImage (inputBinary, false);
			BasicIterator<cv::Vec3b> it (debugImage);
			while (it.moveNext ()) *it /= 5;
			if (mainContourIdx >= 0) {
				Painter painter (debugImage);
				cv::drawContours (debugImage, contours, -1, { 100, 100, 100 });
				cv::drawContours (debugImage, contours, mainContourIdx,
					{ 255, 255, 255 }, -1, 8, hierarchy, 1);
				cv::drawContours (debugImage, contours, mainContourIdx,
					{ 0, 0, 255 }, 1, 8, hierarchy, 1);
				for (auto idx : patternLineContourIdxs)
					if (idx != mainContourIdx) {
						cv::drawContours (debugImage, contours, idx, { 255, 255, 255 }, -1);
						cv::drawContours (debugImage, contours, idx, { 0, 255, 0 });
					}
				for (auto& R_BLOB : keypoints)
					painter.marker (R_BLOB.pt, cv::MARKER_CROSS, cvRound (R_BLOB.size), Color::Yellow, 2);
			}
			_showDebugWindow ("LineGridDetector: Preprocess & Contour Analysis",
				{ inputGray, debugImage });
		}

		if (!res) return false;

		for (auto& keypoint : keypoints)
			keypoint.pt -= (cv::Point2f)roi.tl ();
		Image8 patternBinary;
		_drawSubImageOfPatternLines (contours, hierarchy, patternLineContourIdxs,
			roi, patternBinary);
		_showDebugWindow ("LineGridDetector: Sub Image of Pattern", patternBinary);

		auto extractLineDebugImage = _createDebugImage (patternBinary, false);

		// 3. 根据二值图提取所有线条
		std::vector<LineSeq*> lines;
		res = LineExtractor (patternBinary, _debugExtractLines).run (lines) && lines.size () >= 10;
		_debugDrawExtractLinesResult (extractLineDebugImage, lines);

		if (!res) {
			// 在绘图层绘制检测结果并返回
			auto gridInferDebugImage = _createDebugImage (patternBinary, true);
			_showDebugWindow ("LineGridDetector: Extract Lines && Grid Infer",
				{ extractLineDebugImage, gridInferDebugImage });
			_drawGridDetectResult (r_drawLayer, roi, lines, keypoints);
			return false;
		}

		// 5. 寻找交叉点，然后推导网格坐标
		auto conns = CrossPointExtractor (patternBinary, lines, _debugExtractCross).run ();
		cv::Point2f origin (-1.0f, -1.0f);
		res = GridInfer (lines, conns, keypoints, _pattSize).run (origin);

		auto gridInferDebugImage = _createDebugImage (patternBinary, false);
		_debugDrawExtractLinesResult (gridInferDebugImage, lines, conns, origin);
		_showDebugWindow ("LineGridDetector: Extract Lines && Grid Infer",
			{ extractLineDebugImage, gridInferDebugImage });

		if (res) {
			// 将交点坐标对应填入全网格数组中。
			for (int i = 0; i < conns.size (); ++i) {
				if (!conns[i]->IS_COORD_VALID ())
					continue;
				auto coord = conns[i]->COORD ();
				if (coord.x >= 0 && coord.x < _pattSize.width &&
					coord.y >= 0 && coord.y < _pattSize.height)
					results[coord.x + coord.y * _pattSize.width] =
						conns[i]->p + (cv::Point2f)roi.tl ();
			}
			// 填入中心点坐标
			results[_pattSize.width / 2 + _pattSize.height / 2 * _pattSize.width] =
				origin + (cv::Point2f)roi.tl ();
		}

		// 在绘图层绘制检测结果
		_drawGridDetectResult (r_drawLayer, roi, lines, conns, origin);

		// 清除分配的临时数据
		for (auto p_line : lines) delete p_line;
		for (auto p_conn : conns) delete p_conn;

		return res;

	}



	Image8 LineGridDetector::_preprocessInputImage (const Image& R_IMG) {

		// 如果输入图像不是灰度图，则将其转为灰度图
		Image8 inImage8;
		if (inImage8.FORMAT () == R_IMG.FORMAT ())
			inImage8 = R_IMG;
		else
			R_IMG.CONVERT_TO (inImage8);

		/* 用相邻帧取均值降噪
		cv::Mat avgGrayImage;
		if (_lastFrameImage.empty ())
		avgGrayImage = inImage8.clone ();
		else
		cv::addWeighted (inImage8, 0.5, _lastFrameImage, 0.5, 0.0, avgGrayImage);
		_lastFrameImage = inImage8;
		*/

		auto processedGrayImage = _adjustConstrast (inImage8); // 对比度调整
		Image8 processedGrayImage2;
		cv::medianBlur (processedGrayImage, processedGrayImage2, 3); // 中值滤波降低椒盐噪声
		return processedGrayImage2;

	}



	Image8 LineGridDetector::_adjustConstrast (Image8 gray) {

		float clipHistPercent = 1.0f;
		int histSize = 256;
		double minGray = 0, maxGray = 0;

		//to calculate grayscale histogram
		if (clipHistPercent == 0) {
			// keep full available range
			cv::minMaxLoc (gray, &minGray, &maxGray);
		} else {
			cv::Mat hist; //the grayscale histogram

			float range[] = { 0, 256 };
			const float* histRange = { range };
			bool uniform = true;
			bool accumulate = false;
			cv::calcHist (&(cv::Mat&)gray, 1, 0, cv::Mat (), hist, 1,
				&histSize, &histRange, uniform, accumulate);

			// calculate cumulative distribution from the histogram
			std::vector<float> accumulator (histSize);
			accumulator[0] = hist.at<float> (0);
			for (int i = 1; i < histSize; i++) {
				accumulator[i] = accumulator[i - 1] + hist.at<float> (i);
			}

			// locate points that cuts at required value
			float max = accumulator.back ();
			clipHistPercent *= (max / 100.0f); //make percent as absolute
			clipHistPercent /= 2.0f; // left and right wings
										// locate left cut
			minGray = 0;
			while (accumulator[(int)minGray] < clipHistPercent)
				minGray++;

			// locate right cut
			maxGray = histSize - 1;
			while (accumulator[(int)maxGray] >= (max - clipHistPercent))
				maxGray--;
		}

		// current range
		float inputRange = (float)(maxGray - minGray);

		// alpha expands current range to histsize range
		float alpha = (histSize - 1) / inputRange;
		// beta shifts current range so that minGray will go to 0
		float beta = (float)(-minGray * alpha);

		// Apply brightness and contrast normalization
		Image8 output;
		gray->convertTo (output, -1, alpha, beta);
		return output;

	}



	int LineGridDetector::_extractContour (const Image8& R_BINARY_IMAGE,
		std::vector<Contour>& o_contours, std::vector<cv::Vec4i>& o_hierarchy) {

		// 查找图像中的轮廓
		// 轮廓层次信息：0 - 下一个兄弟轮廓, 1 - 上一个兄弟轮廓, 2 - 子轮廓, 3 - 父轮廓）
		cv::findContours (R_BINARY_IMAGE, o_contours, o_hierarchy,
			cv::RETR_TREE, cv::CHAIN_APPROX_NONE);

		// 查找含有最多子轮廓的轮廓，认为是pattern区域的轮廓
		std::vector<int> childrenCountList (o_hierarchy.size ());
		for (int i = 0; i < o_hierarchy.size (); ++i) {
			childrenCountList[i] = 0;
			for (auto childIndex = o_hierarchy[i][2]; childIndex != -1;
				childIndex = o_hierarchy[childIndex][0]) {
				if (o_contours[childIndex].size () >= 100)
					childrenCountList[i]++;
			}
		}
		if (childrenCountList.empty ()) return -1;
		auto maxCountIter = std::max_element (childrenCountList.begin (), childrenCountList.end ());
		if (*maxCountIter < 4) return -1;
		return (int)(maxCountIter - childrenCountList.begin ());

	}



	bool LineGridDetector::_analyzeContours (const Image8& R_BINARY_IMAGE,
		const std::vector<Contour>& R_CONTOURS, const std::vector<cv::Vec4i>& R_HIERARCHY,
		int mainContourIdx, cv::Rect& o_roi, std::vector<cv::KeyPoint>& o_blobs,
		std::vector<int>& o_patternLineContourIdxs) {

		auto& r_mainContour = R_CONTOURS[mainContourIdx];
		cv::Mat1b patternHullMap;

		// 计算轮廓包围盒（边界留白5个像素）
		o_roi = cv::boundingRect (r_mainContour);
		//o_roi.x -= 5; o_roi.y -= 5; o_roi.width += 10; o_roi.height += 10;

		// 提取pattern轮廓内含有的所有白色实心轮廓
		std::vector<bool> contourBelongsToPattern (R_CONTOURS.size (), false);
		contourBelongsToPattern[mainContourIdx] = true;
		std::queue<int> queue;
		queue.push (mainContourIdx);
		while (!queue.empty ()) {
			auto parentIdx = queue.front ();
			queue.pop ();
			// 遍历白色轮廓内的空洞轮廓
			for (auto holeIdx = R_HIERARCHY[parentIdx][2];
				holeIdx != -1; holeIdx = R_HIERARCHY[holeIdx][0]) {
				contourBelongsToPattern[holeIdx] = true;
				// 遍历空洞轮廓内的白色轮廓
				for (auto childIdx = R_HIERARCHY[holeIdx][2];
					childIdx != -1; childIdx = R_HIERARCHY[childIdx][0]) {
					contourBelongsToPattern[childIdx] = true;
					// 若白色轮廓没有子轮廓，则为实心轮廓，否则，加入队列
					if (R_HIERARCHY[childIdx][2] == -1)
						o_patternLineContourIdxs.push_back (childIdx);
					else
						queue.push (childIdx);
				}
			}
		}

		// 检查其他不属于pattern轮廓内的白色实心轮廓，若在pattern轮廓的凸包范围内则加入数组
		for (auto i = 0; i < R_CONTOURS.size (); ++i) {
			// 跳过属于pattern轮廓内的轮廓或非实心的轮廓或黑色轮廓
			if (contourBelongsToPattern[i] || R_HIERARCHY[i][2] != -1 ||
				*Cursor<uchar> (R_BINARY_IMAGE, R_CONTOURS[i][0]) != 255)
				continue;
			// 跳过有任何轮廓点不在凸包范围内的轮廓
			if (!std::all_of (R_CONTOURS[i].begin (), R_CONTOURS[i].end (),
				[&o_roi, &patternHullMap, &r_mainContour, this](const cv::Point& R_POINT) {
					if (!o_roi.contains (R_POINT)) return false;
					// 仅在需要时：计算凸包并绘制凸包图
					if (patternHullMap.empty ()) {
						std::vector<std::vector<cv::Point>> patternHull (1);
						cv::convexHull (r_mainContour, patternHull[0]);
						patternHullMap = cv::Mat1b (o_roi.size (), 0);
						cv::drawContours (patternHullMap, patternHull, 0,
							255, -1, 8, cv::noArray (), INT_MAX, -o_roi.tl ());
					}
					return patternHullMap (R_POINT - o_roi.tl ()) == 255;
				}
				))
				continue;
				o_patternLineContourIdxs.push_back (i);
		}

		// 提取白色圆点，并将其他轮廓归为pattern line轮廓
		o_blobs = _detectBlobs (R_CONTOURS, o_patternLineContourIdxs);
		o_patternLineContourIdxs.push_back (mainContourIdx);

		return !o_blobs.empty ();

	}



	void LineGridDetector::_drawSubImageOfPatternLines (
		const std::vector<Contour>& R_CONTOURS, const std::vector<cv::Vec4i>& R_HIERARCHY,
		const std::vector<int>& R_IDXS, const cv::Rect& R_ROI, Image8& o_patternImage) {

		o_patternImage = Image8 (R_ROI.size (), Color::Black);
		for (auto idx : R_IDXS)
			cv::drawContours (o_patternImage, R_CONTOURS, idx, 255,
				-1, 8, R_HIERARCHY, 1, -R_ROI.tl ());

	}



	cv::Mat LineGridDetector::_extractSubImageOfPatternByContourAndRotate (
		const Image8& R_BINARY, const Image8& R_GRAY, Image8& o_binary, Image8& o_gray) {

		// 查找图像中的轮廓（按树形结构存储）
		std::vector<std::vector<cv::Point>> contours;
		std::vector<cv::Vec4i> hierarchy;
		cv::findContours (R_BINARY, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_NONE);

		// 查找含有最多子轮廓的轮廓，认为是pattern区域的轮廓
		std::vector<int> childrenCountList (hierarchy.size ());
		for (int i = 0; i < hierarchy.size (); ++i) {
			childrenCountList[i] = 0;
			for (auto childIndex = hierarchy[i][2]; childIndex != -1;
				childIndex = hierarchy[childIndex][0]) {
				if (contours[childIndex].size () >= 100)
					childrenCountList[i]++;
			}
		}
		cv::Point maxIdx;
		cv::minMaxIdx (childrenCountList, NULL, NULL, NULL, &maxIdx.x);
		auto& patternContour = contours[maxIdx.y];

		// 绘制pattern的轮廓到单独的图像中
		Image8 imageOfPattern (R_BINARY.SIZE (), Color::Black);
		cv::drawContours (imageOfPattern, contours, maxIdx.y, { 255 }, -1, 8, hierarchy, 2);

		// 计算包围盒和旋转矩阵
		auto inBBox = cv::minAreaRect (patternContour);
		auto rot = cv::getRotationMatrix2D (inBBox.center, inBBox.angle, 1);
		rot.at<double> (0, 2) += inBBox.size.width / 2.0 - inBBox.center.x;
		rot.at<double> (1, 2) += inBBox.size.height / 2.0 - inBBox.center.y;

		// 对pattern区域进行旋转和子图提取操作
		cv::warpAffine (imageOfPattern, o_binary, rot, inBBox.size, cv::INTER_NEAREST);
		cv::warpAffine (R_GRAY, o_gray, rot, inBBox.size);

		if (IS_DEBUG ()) {
			auto debugImage = _createDebugImage (R_BINARY, true);
			cv::drawContours (debugImage, contours, -1, { 128, 128, 128 }, 1, 8);
			cv::drawContours (debugImage, contours, maxIdx.y, { 0, 0, 255 }, 1, 8, hierarchy, 2);
			cv::drawContours (debugImage, contours, maxIdx.y, { 0, 255, 0 }, 1, 8);
			_showDebugWindow ("LineGridDetector: Pattern Contour", debugImage);
		}

		return rot;

	}



	bool LineGridDetector::_isLinePixel (const Cursor<uchar>& R_CURSOR_CENTER,
		int& o_bgIntensity) {

		int half = (int)(maxLineWidth * 1.5 / 2);
		o_bgIntensity = -1;

		bool upCondition = false;
		auto cursor = R_CURSOR_CENTER + MoveUp;
		for (int dy = -1; dy >= -half && cursor.POSITION ().y >= 0;
			--dy, cursor += MoveUp) {
			if (*R_CURSOR_CENTER - *cursor < minLineConstrast)
				continue;
			upCondition = true;
			o_bgIntensity = *cursor;
			break;
		}
		if (!upCondition)
			return false;

		bool downCondition = false;
		auto p1 = R_CURSOR_CENTER + MoveDown;
		for (int dy = 1; dy <= half && cursor.POSITION ().y < cursor.MAT_SIZE ().height;
			++dy, cursor += MoveDown) {
			if (*R_CURSOR_CENTER - *cursor < minLineConstrast)
				continue;
			downCondition = true;
			o_bgIntensity = (o_bgIntensity + *cursor) / 2;
			break;
		}
		if (!downCondition)
			o_bgIntensity = -1;
		return downCondition;

	}



	std::vector<cv::KeyPoint> LineGridDetector::_detectBlobs (
		const std::vector<Contour>& R_CONTOURS, std::vector<int>& io_idxs) {

		// 用OpenCV的圆点检测类检测图像中的圆点并返回
		auto minArea = 4 * 4;
		auto maxArea = 14 * 14;
		auto minConvexity = 0.85f;
		auto minIntertialRatio = 0.3f;
		std::vector<cv::KeyPoint> blobs;

		std::vector<int> notBlobContourIdxs;
		for (auto contourIdx : io_idxs) {
			cv::Moments moms = moments (R_CONTOURS[contourIdx]);

			// 检查面积
			if (moms.m00 < minArea || moms.m00 >= maxArea) {
				notBlobContourIdxs.push_back (contourIdx);
				continue;
			}

			/*
			if (params.filterByCircularity) {
				double area = moms.m00;
				double perimeter = arcLength (R_CONTOURS[contourIdx], true);
				double ratio = 4 * CV_PI * area / (perimeter * perimeter);
				if (ratio < params.minCircularity || ratio >= params.maxCircularity)
					continue;
			}
			*/

			// 检查惯性率
			auto rect = cv::minAreaRect (R_CONTOURS[contourIdx]);
			auto ratio = rect.size.width / rect.size.height;
			if (ratio > 1.0f) ratio = 1.0f / ratio;
			/*
			double denominator = std::sqrt (std::pow (2 * moms.mu11, 2) +
				std::pow (moms.mu20 - moms.mu02, 2));
			const double eps = 1e-2;
			double ratio;
			if (denominator > eps) {
				double cosmin = (moms.mu20 - moms.mu02) / denominator;
				double sinmin = 2 * moms.mu11 / denominator;
				double cosmax = -cosmin;
				double sinmax = -sinmin;

				double imin = 0.5 * (moms.mu20 + moms.mu02) -
					0.5 * (moms.mu20 - moms.mu02) * cosmin - moms.mu11 * sinmin;
				double imax = 0.5 * (moms.mu20 + moms.mu02) -
					0.5 * (moms.mu20 - moms.mu02) * cosmax - moms.mu11 * sinmax;
				ratio = imin / imax;
			} else {
				ratio = 1;
			}
			*/
			if (ratio < minIntertialRatio) {
				notBlobContourIdxs.push_back (contourIdx);
				continue;
			}

			// 检查凸度
			std::vector<cv::Point> hull;
			cv::convexHull (R_CONTOURS[contourIdx], hull);
			double area = cv::contourArea (R_CONTOURS[contourIdx]);
			double hullArea = cv::contourArea (hull);
			if (fabs (hullArea) < DBL_EPSILON || area / hullArea < minConvexity) {
				notBlobContourIdxs.push_back (contourIdx);
				continue;
			}

			// 计算圆点的中心和尺寸，并加入数组
			cv::Point2d center (moms.m10 / moms.m00, moms.m01 / moms.m00);
			cv::Size size = cv::boundingRect (R_CONTOURS[contourIdx]).size ();
			blobs.push_back ({ (cv::Point2f)center, (size.width + size.height) / 2.0f });
		}
		io_idxs = notBlobContourIdxs;
		return blobs;

	}



	void LineGridDetector::_drawGridDetectResult (Image& r_drawLayer, cv::Rect roi,
		const std::vector<LineSeq*>& R_LINES, const std::vector<cv::KeyPoint>& R_BLOBS) {

		if (r_drawLayer.IS_EMPTY ()) return;
		Painter (r_drawLayer).text ("Debug", { 10, 40 },
			cv::FONT_HERSHEY_PLAIN, 1.0, Color::Red, 1);
		auto subImage = r_drawLayer.SUB_IMAGE (roi);
		Painter painter (r_drawLayer, 1.0f, roi.tl ());
		_debugDrawExtractLinesResult (painter, R_LINES);
		for (int i = 0; i < R_BLOBS.size (); ++i)
			painter.circle (R_BLOBS[i].pt, R_BLOBS[i].size / 2.0f,
				Color::BGR (255, 0, 255), -1);

	}



	void LineGridDetector::_drawGridDetectResult (Image& r_drawLayer, cv::Rect roi,
		const std::vector<LineSeq*>& R_LINES, const std::vector<Conn*>& R_CONNS,
		cv::Point2f origin) {

		if (r_drawLayer.IS_EMPTY ()) return;
		Painter (r_drawLayer).text ("Debug", { 10, 40 },
			cv::FONT_HERSHEY_PLAIN, 1.0, Color::Red, 1);
		auto subImage = r_drawLayer.SUB_IMAGE (roi);
		_debugDrawExtractLinesResult (Painter (r_drawLayer, 1.0f, roi.tl ()),
			R_LINES, R_CONNS, origin);

	}



	void LineGridDetector::_debugDrawExtractLinesResult (Painter painter,
		const std::vector<LineSeq*>& R_LINES, const std::vector<Conn*>& R_CONNS,
		cv::Point2f origin) {

		if (painter.img ().IS_EMPTY ())
			return;

		// 画线
		for (auto i = 0; i < R_LINES.size (); ++i) {
			Color c;
			switch (R_LINES[i]->type) {
			case LineSeq::Horizontal: c = Color::BGR (39, 127, 255); break;
			case LineSeq::Vertical: c = Color::BGR (232, 162, 0); break;
			default: c = Color::BGR (255, 0, 255); break;
			}
			R_LINES[i]->drawLines (painter, c, 1.0f,
				R_LINES[i]->coord == LineSeq::INVALID_COORD || origin.x < 0.0f);
		}

		// 画点
		for (auto i = 0; i < R_LINES.size (); ++i) {
			Color c;
			switch (R_LINES[i]->type) {
			case LineSeq::Horizontal: c = Color::BGR (39, 127, 255); break;
			case LineSeq::Vertical: c = Color::BGR (232, 162, 0); break;
			default: c = Color::BGR (255, 0, 255); break;
			}
			R_LINES[i]->drawPoints (painter, c, 1.0f,
				R_LINES[i]->coord == LineSeq::INVALID_COORD || origin.x < 0.0f);
		}

		// 绘制交叉点
		for (auto i = 0; i < R_CONNS.size (); ++i) {
			Color c = Color::BGR (255, 255, 0);
			if (!R_CONNS[i]->IS_COORD_VALID () || origin.x < 0.0f)
				c /= 2;
			painter.marker (R_CONNS[i]->p, cv::MARKER_CROSS, 7, Color::Black, 2);
			painter.marker (R_CONNS[i]->p, cv::MARKER_CROSS, 7, c);
		}

		// 绘制原点并标注坐标
		if (origin.x >= 0) {
			painter.marker (origin, cv::MARKER_CROSS, 9, Color::Black, 4);
			painter.marker (origin, cv::MARKER_CROSS, 9, Color::BGR (255, 100, 255), 2);

			// 标注线条的坐标
			for (int i = 0; i < R_LINES.size (); ++i) {
				auto& r_line = *R_LINES[i];
				if (r_line.coord == LineSeq::INVALID_COORD)
					continue;
				std::ostringstream sout;
				sout << r_line.coord;
				if (r_line.type == LineSeq::Horizontal)
					painter.text (sout.str (), r_line[0] + cv::Point2f (-5, -5),
						cv::FONT_HERSHEY_PLAIN, 1.0, Color::Red, 1);
				else
					painter.text (sout.str (), r_line[0] + cv::Point2f (5, 5),
						cv::FONT_HERSHEY_PLAIN, 1.0, Color::Green, 1);
			}
			for (auto i = 0; i < R_CONNS.size (); ++i) {
				if (!R_CONNS[i]->IS_COORD_VALID ())
					continue;
				auto coord = R_CONNS[i]->COORD ();
				std::ostringstream sout; sout << "(" << coord.x << "," << coord.y << ")";
				cv::Point2f putTextPoint (
					R_CONNS[i]->p.x > painter.img ().WIDTH () - 30 ?
						R_CONNS[i]->p.x - 25 :
						R_CONNS[i]->p.x + 5,
					R_CONNS[i]->p.y > painter.img ().HEIGHT () - 15 ?
						R_CONNS[i]->p.y - 5 :
						R_CONNS[i]->p.y + 10
				);
				painter.text (sout.str (), putTextPoint, cv::FONT_HERSHEY_PLAIN,
					0.5, Color::BGR (0, 255, 255));
			}

		}

	}

} NS_DLIB_PATTERN_END