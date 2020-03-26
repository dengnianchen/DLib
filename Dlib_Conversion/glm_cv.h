#pragma once
#include "global.h"
#include <opencv2/opencv.hpp>
#include <glm/glm.hpp>



NS_DLIB_CVT_BEGIN

template< class T >
inline glm::vec2 toGVec2 (cv::Point_<T> val) {
	return glm::vec2 (float (val.x), float (val.y));
}

template< class T >
inline glm::vec3 toGVec3 (cv::Point3_<T> val) {
	return glm::vec3 (float (val.x), float (val.y), float (val.z));
}

inline glm::mat3 toGMat3 (cv::Mat val) {
	assert (val.rows >= 3 && val.cols >= 3 && val.channels () == 1 && (val.depth () == CV_32F || val.depth () == CV_64F));
	glm::mat3 glmMatrix;
	for (int r = 0; r < 3; ++r)
		for (int c = 0; c < 3; ++c)
			glmMatrix[c][r] = (val.depth () == CV_32F) ? val.at< float > (r, c) : (float)val.at< double > (r, c);
	return glmMatrix;
}

inline glm::mat4 toGMat4 (cv::Mat val) {
	assert (val.rows >= 4 && val.cols >= 4 && val.channels () == 1 && (val.depth () == CV_32F || val.depth () == CV_64F));
	glm::mat4 glmMatrix;
	for (int r = 0; r < 4; ++r)
		for (int c = 0; c < 4; ++c)
			glmMatrix[c][r] = (val.depth () == CV_32F) ? val.at< float > (r, c) : (float)val.at< double > (r, c);
	return glmMatrix;
}

inline cv::Mat toCvMat (glm::mat3 val) {
	cv::Mat cvMatrix (3, 3, CV_32FC1);
	for (int r = 0; r < 3; ++r)
		for (int c = 0; c < 3; ++c)
			cvMatrix.at< float > (r, c) = val[c][r];
	return cvMatrix;
}

inline cv::Mat toCvMat (glm::mat4 val) {
	cv::Mat cvMatrix (4, 4, CV_32FC1);
	for (int r = 0; r < 4; ++r)
		for (int c = 0; c < 4; ++c)
			cvMatrix.at< float > (r, c) = val[c][r];
	return cvMatrix;
}

NS_DLIB_CVT_END