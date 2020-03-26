using System;
using System.Collections.Generic;
using UnityEngine;

namespace UDlib.Mesh {

	[ExecuteAlways]
	public class Lines : ScriptMesh {

		private Material _material;
		private readonly List<Vector3> _points = new List<Vector3> ();
		private readonly List<Color> _colors = new List<Color> ();
		private readonly List<int> _indices = new List<int> ();

		public void AppendSegment (Vector3 startPoint, Vector3 endPoint, Color color) =>
			AppendSegment (startPoint, endPoint, color, color);

		public void AppendSegment (Vector3 startPoint, Vector3 endPoint, Color startColor,
			Color endColor) {
			_points.Add (startPoint);
			_points.Add (endPoint);
			_colors.Add (startColor);
			_colors.Add (endColor);
			_indices.Add (_points.Count - 2);
			_indices.Add (_points.Count - 1);
		}

		public void AppendSequence (Vector3[] points, Color[] colors,
			bool connectWithExisted = false) {
			if (points.Length == 0)
				throw new ArgumentException ("Must have at least one point", nameof(points));
			if (points.Length != colors.Length)
				throw new ArgumentException ("colors must be same length as points",
					nameof(colors));

			var pOffset = points.Length;

			_points.AddRange (points);
			_colors.AddRange (colors);

			// 连接前序终点和当前序列的第一个点
			if (connectWithExisted && pOffset > 0) {
				_indices.Add (pOffset - 1);
				_indices.Add (pOffset);
			}

			// 处理孤立点的情况
			if (points.Length == 1 && !connectWithExisted) {
				_indices.Add (pOffset);
				_indices.Add (pOffset);
			}

			// 将当前序列的点依次相连
			for (var i = 0; i < _points.Count - 1; ++i) {
				_indices.Add (pOffset);
				_indices.Add (pOffset + 1);
			}
		}

		public void UpdateMesh () {
			if (!MeshObject)
				MeshObject = new UnityEngine.Mesh ();
			MeshObject.vertices = _points.ToArray ();
			MeshObject.colors = _colors.ToArray ();
			MeshObject.SetIndices (_indices, MeshTopology.Lines, 0);
			MeshObject.UploadMeshData (false);
		}

		public void Clear () {
			_points.Clear ();
			_colors.Clear ();
			_indices.Clear ();
			MeshObject = null;
		}

		protected override void Awake () {
			base.Awake ();
			var shader = Resources.Load<Shader> ("UDlib_Mesh_LineShader");
			GetComponent<MeshRenderer> ().material = new Material (shader);
		}

	}

}