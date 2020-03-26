using System;
using System.Globalization;
using UnityEngine;

namespace UDlib.Core.Mathmatics {

	/// <summary>
	///   <para>A standard 3x3 transformation matrix.</para>
	/// </summary>
	public struct Matrix3x3 : IEquatable<Matrix3x3> {

		private static readonly Matrix3x3 zeroMatrix = new Matrix3x3 (
			new Vector3 (0.0f, 0.0f, 0.0f),
			new Vector3 (0.0f, 0.0f, 0.0f),
			new Vector3 (0.0f, 0.0f, 0.0f));
		private static readonly Matrix3x3 identityMatrix = new Matrix3x3 (
			new Vector3 (1.0f, 0.0f, 0.0f),
			new Vector3 (0.0f, 1.0f, 0.0f),
			new Vector3 (0.0f, 0.0f, 1.0f));
		public float m00;
		public float m10;
		public float m20;
		public float m01;
		public float m11;
		public float m21;
		public float m02;
		public float m12;
		public float m22;

		/*
		private Quaternion GetRotation () {
			Quaternion ret;
			Matrix3x3.GetRotation_Injected (ref this, out ret);
			return ret;
		}
	
		private Vector3 GetLossyScale () {
			Vector3 ret;
			Matrix3x3.GetLossyScale_Injected (ref this, out ret);
			return ret;
		}
	
		private bool IsIdentity () {
			return Matrix3x3.IsIdentity_Injected (ref this);
		}
	
		private float GetDeterminant () {
			return Matrix3x3.GetDeterminant_Injected (ref this);
		}
	
		private FrustumPlanes DecomposeProjection () {
			FrustumPlanes ret;
			Matrix3x3.DecomposeProjection_Injected (ref this, out ret);
			return ret;
		}
	
		/// <summary>
		///   <para>Attempts to get a rotation quaternion from this matrix.</para>
		/// </summary>
		public Quaternion rotation {
			get { return this.GetRotation (); }
		}
	
		/// <summary>
		///   <para>Attempts to get a scale value from the matrix. (Read Only)</para>
		/// </summary>
		public Vector3 lossyScale {
			get { return this.GetLossyScale (); }
		}
	
		/// <summary>
		///   <para>Checks whether this is an identity matrix. (Read Only)</para>
		/// </summary>
		public bool isIdentity {
			get { return this.IsIdentity (); }
		}
	
		/// <summary>
		///   <para>The determinant of the matrix. (Read Only)</para>
		/// </summary>
		public float determinant {
			get { return this.GetDeterminant (); }
		}
	
		/// <summary>
		///   <para>This property takes a projection matrix and returns the six plane coordinates that define a projection frustum.</para>
		/// </summary>
		public FrustumPlanes decomposeProjection {
			get { return this.DecomposeProjection (); }
		}
	
		public static float Determinant (Matrix3x3 m) {
			return m.determinant;
		}
		
		[FreeFunction ("MatrixScripting::Inverse", IsThreadSafe = true)]
		public static Matrix3x3 Inverse (Matrix3x3 m) {
			Matrix3x3 ret;
			Matrix3x3.Inverse_Injected (ref m, out ret);
			return ret;
		}
		/// <summary>
		///   <para>The inverse of this matrix. (Read Only)</para>
		/// </summary>
		public Matrix3x3 inverse {
			get { return Matrix3x3.Inverse (this); }
		}
	
		*/

		public static Matrix3x3 Transpose (Matrix3x3 m) {
			return new Matrix3x3 (
				new Vector3 (m.m00, m.m01, m.m02),
				new Vector3 (m.m10, m.m11, m.m12),
				new Vector3 (m.m20, m.m21, m.m22));
		}

		/// <summary>
		///   <para>Returns the transpose of this matrix (Read Only).</para>
		/// </summary>
		public Matrix3x3 transpose => Transpose (this);

		public Matrix3x3 (Vector3 column0, Vector3 column1, Vector3 column2) {
			m00 = column0.x;
			m01 = column1.x;
			m02 = column2.x;
			m10 = column0.y;
			m11 = column1.y;
			m12 = column2.y;
			m20 = column0.z;
			m21 = column1.z;
			m22 = column2.z;
		}

		public float this [int row, int column] {
			get => this[row + column * 3];
			set => this[row + column * 3] = value;
		}

