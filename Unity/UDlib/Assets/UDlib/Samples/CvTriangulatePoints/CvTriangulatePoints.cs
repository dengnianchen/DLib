using System.Collections;
using System.Collections.Generic;
using UDlib.Core;
using UDlib.Core.Utils;
using UDlib.Cv;
using UnityEngine;

namespace UDlib.Samples.CvTriangulatePoints {

	public class CvTriangulatePoints : MonoBehaviour {

		public Camera camera1;
		public Camera camera2;
		public GameObject refObject;
		public GameObject resultObject;

		// Start is called before the first frame update
		void Start () {
		}

		// Update is called once per frame
		void Update () {
			var m1 = camera1.transform.worldToLocalMatrix;
			var projM1 = new Matrix<float> (3, 4);
			projM1.Set (m1);
			var projM2 = new Matrix<float> (3, 4);
			var m2 = camera2.transform.worldToLocalMatrix;
			projM2.Set (m2);
			var p1 = new Array<Vector2> (1);
			var p1_3d = camera1.transform.InverseTransformPoint (refObject.transform.position);
			p1[0] = p1_3d / p1_3d.z;
			var p2 = new Array<Vector2> (1);
			var p2_3d = camera2.transform.InverseTransformPoint (refObject.transform.position);
			p2[0] = p2_3d / p2_3d.z;
			var p = Calib3D.TriangulatePoints (projM1, projM2, p1, p2, out var error1,
				out var error2);
			resultObject.transform.position = p[0];
			Debug.Log ($"error1 = {error1}, error2 = {error2}");
		}

	}

}