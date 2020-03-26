using System;
using System.Text;
using UDlib.Core;
using UDlib.Cv;
using UnityEngine;

namespace UDlib.Test.Scripts {

	public class TestBehaviour : MonoBehaviour {

		private void Start () {
			TestCvStructure ();
			TestGlmStructure ();
			TestSelfStructure ();
			TestString ();
			TestException ();
			TestArrayAndMatrix ();
		}

		void TestCvStructure () {
			{
				var a = new Vector2Int (1, 2);
				var b = new Vector2Int (3, 4);
				var r = Plugin.cvStructureTest (ref a, ref b, out var c);
				Debug.LogFormat ("c = ({0}, {1}), return {2}", c.x, c.y, r);
			}
			{
				var a = new Vector3 (1, 2, 3);
				var b = new Vector3Int (4, 5, 6);
				var r = Plugin.cvStructureTest2 (a, b, out var c);
				Debug.LogFormat ("a.x = {4} c = ({0}, {1}, {2}) return {3}", c.x, c.y, c.z, r, a.x);
			}
		}

		void TestGlmStructure () {
			var a = new Vector2Int (1, 2);
			var b = new Vector3Int (3, 4, 5);
			var c = new Vector2 (6, 7);
			Plugin.glmStructureTest (a, b, c, out var d);
			Debug.LogFormat ("d = ({0}, {1}, {2})", d.x, d.y, d.z);
		}

		void TestSelfStructure () {
			{
				var a = new Struct (Struct.ETest.TA, new Vector2Int (1, 2), 3.0f, 4.0f);
				Plugin.structureTest (a, out var b);
				Debug.LogFormat ("b = ({0}, {1}, [ {2}, {3} ])", b.a, b.b, b.c[0], b.c[1]);
			}
			{
				var a = Plugin.structureTest2 ();
				Debug.LogFormat ("a = ({0}, {1})", a.a, a.b);
			}
			{
				var a = Plugin.structureTest3 ();
				Debug.LogFormat ("a = ({0}, {1}, {2})", a.a, a.b, a.c);
			}
		}

		void TestString () {
			{
				var str = "Hello C#!";
				var str2 = new StringBuilder (256);
				var str3 = Plugin.stringTest (str, str2, out var str4);
				Debug.LogFormat ("str2 = {0}, str3 = {1}, str4 = {2}", str2, str3, str4);
			}
			{
				var str = "你好 C#!";
				var str2 = new StringBuilder (256);
				var str3 = Plugin.stringTest2 (str, str2, out var str4);
				Debug.LogFormat ("str2 = {0}, str3 = {1}, str4 == null is {2}", str2, str3,
					str4 == null);
			}
		}

		void TestException () {
			try {
				Plugin.exceptionTest ();
			} catch (Exception e) {
				Debug.LogException (e);
			}
		}

		void TestArrayAndMatrix () {
			var arr1 = new Array<float> (3) { [0] = 1.0f, [1] = 2.0f, [2] = 3.0f };
			var arr2 = new Array<float> ();
			var arr3 = new Array<float> ();
			var mat1 = new Matrix<float> (2, 2) { [0] = 1.0f, [1] = 2.0f, [2] = 3.0f, [3] = 4.0f };
			var mat2 = new Matrix<float> ();
			var mat3 = new Matrix<float> ();
			Plugin.arrayAndMatrixTest (arr1, arr2, arr3, mat1, mat2, mat3);
			Debug.Log ($"arr1 = {arr1}");
			Debug.Log ($"arr2 = {arr2}");
			Debug.Log ($"arr3 = {arr3}");
			Debug.Log ($"mat1 = {mat1}");
			Debug.Log ($"mat2 = {mat2}");
			Debug.Log ($"mat3 = {mat3}");
		}

	}

}