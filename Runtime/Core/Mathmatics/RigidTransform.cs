using UnityEngine;

namespace UDlib.Core.Mathmatics {

	public class RigidTransform {

		public Vector3 Position { get; }
		public Vector3 InversePosition { get; }
		public Quaternion Rotation { get; }
		public Quaternion InverseRotation { get; }

		public RigidTransform () {
			Position = InversePosition = Vector3.zero;
			Rotation = InverseRotation = Quaternion.identity;
		}

		public RigidTransform (Vector3 position, Quaternion rotation) {
			Position = position;
			Rotation = rotation;
			InverseRotation = Quaternion.Inverse (rotation);
			InversePosition = InverseRotation * -Position;
		}

		public RigidTransform (Matrix4x4 m) {
			Rotation = m.rotation;
			Position = m.GetColumn (3);
			InverseRotation = Quaternion.Inverse (Rotation);
			InversePosition = InverseRotation * -Position;
		}
		
		public RigidTransform (Transform unityTransform, bool isLocal) {
			if (isLocal) {
				Position = unityTransform.localPosition;
				Rotation = unityTransform.localRotation;
			} else {
				Position = unityTransform.position;
				Rotation = unityTransform.rotation;
			}
			InverseRotation = Quaternion.Inverse (Rotation);
			InversePosition = InverseRotation * -Position;
		}
		
		public RigidTransform GetInverse () => new RigidTransform (InversePosition, InverseRotation);

		public Vector3 TransformPoint (Vector3 p) => Rotation * p + Position;

		public Vector3 InverseTransformPoint (Vector3 p) => InverseRotation * p + InversePosition;

		public Vector3 TransformVector (Vector3 v) => Rotation * v;

		public Vector3 InverseTransformVector (Vector3 v) => InverseRotation * v;

		public static implicit operator Matrix4x4 (RigidTransform t) =>
			Matrix4x4.TRS (t.Position, t.Rotation, Vector3.one);

		public static RigidTransform operator * (RigidTransform lhs, RigidTransform rhs) =>
			new RigidTransform (lhs.TransformPoint (rhs.Position), lhs.Rotation * rhs.Rotation);

	}

}