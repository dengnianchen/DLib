using System;
using UnityEngine;

namespace UDlib.Core.Image {

	// TODO Re-implement RoiControl
	[Obsolete]
	public class RoiControl : MonoBehaviour {

		public Rect roi = new Rect (0, 0, 1, 1);
		Vector2 pos1 = Vector2.zero;
		Vector2 pos2 = Vector2.one;

		void Start () { }

		void Update () {
			if (Input.GetMouseButtonDown (0)) {
				var pos = new Vector3 (Input.mousePosition.x, Input.mousePosition.y, 0.0f);
				if (Raycast (pos, out Vector3 p)) {
					pos1 = pos2 = new Vector2 (p.x + 0.5f, 0.5f - p.y);
				}
			} else if (Input.GetMouseButtonUp (0)) {
				if (Vector2.Distance (pos1, pos2) < 0.001f) {
					pos1 = Vector2.zero;
					pos2 = Vector2.one;
					UpdateRoi ();
				}
			} else if (Input.GetMouseButton (0)) {
				var pos = new Vector3 (Input.mousePosition.x, Input.mousePosition.y, 0.0f);
				if (Raycast (pos, out Vector3 p)) {
					pos2 = new Vector2 (p.x + 0.5f, 0.5f - p.y);
					UpdateRoi ();
				}
			}
		}

		void UpdateRoi () {
			roi.min = new Vector2 (Mathf.Min (pos1.x, pos2.x), Mathf.Min (pos1.y, pos2.y));
			roi.max = new Vector2 (Mathf.Max (pos1.x, pos2.x), Mathf.Max (pos1.y, pos2.y));
			GetComponent<Renderer> ().material.SetVector ("_Roi",
				new Vector4 (roi.min.x, roi.min.y, roi.max.x, roi.max.y));
		}

		bool Raycast (Vector3 pos, out Vector3 hitPosition) {
			hitPosition = Vector3.zero;
			var ray = Camera.main.ScreenPointToRay (pos);
			if (!GetComponent<Collider> ().Raycast (ray, out RaycastHit hitInfo, 200.0f))
				return false;
			hitPosition = transform.InverseTransformPoint (hitInfo.point);
			if (Mathf.Abs (hitPosition.x) > 0.5f || Mathf.Abs (hitPosition.y) > 0.5f)
				return false;
			return true;
		}

	}

}