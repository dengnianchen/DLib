using System.IO;
using UDlib.Core.Mathmatics;
using UnityEngine;

namespace UDlib.Core.Utils {

	public static class BinaryIOEx {

		public static Vector2 ReadVector2 (this BinaryReader reader) {
			return new Vector2 (
				reader.ReadSingle (),
				reader.ReadSingle ());
		}

		public static Vector3 ReadVector3 (this BinaryReader reader) {
			return new Vector3 (
				reader.ReadSingle (),
				reader.ReadSingle (),
				reader.ReadSingle ());
		}

		public static Rect ReadRect (this BinaryReader reader) {
			return new Rect (
				reader.ReadSingle (),
				reader.ReadSingle (),
				reader.ReadSingle (),
				reader.ReadSingle ());
		}

		public static Quaternion ReadQuaternion (this BinaryReader reader) {
			return new Quaternion (
				reader.ReadSingle (),
				reader.ReadSingle (),
				reader.ReadSingle (),
				reader.ReadSingle ());
		}

		public static Matrix4x4 ReadMatrix4 (this BinaryReader reader) {
			Matrix4x4 m;
			m.m00 = reader.ReadSingle ();
			m.m01 = reader.ReadSingle ();
			m.m02 = reader.ReadSingle ();
			m.m03 = reader.ReadSingle ();
			m.m10 = reader.ReadSingle ();
			m.m11 = reader.ReadSingle ();
			m.m12 = reader.ReadSingle ();
			m.m13 = reader.ReadSingle ();
			m.m20 = reader.ReadSingle ();
			m.m21 = reader.ReadSingle ();
			m.m22 = reader.ReadSingle ();
			m.m23 = reader.ReadSingle ();
			m.m30 = reader.ReadSingle ();
			m.m31 = reader.ReadSingle ();
			m.m32 = reader.ReadSingle ();
			m.m33 = reader.ReadSingle ();
			return m;
		}

		public static Vector2Int ReadVector2i (this BinaryReader reader) =>
			new Vector2Int (reader.ReadInt32 (), reader.ReadInt32 ());

		public static RigidTransform ReadRigidTransform (this BinaryReader reader) =>
			new RigidTransform (reader.ReadVector3 (), reader.ReadQuaternion ());

		public static void Write (this BinaryWriter writer, Vector2 v) {
			writer.Write (v.x);
			writer.Write (v.y);
		}

		public static void Write (this BinaryWriter writer, Vector3 v) {
			writer.Write (v.x);
			writer.Write (v.y);
			writer.Write (v.z);
		}

		public static void Write (this BinaryWriter writer, Rect r) {
			writer.Write (r.xMin);
			writer.Write (r.yMin);
			writer.Write (r.width);
			writer.Write (r.height);
		}

		public static void Write (this BinaryWriter writer, Quaternion q) {
			writer.Write (q.x);
			writer.Write (q.y);
			writer.Write (q.z);
			writer.Write (q.w);
		}

		public static void Write (this BinaryWriter writer, Matrix4x4 m) {
			writer.Write (m.m00);
			writer.Write (m.m01);
			writer.Write (m.m02);
			writer.Write (m.m03);
			writer.Write (m.m10);
			writer.Write (m.m11);
			writer.Write (m.m12);
			writer.Write (m.m13);
			writer.Write (m.m20);
			writer.Write (m.m21);
			writer.Write (m.m22);
			writer.Write (m.m23);
			writer.Write (m.m30);
			writer.Write (m.m31);
			writer.Write (m.m32);
			writer.Write (m.m33);
		}

		public static void Write (this BinaryWriter writer, Vector2Int v) {
			writer.Write (v.x);
			writer.Write (v.y);
		}

		public static void Write (this BinaryWriter writer, RigidTransform r) {
			writer.Write (r.Position);
			writer.Write (r.Rotation);
		}

	}

}