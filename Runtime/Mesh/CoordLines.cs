using System;
using UnityEngine;

namespace UDlib.Mesh {

	public class CoordLines : Lines {

		public float length = 1.0f;
		private float _length;

		private void Start () {
			AppendSegment (new Vector3 (), Vector3.right * length, Color.red);
			AppendSegment (new Vector3 (), Vector3.up * length, Color.green);
			AppendSegment (new Vector3 (), Vector3.forward * length, Color.blue);
			UpdateMesh ();
			_length = length;
		}

		private void Update () {
			if (Math.Abs (length - _length) < 1e-5) return;
			Clear ();
			AppendSegment (new Vector3 (), Vector3.right * length, Color.red);
			AppendSegment (new Vector3 (), Vector3.up * length, Color.green);
			AppendSegment (new Vector3 (), Vector3.forward * length, Color.blue);
			UpdateMesh ();
			_length = length;
		}

	}

}