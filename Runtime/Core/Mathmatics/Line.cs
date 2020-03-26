using UnityEngine;

namespace UDlib.Core.Mathmatics {

	public class Line {

		public readonly Vector3 start;
		public readonly Vector3 end;
		public readonly float length;
		public readonly Quaternion direction;

		public Line (Vector3 start, Vector3 end) {
			this.start = start;
			this.end = end;
			length = Vector3.Distance (this.start, this.end);
			direction = Mathf.Approximately (length, 0.0f) ?
				Quaternion.identity :
				Quaternion.LookRotation (this.end - this.start);
		}

		public Vector3 GetPoint (float d) => (end - start) / length * d + start;

		public Vector3 GetNearestPoint (Vector3 p, bool clamp) =>
			GetNearestPoint (p, clamp, out _);

		public Vector3 GetNearestPoint (Vector3 p, bool clamp, out float d) {
			if (Mathf.Approximately (length, 0.0f)) {
				if (clamp) {
					d = Vector3.Distance (p, start);
					return start;
				}
				d = 0.0f;
				return p;
			}
			var v = (end - start) / length;
			var projLength = Vector3.Dot (v, p - start);
			if (clamp) {
				if (projLength < 0.0f) projLength = 0.0f;
				if (projLength > length) projLength = length;
			}
			var pOnLine = start + projLength * v;
			d = Vector3.Distance (pOnLine, p);
			return pOnLine;
		}

		public float GetDistance (Vector3 p, bool clamp) {
			GetNearestPoint (p, clamp, out var d);
			return d;
		}

		public static bool GetIntersection (Line line1, Line line2, out Vector3 point) {
			var o1 = line1.start;
			var q1 = Quaternion.LookRotation (line1.end - line1.start,
				Vector3.Cross (line1.end - line1.start, line2.end - line2.start));
			var q1Inv = Quaternion.Inverse (q1);
			var start2 = q1Inv * (line2.start - o1);
			var end2 = q1Inv * (line2.end - o1);

			if (Mathf.Abs (start2.z) < 10e-3f) {
				if (Mathf.Abs (start2.x - end2.x) < 10e-5f) {
					point = new Vector3 (float.NaN, float.NaN, float.NaN);
					return false;
				}
				point = o1 + q1 * new Vector3 (0.0f,
					        start2.y + start2.x * (end2.y - start2.y) / (start2.x - end2.x), 0.0f);
				return true;
			}
			point = new Vector3 (float.NaN, float.NaN, float.NaN);
			return false;
		}

	}

}