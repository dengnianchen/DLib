using System;
using UDlib.Core.Mathmatics;
using UnityEngine;

namespace UDlib.Mesh {

	public class QuadMesh : GridMesh {

		public Vector2 size = Vector2.one;
		public Vector2Int gridSize = new Vector2Int (2, 2);

		private Vector2 _size;
		private Vector2Int _gridSize;

		private void Start () {
			GenerateMesh (gridSize, quadSize: size, dynamicMesh: true);
			_size = size;
			_gridSize = gridSize;
		}

		private void Update () {
			if (_gridSize != gridSize)
				GenerateMesh (gridSize, quadSize: size, dynamicMesh: true);
			else if (_size != size) {
				if (Math.Abs (_size.Area ()) < 1e-5f)
					GenerateMesh (gridSize, quadSize: size, dynamicMesh: true);
				else
					TransformVertices (Matrix4x4.Scale (
						new Vector3 (size.x / _size.x, size.y / _size.y, 1.0f)));
			}
			_size = size;
			_gridSize = gridSize;
		}

	}

}