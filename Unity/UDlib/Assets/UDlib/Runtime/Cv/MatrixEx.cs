using System;
using UDlib.Core.Mathmatics;
using UnityEngine;

namespace UDlib.Cv {

	public static class MatrixEx {

		public static void Set (this Matrix<float> _this, Vector3 a) {
			if (_this.NCols != 1 && _this.NRows != 1)
				throw new ArgumentException ("Size mismatch", nameof(a));
			try {
				_this[0] = a.x;
				_this[1] = a.y;
				_this[2] = a.z;
			} catch (IndexOutOfRangeException) {
				// ignore IndexOutOfRangeException
			}
		}

		public static void Set (this Matrix<float> _this, Vector4 a) {
			if (_this.NCols != 1 && _this.NRows != 1)
				throw new ArgumentException ("Size mismatch", nameof(a));
			try {
				_this[0] = a.x;
				_this[1] = a.y;
				_this[2] = a.z;
				_this[3] = a.w;
			} catch (IndexOutOfRangeException) {
				// ignore IndexOutOfRangeException
			}
		}

		public static void Set (this Matrix<float> _this, Matrix3x3 a) {
			SetIfInRange (_this, 0, 0, a.m00);
			SetIfInRange (_this, 0, 1, a.m01);
			SetIfInRange (_this, 0, 2, a.m02);
			SetIfInRange (_this, 1, 0, a.m10);
			SetIfInRange (_this, 1, 1, a.m11);
			SetIfInRange (_this, 1, 2, a.m12);
			SetIfInRange (_this, 2, 0, a.m20);
			SetIfInRange (_this, 2, 1, a.m21);
			SetIfInRange (_this, 2, 2, a.m22);
		}

		public static void Set (this Matrix<float> _this, Matrix4x4 a) {
			SetIfInRange (_this, 0, 0, a.m00);
			SetIfInRange (_this, 0, 1, a.m01);
			SetIfInRange (_this, 0, 2, a.m02);
			SetIfInRange (_this, 0, 3, a.m03);
			SetIfInRange (_this, 1, 0, a.m10);
			SetIfInRange (_this, 1, 1, a.m11);
			SetIfInRange (_this, 1, 2, a.m12);
			SetIfInRange (_this, 1, 3, a.m13);
			SetIfInRange (_this, 2, 0, a.m20);
			SetIfInRange (_this, 2, 1, a.m21);
			SetIfInRange (_this, 2, 2, a.m22);
			SetIfInRange (_this, 2, 3, a.m23);
			SetIfInRange (_this, 3, 0, a.m30);
			SetIfInRange (_this, 3, 1, a.m31);
			SetIfInRange (_this, 3, 2, a.m32);
			SetIfInRange (_this, 3, 3, a.m33);
		}

		private static void SetIfInRange (Matrix<float> m, int r, int c, float value) {
			if (r >= m.NRows || c >= m.NCols)
				return;
			m[r, c] = value;
		}

		public static void Get (this Matrix<float> _this, out Vector3 a) {
			if (_this.NCols != 1 && _this.NRows != 1)
				throw new ArgumentException ("Size mismatch", nameof(a));
			a = Vector3.zero;
			try {
				a.x = _this[0];
				a.y = _this[1];
				a.z = _this[2];
			} catch (IndexOutOfRangeException) {
				// ignore IndexOutOfRangeException
			}
		}

		public static void Get (this Matrix<float> _this, out Vector4 a) {
			if (_this.NCols != 1 && _this.NRows != 1)
				throw new ArgumentException ("Size mismatch", nameof(a));
			a = Vector4.zero;
			try {
				a.x = _this[0];
				a.y = _this[1];
				a.z = _this[2];
				a.w = _this[3];
			} catch (IndexOutOfRangeException) {
				// ignore IndexOutOfRangeException
			}
		}

		public static void Get (this Matrix<float> _this, out Matrix3x3 a) {
			a.m00 = GetIfInRange (_this, 0, 0);
			a.m01 = GetIfInRange (_this, 0, 1);
			a.m02 = GetIfInRange (_this, 0, 2);
			a.m10 = GetIfInRange (_this, 1, 0);
			a.m11 = GetIfInRange (_this, 1, 1);
			a.m12 = GetIfInRange (_this, 1, 2);
			a.m20 = GetIfInRange (_this, 2, 0);
			a.m21 = GetIfInRange (_this, 2, 1);
			a.m22 = GetIfInRange (_this, 2, 2);
		}

		public static void Get (this Matrix<float> _this, out Matrix4x4 a) {
			a.m00 = GetIfInRange (_this, 0, 0);
			a.m01 = GetIfInRange (_this, 0, 1);
			a.m02 = GetIfInRange (_this, 0, 2);
			a.m03 = GetIfInRange (_this, 0, 3);
			a.m10 = GetIfInRange (_this, 1, 0);
			a.m11 = GetIfInRange (_this, 1, 1);
			a.m12 = GetIfInRange (_this, 1, 2);
			a.m13 = GetIfInRange (_this, 1, 3);
			a.m20 = GetIfInRange (_this, 2, 0);
			a.m21 = GetIfInRange (_this, 2, 1);
			a.m22 = GetIfInRange (_this, 2, 2);
			a.m23 = GetIfInRange (_this, 2, 3);
			a.m30 = GetIfInRange (_this, 3, 0);
			a.m31 = GetIfInRange (_this, 3, 1);
			a.m32 = GetIfInRange (_this, 3, 2);
			a.m33 = GetIfInRange (_this, 3, 3);
		}

		private static float GetIfInRange (Matrix<float> m, int r, int c)
			=> r >= m.NRows || c >= m.NCols ? 0.0f : m[r, c];

	}

}