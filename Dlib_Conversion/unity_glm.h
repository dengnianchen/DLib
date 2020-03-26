#pragma once
#include "global.h"
#include <glm/glm.hpp>



NS_DLIB_CVT_BEGIN

inline glm::vec3 un2gl (const glm::vec3& R_UNITY_VEC) {
	return { R_UNITY_VEC.x, R_UNITY_VEC.y, -R_UNITY_VEC.z };
}



inline glm::vec4 un2gl (const glm::vec4& R_UNITY_VEC) {
	return { R_UNITY_VEC.x, R_UNITY_VEC.y, -R_UNITY_VEC.z, R_UNITY_VEC.w };
}



inline glm::mat3 un2gl (const glm::mat3& R_UNITY_MAT) {
	glm::mat3 glmMat = R_UNITY_MAT;
	for (int c = 0; c < 3; ++c)
		glmMat[c][2] = -glmMat[c][2];
	glmMat[2] = -glmMat[2];
	return glmMat;
}



inline glm::mat4 un2gl (const glm::mat4& R_UNITY_MAT) {
	glm::mat4 glmMat = R_UNITY_MAT;
	for (int c = 0; c < 4; ++c)
		glmMat[c][2] = -glmMat[c][2];
	glmMat[2] = -glmMat[2];
	return glmMat;
}



inline glm::vec3 gl2un (const glm::vec3& R_GLM_VEC) {
	return { R_GLM_VEC.x, R_GLM_VEC.y, -R_GLM_VEC.z };
}



inline glm::vec4 gl2un (const glm::vec4& R_GLM_VEC) {
	return { R_GLM_VEC.x, R_GLM_VEC.y, -R_GLM_VEC.z, R_GLM_VEC.w };
}



inline glm::mat3 gl2un (const glm::mat3& R_GLM_MAT) {
	glm::mat3 unityMat = R_GLM_MAT;
	for (int c = 0; c < 3; ++c)
		unityMat[c][2] = -unityMat[c][2];
	unityMat[2] = -unityMat[2];
	return unityMat;
}



inline glm::mat4 gl2un (const glm::mat4& R_GLM_MAT) {
	glm::mat4 unityMat = R_GLM_MAT;
	for (int c = 0; c < 4; ++c)
		unityMat[c][2] = -unityMat[c][2];
	unityMat[2] = -unityMat[2];
	return unityMat;
}

NS_DLIB_CVT_END
