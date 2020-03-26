#pragma once
#include "global.h"
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
#include <QMatrix3x3>
#include <QMatrix4x4>
#include <glm/glm.hpp>



NS_DLIB_CVT_BEGIN

inline QVector2D toQVec2 (const glm::vec2& R_VAL) { return QVector2D (R_VAL.x, R_VAL.y); }
inline QVector3D toQVec3 (const glm::vec3& R_VAL) { return QVector3D (R_VAL.x, R_VAL.y, R_VAL.z); }
inline QVector4D toQVec4 (const glm::vec4& R_VAL) { return QVector4D (R_VAL.x, R_VAL.y, R_VAL.z, R_VAL.w); }



inline glm::vec2 toGVec2 (const QVector2D& R_VAL) { return glm::vec2 (R_VAL.x (), R_VAL.y ()); }
inline glm::vec3 toGVec3 (const QVector3D& R_VAL) { return glm::vec3 (R_VAL.x (), R_VAL.y (), R_VAL.z ()); }
inline glm::vec4 toGVec4 (const QVector4D& R_VAL) { return glm::vec4 (R_VAL.x (), R_VAL.y (), R_VAL.z (), R_VAL.w ()); }


inline QMatrix3x3 toQMat3 (const glm::mat3& R_VAL) {
	float matData[] = {
		R_VAL[0][0], R_VAL[1][0], R_VAL[2][0],
		R_VAL[0][1], R_VAL[1][1], R_VAL[2][1],
		R_VAL[0][2], R_VAL[1][2], R_VAL[2][2]
	};
	return QMatrix3x3 (matData);
}
inline QMatrix4x4 toQMat4 (const glm::mat4& R_VAL) {
	return QMatrix4x4 (
		R_VAL[0][0], R_VAL[1][0], R_VAL[2][0], R_VAL[3][0],
		R_VAL[0][1], R_VAL[1][1], R_VAL[2][1], R_VAL[3][1],
		R_VAL[0][2], R_VAL[1][2], R_VAL[2][2], R_VAL[3][2],
		R_VAL[0][3], R_VAL[1][3], R_VAL[2][3], R_VAL[3][3]);
}


inline glm::mat3 toGMat3 (const QMatrix3x3& R_VAL) {
	glm::mat3 gmat;
	for (int r = 0; r < 3; ++r)
		for (int c = 0; c < 3; ++c)
			gmat[c][r] = R_VAL (r, c);
	return gmat;
}
inline glm::mat4 toGMat4 (const QMatrix4x4& R_VAL) {
	glm::mat4 gmat;
	for (int r = 0; r < 4; ++r)
		for (int c = 0; c < 4; ++c)
			gmat[c][r] = R_VAL (r, c);
	return gmat;
}

NS_DLIB_CVT_END