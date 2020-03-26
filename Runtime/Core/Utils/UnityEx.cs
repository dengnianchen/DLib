using System;
using JetBrains.Annotations;
using UDlib.Core.Mathmatics;
using UnityEngine;

namespace UDlib.Core.Utils {

	public static class UnityEx {

		public static T Find<T> ([NotNull] this Transform t, string n) where T : Component =>
			t.Find (n)?.GetComponent<T> ();

		public static void SetLocalToWorldMatrix ([NotNull] this Transform t, Matrix4x4 m) =>
			SetLocalMatrix (t, t.parent == null ? m : t.parent.worldToLocalMatrix * m);

		public static void SetLocalMatrix ([NotNull] this Transform t, Matrix4x4 m) {
			t.localRotation = m.rotation;
			t.localPosition = m.GetColumn (3);
			t.localScale = m.lossyScale;
		}

		public static void SetLocalToWorldRigid ([NotNull] this Transform t, RigidTransform r) {
			t.position = r.Position;
			t.rotation = r.Rotation;
		}

		public static void SetLocalRigid ([NotNull] this Transform t, RigidTransform r) {
			t.localPosition = r.Position;
			t.localRotation = r.Rotation;
		}

		public static Matrix4x4 GetLocalMatrix ([NotNull] this Transform t) =>
			t.parent == null ?
				t.localToWorldMatrix :
				t.parent.worldToLocalMatrix * t.localToWorldMatrix;

		public static RigidTransform GetLocalRigid ([NotNull] this Transform t) =>
			new RigidTransform (t.localPosition, t.localRotation);

		public static RigidTransform GetLocalToWorldRigid ([NotNull] this Transform t) =>
			new RigidTransform (t.position, t.rotation);

		public static Vector3
			TransformVector ([NotNull] this Transform t, Vector3 v, Transform to) =>
			to == null ?
				t.TransformVector (v) :
				to.InverseTransformVector (t.TransformVector (v));

		public static Vector3
			TransformPoint ([NotNull] this Transform t, Vector3 p, Transform to) =>
			to == null ?
				t.TransformPoint (p) :
				to.InverseTransformPoint (t.TransformPoint (p));

		public static void SetProjection ([NotNull] this Camera c, Vector3 blRay, Vector3 trRay) {
			blRay /= blRay.z;
			trRay /= trRay.z;
			var nearClipPlane = c.nearClipPlane;
			var left = blRay.x * nearClipPlane;
			var top = trRay.y * nearClipPlane;
			var right = trRay.x * nearClipPlane;
			var bottom = blRay.y * nearClipPlane;
			var near = nearClipPlane;
			var far = c.farClipPlane;
			var a = -(far + near) / (far - near);
			var b = -2.0f * far * near / (far - near);
			var projection = Matrix4x4.zero;
			projection.SetRow (0,
				new Vector4 (2 * near / (right - left), 0, (right + left) / (right - left), 0));
			projection.SetRow (1,
				new Vector4 (0, 2 * near / (top - bottom), (top + bottom) / (top - bottom), 0));
			projection.SetRow (2, new Vector4 (0, 0, a, b));
			projection.SetRow (3, new Vector4 (0, 0, -1, 0));
			c.projectionMatrix = projection;
		}

		public static bool DeepEquals (this Array a, Array b) {
			if (a.Length != b.Length) return false;
			var i = 0;
			for (; i < a.Length; ++i)
				if (!a.GetValue (i).Equals (b.GetValue (i)))
					break;
			return i == a.Length;
		}

		public static Vector2Int GetSize (this Texture a) => new Vector2Int(a.width, a.height);

	}

}