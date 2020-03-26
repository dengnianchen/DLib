using UnityEngine;

namespace UDlib.Core.Mathmatics {

	public abstract class Curve {

		public readonly Vector2 range;
		public readonly float length;

		public bool IsValid => range.x < range.y;

		public Vector2 Range => range;

		public float RangeLength => range.y - range.x;
		
		protected Curve (Vector2 range, float length) {
			this.range = range;
			this.length = length;
		}

		public bool IsInRange (float t) => t >= range.x && t <= range.y;

		public abstract Vector3 GetPointAt (float t);
		public abstract float GetAngleAt (float t);
		public abstract float GetLengthAt (float t);

	}

}