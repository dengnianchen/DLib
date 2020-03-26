using System.Runtime.InteropServices;
using System.Text;
using UDlib.Core;
using UDlib.Cv;
using UnityEngine;

namespace UDlib.Test.Scripts {

	internal static class Plugin {

		[DllImport ("UDlib_Test")]
		public static extern bool cvStructureTest (ref Vector2Int a, ref Vector2Int b,
			out Vector2Int o_c);

		[DllImport ("UDlib_Test")]
		public static extern bool cvStructureTest2 (Vector3 a, Vector3Int b, out Vector3 o_c);

		[DllImport ("UDlib_Test")]
		public static extern void glmStructureTest (Vector2Int a, Vector3Int b, Vector2 c,
			out Vector3 o_d);

		[DllImport ("UDlib_Test")]
		public static extern void structureTest (Struct a, out Struct o_b);

		[DllImport ("UDlib_Test")]
		public static extern Struct2 structureTest2 ();

		[DllImport ("UDlib_Test")]
		public static extern Struct3 structureTest3 ();

		[DllImport ("UDlib_Test")]
		public static extern ETest enumTest (ETest a);

		[DllImport ("UDlib_Test")]
		public static extern string stringTest (string str, StringBuilder o_str, out string o_str2);

		[DllImport ("UDlib_Test", CharSet = CharSet.Unicode)]
		public static extern string
			stringTest2 (string str, StringBuilder o_str, out string o_str2);

		[DllImport ("UDlib_Test")]
		public static extern void exceptionTest ();

		[DllImport ("UDlib_Test")]
		public static extern void exceptionTest2 ();

		[DllImport ("UDlib_Test")]
		public static extern void arrayAndMatrixTest (
			Array<float> arr1, Array<float> arr2, [Out] Array<float> arr3,
			Matrix<float> mat1, Matrix<float> mat2, [Out] Matrix<float> mat3);

	}

}