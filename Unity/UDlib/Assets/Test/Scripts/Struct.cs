using System.Runtime.InteropServices;
using UnityEngine;

namespace UDlib.Test.Scripts {

	[StructLayout (LayoutKind.Sequential)]
	public struct Struct {

		public enum ETest { TA, TB, TC }

		public ETest a;
		public Vector2Int b;

		[MarshalAs (UnmanagedType.ByValArray, SizeConst = 2, ArraySubType = UnmanagedType.R4)]
		public float[] c;

		public Struct (ETest a, Vector2Int b, float c1, float c2) {
			this.a = a;
			this.b = b;
			c = new[] { c1, c2 };
		}

	}

}