		public float this [int index] {
			get {
				switch (index) {
					case 0: return m00;
					case 1: return m10;
					case 2: return m20;
					case 3: return m01;
					case 4: return m11;
					case 5: return m21;
					case 6: return m02;
					case 7: return m12;
					case 8: return m22;
					default: throw new IndexOutOfRangeException ("Invalid matrix index!");
				}
			}
			set {
				switch (index) {
					case 0:
						m00 = value;
						break;
					case 1:
						m10 = value;
						break;
					case 2:
						m20 = value;
						break;
					case 3:
						m01 = value;
						break;
					case 4:
						m11 = value;
						break;
					case 5:
						m21 = value;
						break;
					case 6:
						m02 = value;
						break;
					case 7:
						m12 = value;
						break;
					case 8:
						m22 = value;
						break;
					default: throw new IndexOutOfRangeException ("Invalid matrix index!");
				}
			}
		}

		public override int GetHashCode () =>
			GetColumn (0).GetHashCode () ^
			(GetColumn (1).GetHashCode () << 2) ^
			(GetColumn (2).GetHashCode () >> 2);

		public override bool Equals (object other) => other is Matrix3x3 other1 && Equals (other1);

		public bool Equals (Matrix3x3 other) =>
			GetColumn (0).Equals (other.GetColumn (0)) &&
			GetColumn (1).Equals (other.GetColumn (1)) &&
			GetColumn (2).Equals (other.GetColumn (2));

		public static Matrix3x3 operator * (Matrix3x3 lhs, Matrix3x3 rhs) =>
			new Matrix3x3 {
				m00 = lhs.m00 * rhs.m00 + lhs.m01 * rhs.m10 + lhs.m02 * rhs.m20,
				m01 = lhs.m00 * rhs.m01 + lhs.m01 * rhs.m11 + lhs.m02 * rhs.m21,
				m02 = lhs.m00 * rhs.m02 + lhs.m01 * rhs.m12 + lhs.m02 * rhs.m22,
				m10 = lhs.m10 * rhs.m00 + lhs.m11 * rhs.m10 + lhs.m12 * rhs.m20,
				m11 = lhs.m10 * rhs.m01 + lhs.m11 * rhs.m11 + lhs.m12 * rhs.m21,
				m12 = lhs.m10 * rhs.m02 + lhs.m11 * rhs.m12 + lhs.m12 * rhs.m22,
				m20 = lhs.m20 * rhs.m00 + lhs.m21 * rhs.m10 + lhs.m22 * rhs.m20,
				m21 = lhs.m20 * rhs.m01 + lhs.m21 * rhs.m11 + lhs.m22 * rhs.m21,
				m22 = lhs.m20 * rhs.m02 + lhs.m21 * rhs.m12 + lhs.m22 * rhs.m22
			};

		public static Vector3 operator * (Matrix3x3 lhs, Vector3 vector) =>
			new Vector3 {
				x = lhs.m00 * vector.x + lhs.m01 * vector.y + lhs.m02 * vector.z,
				y = lhs.m10 * vector.x + lhs.m11 * vector.y + lhs.m12 * vector.z,
				z = lhs.m20 * vector.x + lhs.m21 * vector.y + lhs.m22 * vector.z
			};

		public static bool operator == (Matrix3x3 lhs, Matrix3x3 rhs) => lhs.Equals (rhs);

		public static bool operator != (Matrix3x3 lhs, Matrix3x3 rhs) => !lhs.Equals (rhs);

		/// <summary>
		///   <para>Get a column of the matrix.</para>
		/// </summary>
		/// <param name="index"></param>
		public Vector3 GetColumn (int index) {
			switch (index) {
				case 0:
					return new Vector3 (m00, m10, m20);
				case 1:
					return new Vector3 (m01, m11, m21);
				case 2:
					return new Vector3 (m02, m12, m22);
				default:
					throw new IndexOutOfRangeException ("Invalid column index!");
			}
		}

		/// <summary>
		///   <para>Returns a row of the matrix.</para>
		/// </summary>
		/// <param name="index"></param>
		public Vector3 GetRow (int index) {
			switch (index) {
				case 0:
					return new Vector3 (m00, m01, m02);
				case 1:
					return new Vector3 (m10, m11, m12);
				case 2:
					return new Vector3 (m20, m21, m22);
				default:
					throw new IndexOutOfRangeException ("Invalid row index!");
			}
		}

