using System;
using UDlib.Mesh;
using UDlib.Cv;
using UnityEngine;

namespace UDlib.Cam {

	public class Viewport : Lines {

		internal CameraService cameraService;
		private PerspectiveProjection _proj;
		private float _depthToDraw;

		internal float DepthToDraw {
			set {
				if (Math.Abs (_depthToDraw - value) < 1e-5)
					return;
				_depthToDraw = value;
				_UpdateLines ();
			}
		}

		private void Update () {
			var intrinsic = cameraService == null ? null : cameraService.Device?.intrinsic;
			var curProj = intrinsic == null || !intrinsic.IsValid ? null : intrinsic.Projection;
			if (_proj == curProj)
				return;
			_proj = curProj;
			_UpdateLines ();
		}

		private void _UpdateLines () {
			Clear ();
			if (_proj == null)
				return;

			Vector3[] borderRays = {
				_proj.TopLeftRay,
				_proj.TopRightRay,
				_proj.BottomRightRay,
				_proj.BottomLeftRay
			};
			Color[] borderColors = {
				Color.gray, Color.red, Color.yellow, Color.green
			};
			for (var i = 0; i < 4; ++i)
				AppendSegment (Vector3.zero, borderRays[i] * _depthToDraw, borderColors[i],
					new Color (borderColors[i].r, borderColors[i].g, borderColors[i].b, 0.0f));
			for (var i = 0; i < 4; ++i) {
				var start = borderRays[i];
				var end = borderRays[(i + 1) % 4];
				AppendSegment (start, end, borderColors[i], borderColors[(i + 1) % 4]);
			}
			UpdateMesh ();
		}

	}

}