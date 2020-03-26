#include "BlobDetector.h"

using namespace cv;

#define DEBUG_BLOB_DETECTOR



NS_DLIB_PATTERN_BEGIN namespace _Internal {

	class SimpleBlobDetectorImpl : public SimpleBlobDetector {
	public:
		explicit SimpleBlobDetectorImpl (const SimpleBlobDetector::Params& parameters) :
			params (parameters) {
		}

		virtual void read (const FileNode& fn) override { params.read (fn); }

		virtual void write (FileStorage& fs) const override {
			writeFormat (fs);
			params.write (fs);
		}

	protected:
		struct Center {
			Point2d location;
			double radius;
			double confidence;
		};

		virtual void detect (InputArray image, std::vector<KeyPoint>& keypoints, InputArray mask = noArray ()) override;
		virtual void findBlobs (InputArray binaryImage, std::vector<Center>& centers) const;

		Params params;

	};



	SimpleBlobDetector::Params::Params () {

		minDistBetweenBlobs = 10;

		filterByColor = true;
		blobColor = 0;

		filterByArea = true;
		minArea = 25;
		maxArea = 5000;

		filterByCircularity = false;
		minCircularity = 0.8f;
		maxCircularity = std::numeric_limits<float>::max ();

		filterByInertia = true;
		//minInertiaRatio = 0.6;
		minInertiaRatio = 0.1f;
		maxInertiaRatio = std::numeric_limits<float>::max ();

		filterByConvexity = true;
		//minConvexity = 0.8;
		minConvexity = 0.95f;
		maxConvexity = std::numeric_limits<float>::max ();

	}



	void SimpleBlobDetector::Params::read (const cv::FileNode& fn) {

		minDistBetweenBlobs = fn["minDistBetweenBlobs"];

		filterByColor = (int)fn["filterByColor"] != 0 ? true : false;
		blobColor = (uchar)(int)fn["blobColor"];

		filterByArea = (int)fn["filterByArea"] != 0 ? true : false;
		minArea = fn["minArea"];
		maxArea = fn["maxArea"];

		filterByCircularity = (int)fn["filterByCircularity"] != 0 ? true : false;
		minCircularity = fn["minCircularity"];
		maxCircularity = fn["maxCircularity"];

		filterByInertia = (int)fn["filterByInertia"] != 0 ? true : false;
		minInertiaRatio = fn["minInertiaRatio"];
		maxInertiaRatio = fn["maxInertiaRatio"];

		filterByConvexity = (int)fn["filterByConvexity"] != 0 ? true : false;
		minConvexity = fn["minConvexity"];
		maxConvexity = fn["maxConvexity"];

	}



	void SimpleBlobDetector::Params::write (cv::FileStorage& fs) const {

		fs << "minDistBetweenBlobs" << minDistBetweenBlobs;

		fs << "filterByColor" << (int)filterByColor;
		fs << "blobColor" << (int)blobColor;

		fs << "filterByArea" << (int)filterByArea;
		fs << "minArea" << minArea;
		fs << "maxArea" << maxArea;

		fs << "filterByCircularity" << (int)filterByCircularity;
		fs << "minCircularity" << minCircularity;
		fs << "maxCircularity" << maxCircularity;

		fs << "filterByInertia" << (int)filterByInertia;
		fs << "minInertiaRatio" << minInertiaRatio;
		fs << "maxInertiaRatio" << maxInertiaRatio;

		fs << "filterByConvexity" << (int)filterByConvexity;
		fs << "minConvexity" << minConvexity;
		fs << "maxConvexity" << maxConvexity;

	}



	void SimpleBlobDetectorImpl::findBlobs (InputArray _binaryImage,
		std::vector<Center>& centers) const {

		Mat binaryImage = _binaryImage.getMat ();
		centers.clear ();

		std::vector < std::vector<Point> > contours;
		findContours (binaryImage, contours, RETR_LIST, CHAIN_APPROX_NONE);

#ifdef DEBUG_BLOB_DETECTOR
		Mat keypointsImage;
		cvtColor (binaryImage, keypointsImage, COLOR_GRAY2RGB);

		Mat contoursImage;
		cvtColor (binaryImage, contoursImage, COLOR_GRAY2RGB);
		drawContours (contoursImage, contours, -1, Scalar (0, 255, 0));
		imshow ("contours", contoursImage);
#endif

		for (size_t contourIdx = 0; contourIdx < contours.size (); contourIdx++) {
			Center center;
			center.confidence = 1;
			Moments moms = moments (contours[contourIdx]);
			if (params.filterByArea) {
				double area = moms.m00;
				if (area < params.minArea || area >= params.maxArea)
					continue;
			}

			if (params.filterByCircularity) {
				double area = moms.m00;
				double perimeter = arcLength (contours[contourIdx], true);
				double ratio = 4 * CV_PI * area / (perimeter * perimeter);
				if (ratio < params.minCircularity || ratio >= params.maxCircularity)
					continue;
			}

			if (params.filterByInertia) {
				double denominator = std::sqrt (std::pow (2 * moms.mu11, 2) + std::pow (moms.mu20 - moms.mu02, 2));
				const double eps = 1e-2;
				double ratio;
				if (denominator > eps) {
					double cosmin = (moms.mu20 - moms.mu02) / denominator;
					double sinmin = 2 * moms.mu11 / denominator;
					double cosmax = -cosmin;
					double sinmax = -sinmin;

					double imin = 0.5 * (moms.mu20 + moms.mu02) - 0.5 * (moms.mu20 - moms.mu02) * cosmin - moms.mu11 * sinmin;
					double imax = 0.5 * (moms.mu20 + moms.mu02) - 0.5 * (moms.mu20 - moms.mu02) * cosmax - moms.mu11 * sinmax;
					ratio = imin / imax;
				} else {
					ratio = 1;
				}

				if (ratio < params.minInertiaRatio || ratio >= params.maxInertiaRatio)
					continue;

				center.confidence = ratio * ratio;
			}

			if (params.filterByConvexity) {
				std::vector < Point > hull;
				convexHull (contours[contourIdx], hull);
				double area = contourArea (contours[contourIdx]);
				double hullArea = contourArea (hull);
				if (fabs (hullArea) < DBL_EPSILON)
					continue;
				drawContours (keypointsImage, std::vector<std::vector<Point>> (1, hull), -1, Scalar (0, 255, 0), 1, cv::LINE_AA);
				double ratio = area / hullArea;
				if (ratio < params.minConvexity || ratio >= params.maxConvexity)
					continue;
			}

			if (moms.m00 == 0.0)
				continue;
			center.location = Point2d (moms.m10 / moms.m00, moms.m01 / moms.m00);

			if (params.filterByColor) {
				if (binaryImage.at<uchar> (cvRound (center.location.y), cvRound (center.location.x)) != params.blobColor)
					continue;
			}

			//compute blob radius
			{
				std::vector<double> dists;
				for (size_t pointIdx = 0; pointIdx < contours[contourIdx].size (); pointIdx++) {
					Point2d pt = contours[contourIdx][pointIdx];
					dists.push_back (norm (center.location - pt));
				}
				std::sort (dists.begin (), dists.end ());
				center.radius = (dists[(dists.size () - 1) / 2] + dists[dists.size () / 2]) / 2.;
			}

			centers.push_back (center);


#ifdef DEBUG_BLOB_DETECTOR
			circle (keypointsImage, center.location, 1, Scalar (0, 0, 255), 1);
#endif
		}
#ifdef DEBUG_BLOB_DETECTOR
		imshow ("bk", keypointsImage);
		waitKey ();
#endif
	}



	void SimpleBlobDetectorImpl::detect (InputArray image,
		std::vector<cv::KeyPoint>& keypoints, InputArray mask) {

		keypoints.clear ();
		Mat binarizedImage = image.getMat ();
		if (binarizedImage.type () != CV_8UC1)
			CV_Error (Error::StsUnsupportedFormat, "Blob detector only supports 8-bit images!");

		std::vector < std::vector<Center> > centers;
		std::vector < Center > curCenters;
		findBlobs (binarizedImage, curCenters);
		std::vector < std::vector<Center> > newCenters;
		for (size_t i = 0; i < curCenters.size (); i++) {
			bool isNew = true;
			for (size_t j = 0; j < centers.size (); j++) {
				double dist = norm (centers[j][centers[j].size () / 2].location - curCenters[i].location);
				isNew = dist >= params.minDistBetweenBlobs && dist >= centers[j][centers[j].size () / 2].radius && dist >= curCenters[i].radius;
				if (!isNew) {
					centers[j].push_back (curCenters[i]);

					size_t k = centers[j].size () - 1;
					while (k > 0 && curCenters[i].radius < centers[j][k - 1].radius) {
						centers[j][k] = centers[j][k - 1];
						k--;
					}
					centers[j][k] = curCenters[i];

					break;
				}
			}
			if (isNew)
				newCenters.push_back (std::vector<Center> (1, curCenters[i]));
		}
		std::copy (newCenters.begin (), newCenters.end (), std::back_inserter (centers));

		for (size_t i = 0; i < centers.size (); i++) {
			Point2d sumPoint (0, 0);
			double normalizer = 0;
			for (size_t j = 0; j < centers[i].size (); j++) {
				sumPoint += centers[i][j].confidence * centers[i][j].location;
				normalizer += centers[i][j].confidence;
			}
			sumPoint *= (1. / normalizer);
			KeyPoint kpt (sumPoint, (float)(centers[i][centers[i].size () / 2].radius) * 2.0f);
			keypoints.push_back (kpt);
		}

		if (!mask.empty ()) {
			KeyPointsFilter::runByPixelsMask (keypoints, mask.getMat ());
		}

	}



	Ptr<SimpleBlobDetector> SimpleBlobDetector::create (
		const SimpleBlobDetector::Params& params) {

		return makePtr<SimpleBlobDetectorImpl> (params);

	}

} NS_DLIB_PATTERN_END