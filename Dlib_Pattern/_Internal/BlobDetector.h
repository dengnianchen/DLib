#include "../global.h"



NS_DLIB_PATTERN_BEGIN namespace _Internal {

    /** @brief Class for extracting blobs from an image. :

    The class implements a simple algorithm for extracting blobs from an image:

    1.  Convert the source image to binary images by applying thresholding with several thresholds from
        minThreshold (inclusive) to maxThreshold (exclusive) with distance thresholdStep between
        neighboring thresholds.
    2.  Extract connected components from every binary image by findContours and calculate their
        centers.
    3.  Group centers from several binary images by their coordinates. Close centers form one group that
        corresponds to one blob, which is controlled by the minDistBetweenBlobs parameter.
    4.  From the groups, estimate final centers of blobs and their radiuses and return as locations and
        sizes of keypoints.

    This class performs several filtrations of returned blobs. You should set filterBy\* to true/false
    to turn on/off corresponding filtration. Available filtrations:

    -   **By color**. This filter compares the intensity of a binary image at the center of a blob to
    blobColor. If they differ, the blob is filtered out. Use blobColor = 0 to extract dark blobs
    and blobColor = 255 to extract light blobs.
    -   **By area**. Extracted blobs have an area between minArea (inclusive) and maxArea (exclusive).
    -   **By circularity**. Extracted blobs have circularity
    (\f$\frac{4*\pi*Area}{perimeter * perimeter}\f$) between minCircularity (inclusive) and
    maxCircularity (exclusive).
    -   **By ratio of the minimum inertia to maximum inertia**. Extracted blobs have this ratio
    between minInertiaRatio (inclusive) and maxInertiaRatio (exclusive).
    -   **By convexity**. Extracted blobs have convexity (area / area of blob convex hull) between
    minConvexity (inclusive) and maxConvexity (exclusive).

    Default values of parameters are tuned to extract dark circular blobs.
     */
    class SimpleBlobDetector : public cv::Feature2D {
    public:
        struct Params {
            Params ();
            float minDistBetweenBlobs;

            bool filterByColor;
            uchar blobColor;

            bool filterByArea;
            float minArea, maxArea;

            bool filterByCircularity;
            float minCircularity, maxCircularity;

            bool filterByInertia;
            float minInertiaRatio, maxInertiaRatio;

            bool filterByConvexity;
            float minConvexity, maxConvexity;

            void read (const cv::FileNode& fn);
            void write (cv::FileStorage& fs) const;
        };

        static cv::Ptr<SimpleBlobDetector> create (const SimpleBlobDetector::Params& parameters = {});

        virtual cv::String getDefaultName () const override {
            return (cv::Feature2D::getDefaultName () + ".SimpleBlobDetector");
        }

    };

} NS_DLIB_PATTERN_END