		/// <summary>
		///   <para>Sets a column of the matrix.</para>
		/// </summary>
		/// <param name="index"></param>
		/// <param name="column"></param>
		public void SetColumn (int index, Vector3 column) {
			this[0, index] = column.x;
			this[1, index] = column.y;
			this[2, index] = column.z;
		}

		/// <summary>
		///   <para>Sets a row of the matrix.</para>
		/// </summary>
		/// <param name="index"></param>
		/// <param name="row"></param>
		public void SetRow (int index, Vector3 row) {
			this[index, 0] = row.x;
			this[index, 1] = row.y;
			this[index, 2] = row.z;
		}

		/// <summary>
		///   <para>Creates a scaling matrix.</para>
		/// </summary>
		/// <param name="vector"></param>
		public static Matrix3x3 Scale (Vector3 vector) =>
			new Matrix3x3 {
				m00 = vector.x, m01 = 0.0f, m02 = 0.0f,
				m10 = 0.0f, m11 = vector.y, m12 = 0.0f,
				m20 = 0.0f, m21 = 0.0f, m22 = vector.z
			};

		/// <summary>
		///   <para>Creates a rotation matrix.</para>
		/// </summary>
		/// <param name="q"></param>
		public static Matrix3x3 Rotate (Quaternion q) {
			var num1 = q.x * 2f;
			var num2 = q.y * 2f;
			var num3 = q.z * 2f;
			var num4 = q.x * num1;
			var num5 = q.y * num2;
			var num6 = q.z * num3;
			var num7 = q.x * num2;
			var num8 = q.x * num3;
			var num9 = q.y * num3;
			var num10 = q.w * num1;
			var num11 = q.w * num2;
			var num12 = q.w * num3;
			return new Matrix3x3 {
				m00 = 1.0f - (num5 + num6),
				m10 = num7 + num12,
				m20 = num8 - num11,
				m01 = num7 - num12,
				m11 = 1.0f - (num4 + num6),
				m21 = num9 + num10,
				m02 = num8 + num11,
				m12 = num9 - num10,
				m22 = 1.0f - (num4 + num5)
			};
		}

		/// <summary>
		///   <para>Returns a matrix with all elements set to zero (Read Only).</para>
		/// </summary>
		public static Matrix3x3 zero => zeroMatrix;

		/// <summary>
		///   <para>Returns the identity matrix (Read Only).</para>
		/// </summary>
		public static Matrix3x3 identity => identityMatrix;

		/// <summary>
		///   <para>Returns a nicely formatted string for this matrix.</para>
		/// </summary>
		public override string ToString () =>
			string.Format (
				"{0:F5}\t{1:F5}\t{2:F5}\n{3:F5}\t{4:F5}\t{5:F5}\n{6:F5}\t{7:F5}\t{8:F5}\n",
				m00, m01, m02, m10, m11, m12, m20, m21, m22);

		/// <summary>
		///   <para>Returns a nicely formatted string for this matrix.</para>
		/// </summary>
		/// <param name="format"></param>
		public string ToString (string format) =>
			string.Format ("{0}\t{1}\t{2}\n{3}\t{4}\t{5}\n{6}\t{7}\t{8}\n",
				ToInvariantString (format, m00),
				ToInvariantString (format, m01),
				ToInvariantString (format, m02),
				ToInvariantString (format, m10),
				ToInvariantString (format, m11),
				ToInvariantString (format, m12),
				ToInvariantString (format, m20),
				ToInvariantString (format, m21),
				ToInvariantString (format, m22));

		private static string ToInvariantString (string format, float val) =>
			val.ToString (format, CultureInfo.InvariantCulture.NumberFormat);

		public static implicit operator Matrix3x3 (Matrix4x4 a) =>
			new Matrix3x3 {
				m00 = a.m00, m01 = a.m01, m02 = a.m02,
				m10 = a.m10, m11 = a.m11, m12 = a.m12,
				m20 = a.m20, m21 = a.m21, m22 = a.m22
			};

		public static implicit operator Matrix4x4 (Matrix3x3 a) =>
			new Matrix4x4 {
				m00 = a.m00, m01 = a.m01, m02 = a.m02, m03 = 0.0f,
				m10 = a.m10, m11 = a.m11, m12 = a.m12, m13 = 0.0f,
				m20 = a.m20, m21 = a.m21, m22 = a.m22, m23 = 0.0f,
				m30 = 0.0f, m31 = 0.0f, m32 = 0.0f, m33 = 1.0f
			};

	}

}