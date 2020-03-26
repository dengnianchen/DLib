using System;
using UnityEngine;

namespace UDlib.Core.Mathmatics {

	[Serializable]
	public struct Vector3Byte : IEquatable<Vector3Byte> {

		public byte x;
		public byte y;
		public byte z;
		
		public Vector3Byte (int x, int y, int z) {
			this.x = (byte) x;
			this.y = (byte) y;
			this.z = (byte) z;
		}
		
		public byte[] Data () => new[] { x, y, z };
		
		public int Volumn () => x * y * z;

		public static Vector3Byte operator + (Vector3Byte a, Vector3Byte b) {
			return new Vector3Byte (a.x + b.x, a.y + b.y, a.z + b.z);
		}

		public static Vector3Byte operator - (Vector3Byte a, Vector3Byte b) {
			return new Vector3Byte (a.x - b.x, a.y - b.y, a.z - b.z);
		}

		public static Vector3Byte operator * (Vector3Byte a, Vector3Byte b) {
			return new Vector3Byte (a.x * b.x, a.y * b.y, a.z * b.z);
		}

		public static Vector3Byte operator / (Vector3Byte a, Vector3Byte b) {
			return new Vector3Byte (a.x / b.x, a.y / b.y, a.z / b.z);
		}

		public static implicit operator Vector3Int (Vector3Byte value) {
			return new Vector3Int (value.x, value.y, value.z);
		}

		public static implicit operator Vector3Byte (Vector3Int value) {
			return new Vector3Byte (value.x, value.y, value.z);
		}

		public static implicit operator Vector3 (Vector3Byte value) {
			return new Vector3 (value.x, value.y, value.z);
		}

		public static implicit operator Vector3Byte (Vector3 value) {
			return new Vector3Byte ((int) value.x, (int) value.y, (int) value.z);
		}

		public static implicit operator Color32 (Vector3Byte value) {
			return new Color32 (value.x, value.y, value.z, 255);
		}

		public static implicit operator Vector3Byte (Color32 value) {
			return new Vector3Byte (value.r, value.g, value.b);
		}
		
		public static implicit operator Color (Vector3Byte value) {
			return new Color (value.x / 255.0f, value.y / 255.0f, value.z / 255.0f, 1.0f);
		}
		
		public static implicit operator Vector3Byte (Color value) {
			return new Vector3Byte ((int) (value.r * 255), (int) (value.g * 255),
				(int) (value.b * 255));
		}

		public bool Equals (Vector3Byte other) {
			return x == other.x && y == other.y && z == other.z;
		}

	}

}