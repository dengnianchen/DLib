using System;
using UnityEngine;

namespace UDlib.Core.Mathmatics {

	[Serializable]
	public struct Vector4Byte : IEquatable<Vector4Byte> {

		public byte x;
		public byte y;
		public byte z;
		public byte w;
		
		public Vector4Byte (int x, int y, int z, int w) {
			this.x = (byte) x;
			this.y = (byte) y;
			this.z = (byte) z;
			this.w = (byte) w;
		}

		public byte[] Data () => new[] { x, y, z, w };
		
		public int Volumn () => x * y * z * w;

		public static Vector4Byte operator + (Vector4Byte a, Vector4Byte b) {
			return new Vector4Byte (a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
		}
		
		public static Vector4Byte operator - (Vector4Byte a, Vector4Byte b) {
			return new Vector4Byte (a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
		}

		public static Vector4Byte operator * (Vector4Byte a, Vector4Byte b) {
			return new Vector4Byte (a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
		}

		public static Vector4Byte operator / (Vector4Byte a, Vector4Byte b) {
			return new Vector4Byte (a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w);
		}

		public static implicit operator Color32 (Vector4Byte value) {
			return new Color32 (value.x, value.y, value.z, value.w);
		}

		public static implicit operator Vector4Byte (Color32 value) {
			return new Vector4Byte (value.r, value.g, value.b, value.a);
		}

		public static implicit operator Color (Vector4Byte value) {
			return new Color (value.x / 255.0f, value.y / 255.0f,
				value.z / 255.0f, value.w / 255.0f);
		}
		
		public static implicit operator Vector4Byte (Color value) {
			return new Vector4Byte ((int) (value.r * 255), (int) (value.g * 255),
				(int) (value.b * 255), (int) (value.a * 255));
		}

		public bool Equals (Vector4Byte other) {
			return x == other.x && y == other.y && z == other.z && w == other.w;
		}

	}

}