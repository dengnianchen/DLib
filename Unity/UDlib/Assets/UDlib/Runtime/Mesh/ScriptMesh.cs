using UnityEngine;

namespace UDlib.Mesh {

	[RequireComponent (typeof(MeshFilter), typeof(MeshRenderer))]
	public class ScriptMesh : MonoBehaviour {

		private MeshFilter _meshFilter;
		
		public UnityEngine.Mesh MeshObject {
			get => _meshFilter.sharedMesh;
			set => _meshFilter.sharedMesh = value;
		}

		protected virtual void Awake () {
			_meshFilter = GetComponent<MeshFilter> ();
		}

	}

}