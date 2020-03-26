using System;
using UDlib.Core;
using UDlib.Core.Image;
using UDlib.Core.Mathmatics;
using UDlib.Cv;
using UnityEngine;

namespace UDlib.Cam {

	public class Intrinsic : CObject {

		public readonly Device device;

		public PerspectiveProjection Projection { get; private set; }
		public Array<float> DistortionCeofs { get; private set; }
		public bool IsValid { get; private set; }
		public Vector2 Focal => Projection.Focal;
		public Vector2 Center => Projection.Center;
		public Vector2 Fov => Projection.Fov;

		public void UndistortImage (TexImage image, TexImage outUndistortedImage) =>
			Plugin.Intrinsic_undistortImage (this, image, outUndistortedImage);

		public Array<Vector2> UndistortPoints (Array<Vector2> inPoints) {
			var outPoints = new Array<Vector2> (inPoints.Length);
			Plugin.Intrinsic_undistortPoints (this, inPoints, outPoints);
			return outPoints;
		}

		internal Intrinsic (Device device, IntPtr pointer) : base (pointer) {
			this.device = device;
			this.Projection = new PerspectiveProjection (Matrix3x3.identity, device.FrameSize);
		}

		internal void _LoadData () {
			var matrix = new Matrix<float> (3, 3);
			DistortionCeofs = new Array<float> (5);
			IsValid = Plugin.Intrinsic_getData (this, matrix, DistortionCeofs);
			matrix.Get (out Matrix3x3 matrix1);
			Projection = new PerspectiveProjection (matrix1, device.FrameSize);
		}

		internal void _UpdateSize () =>
			Projection = new PerspectiveProjection (Projection, device.FrameSize);

		protected override void Dispose (bool disposing) { }

	}

}