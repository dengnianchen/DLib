using System;
using System.Collections.Generic;
using System.Linq;
using UDlib.Core.Mathmatics;
using UnityEngine;

namespace UDlib.Mesh {

	[ExecuteAlways]
	public class GridMesh : ScriptMesh {

		private bool _dynamicMesh;

		public void UpdateVertices (Vector3[] vertices = null, Vector2[] uvs = null,
			Color[] colors = null) {
			if (!MeshObject)
				throw new InvalidOperationException (
					"Mesh is not generated yet. Call GenerateMesh() first.");
			if (vertices != null)
				MeshObject.vertices = vertices;
			if (uvs != null)
				MeshObject.uv = uvs;
			if (colors != null)
				MeshObject.colors = colors;
			if (vertices != null) {
				MeshObject.RecalculateBounds ();
				MeshObject.RecalculateNormals ();
				MeshObject.RecalculateTangents ();
			}
			if (vertices != null || uvs != null || colors != null)
				MeshObject.UploadMeshData (!_dynamicMesh);
		}

		public void TransformVertices (Matrix4x4 t) {
			if (!_dynamicMesh)
				throw new InvalidOperationException("Mesh is not generated in dynamic mode");
			if (!MeshObject)
				throw new InvalidOperationException (
					"Mesh is not generated yet. Call GenerateMesh() first.");
			UpdateVertices (MeshObject.vertices.Select (t.MultiplyPoint).ToArray ());
		}
		
		public void GenerateMesh (Vector2Int gridSize, Vector3[] vertices = null,
			Vector2[] uvs = null, Color[] colors = null, Vector2 quadSize = default,
			bool dynamicMesh = false) {
			if (gridSize.x <= 1 || gridSize.y <= 1)
				throw new ArgumentException ("Grid size must be larger than 1", nameof(gridSize));
			var mesh = new UnityEngine.Mesh ();
			if (dynamicMesh)
				mesh.MarkDynamic ();

			// 使用指定的顶点数组或生成默认顶点数组（大小为quadSize的矩形网格）
			if (vertices != null)
				mesh.vertices = vertices;
			else {
				// 若未指定quadSize，则默认为单位正方形
				if (Math.Abs (quadSize.Area ()) < 1e-5f)
					quadSize = Vector2.one;
				vertices = new Vector3[gridSize.Area ()];
				for (int r = 0, i = 0; r < gridSize.y; ++r)
				for (var c = 0; c < gridSize.x; ++c, ++i) {
					vertices[i] = new Vector3 (quadSize.x * c / (gridSize.x - 1),
						quadSize.y * r / (gridSize.y - 1), 0.0f);
				}
				mesh.vertices = vertices;
			}

			// 使用指定的UV数组或生成默认UV数组（均匀坐标）
			if (uvs != null)
				mesh.uv = uvs;
			else {
				uvs = new Vector2[gridSize.Area ()];
				for (int r = 0, i = 0; r < gridSize.y; ++r)
				for (var c = 0; c < gridSize.x; ++c, ++i) {
					uvs[i] = new Vector2 ((float) c / (gridSize.x - 1),
						(float) r / (gridSize.y - 1));
				}
				mesh.uv = uvs;
			}

			// 使用指定的颜色数组或生成默认颜色数组（白色）
			if (colors != null)
				mesh.colors = colors;
			else {
				colors = new Color[gridSize.Area ()];
				for (var i = 0; i < colors.Length; ++i)
					colors[i] = Color.white;
				mesh.colors = colors;
			}
			mesh.triangles = _GenerateTriangleList (gridSize).ToArray ();
			mesh.SetTriangles (_GenerateTriangleList (gridSize), 0);
			mesh.RecalculateNormals ();
			mesh.RecalculateTangents ();
			mesh.UploadMeshData (!dynamicMesh);
			MeshObject = mesh;
			_dynamicMesh = dynamicMesh;
		}

		private static List<int> _GenerateTriangleList (Vector2Int gridSize) {
			// 生成三角面片
			var triangleList = new List<int> ();
			for (var r = 0; r < gridSize.y - 1; ++r)
			for (var c = 0; c < gridSize.x - 1; ++c) {
				var quad = new[] {
					r * gridSize.x + c,
					r * gridSize.x + c + 1,
					(r + 1) * gridSize.x + c + 1,
					(r + 1) * gridSize.x + c
				};
				triangleList.Add (quad[0]);
				triangleList.Add (quad[2]);
				triangleList.Add (quad[1]);
				triangleList.Add (quad[0]);
				triangleList.Add (quad[3]);
				triangleList.Add (quad[2]);
			}
			return triangleList;
		}

	}

}