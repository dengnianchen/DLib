using UnityEngine;

namespace UDlib.Core.Mathmatics {

	public static class MatrixEx {

		public static void SetData (this Matrix4x4 m, float[] mdata) {
			m.m00 = mdata[0];
			m.m01 = mdata[1];
			m.m02 = mdata[2];
			m.m03 = mdata[3];
			m.m10 = mdata[4];
			m.m11 = mdata[5];
			m.m12 = mdata[6];
			m.m13 = mdata[7];
			m.m20 = mdata[8];
			m.m21 = mdata[9];
			m.m22 = mdata[10];
			m.m23 = mdata[11];
			m.m30 = mdata[12];
			m.m31 = mdata[13];
			m.m32 = mdata[14];
			m.m33 = mdata[15];
		}

		public static float[] Data (this Matrix4x4 m) {
			return new[] {
				m.m00, m.m01, m.m02, m.m03,
				m.m10, m.m11, m.m12, m.m13,
				m.m20, m.m21, m.m22, m.m23,
				m.m30, m.m31, m.m32, m.m33
			};
		}
		
		public static void SetData (this Matrix3x3 m, float[] mdata) {
			m.m00 = mdata[0];
			m.m01 = mdata[1];
			m.m02 = mdata[2];
			m.m10 = mdata[3];
			m.m11 = mdata[4];
			m.m12 = mdata[5];
			m.m20 = mdata[6];
			m.m21 = mdata[7];
			m.m22 = mdata[8];
		}

		public static float[] Data (this Matrix3x3 m) {
			return new[] {
				m.m00, m.m01, m.m02,
				m.m10, m.m11, m.m12,
				m.m20, m.m21, m.m22
			};
		}

	}

}