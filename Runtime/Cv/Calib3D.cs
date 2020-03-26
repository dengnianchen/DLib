using UDlib.Core;
using UnityEngine;

namespace UDlib.Cv {

	public static class Calib3D {

		public static Vector3[] TriangulatePoints (Matrix<float> projMat1, Matrix<float> projMat2,
			Array<Vector2> projPoints1, Array<Vector2> projPoints2,
			out float o_error1, out float o_error2) {
			var projPoints1_ = new Matrix<Vector2> (1, projPoints1.Length, projPoints1);
			var projPoints2_ = new Matrix<Vector2> (1, projPoints2.Length, projPoints2);
			var points4D = new Matrix<float> (4, projPoints1.Length);
			Plugin.triangulatePoints(projMat1, projMat2, projPoints1_, projPoints2_, points4D,
				out o_error1, out o_error2);
			var pointsRet = new Vector3[projPoints1.Length];
			for (var i = 0; i < projPoints1.Length; ++i)
				pointsRet[i] = new Vector3(
					points4D[0, i] / points4D[3, i],
					points4D[1, i] / points4D[3, i],
					points4D[2, i] / points4D[3, i]);
			return pointsRet;
		}

	}

}