using System;
using UnityEngine;

namespace UDlib.Core.Mathmatics {

	public class Poly3Curve : Curve {

		private readonly float[] c = new float[4];

		public Poly3Curve (float c0, float c1, float c2, float c3,
			float start, float end) : base (new Vector2 (start, end), float.NaN) {
			c[0] = c0;
			c[1] = c1;
			c[2] = c2;
			c[3] = c3;
		}

		public override Vector3 GetPointAt (float t) =>
			new Vector3 (c[0] + c[1] * t + c[2] * t * t + c[3] * t * t * t, 0.0f, t);

		public override float GetAngleAt (float t) =>
			Mathf.Rad2Deg * Mathf.Atan (3 * c[3] * t * t + 2 * c[2] * t + c[1]);

		public override float GetLengthAt (float t) => throw new NotImplementedException ();

	}

}