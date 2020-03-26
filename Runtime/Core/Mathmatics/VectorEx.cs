using UnityEngine;

namespace UDlib.Core.Mathmatics {

	public static class VectorEx {

		public static int[] Data (this Vector2Int a) => new[] { a.x, a.y };

		public static int[] Data (this Vector3Int a) => new[] { a.x, a.y, a.z };

		public static float[] Data (this Vector2 a) => new[] { a.x, a.y };

		public static float[] Data (this Vector3 a) => new[] { a.x, a.y, a.z };

		public static float[] Data (this Vector4 a) => new[] { a.x, a.y, a.z, a.w };

		public static int Area (this Vector2Int a) => a.x * a.y;

		public static float Area (this Vector2 a) => a.x * a.y;

		public static int Volumn (this Vector3Int a) => a.x * a.y * a.z;

		public static float Volumn (this Vector3 a) => a.x * a.y * a.z;

	}

}