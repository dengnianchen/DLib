using System.Runtime.InteropServices;
using UnityEngine;

namespace UDlib.Test.Scripts {

	public struct Struct3 {

		[MarshalAs(UnmanagedType.I1)]
		public bool a;
		public Vector2Int b;
		public Vector2 c;

	}

}