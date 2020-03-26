using System;
using UDlib.Core.Mathmatics;
using UnityEngine;

namespace UDlib.Cv {

	public class PerspectiveProjection {

		private readonly Matrix3x3 _m;
		private readonly Vector2 _size;

		public Vector2 Size => _size;
		public Vector2 Focal => new Vector2 (_m[0, 0], _m[1, 1]);
		public Vector2 Center => new Vector2 (_m[0, 2], _m[1, 2]);
		public Vector3 TopLeftRay => PixelToRay (Vector2.zero);
		public Vector3 TopRightRay => PixelToRay (new Vector2 (_size.x, 0));
		public Vector3 BottomLeftRay => PixelToRay (new Vector2 (0, _size.y));
		public Vector3 BottomRightRay => PixelToRay (_size);
		public Vector2 Fov {
			get {
				var tlRay = TopLeftRay;
				var brRay = BottomRightRay;
				return new Vector2 {
					x = Vector2.Angle (new Vector2 (tlRay.x, 1.0f), new Vector2 (brRay.x, 1.0f)),
					y = Vector2.Angle (new Vector2 (tlRay.y, 1.0f), new Vector2 (brRay.y, 1.0f))
				};
			}
		}

		public PerspectiveProjection (Matrix3x3 cvProjMatrix, Vector2 size = default) {
			_m = cvProjMatrix;
			_size = Math.Abs (size.Area ()) < 1e-5 ?
				(Vector2) (cvProjMatrix.GetColumn (2) * 2.0f) :
				size;
		}

		public PerspectiveProjection (Matrix4x4 unityProjMatrix, Vector2 size) {
			_m.m00 = 0.5f * size.x * unityProjMatrix.m00;
			_m.m02 = 0.5f * size.x * (1.0f - unityProjMatrix.m02);
			_m.m11 = -0.5f * size.y * unityProjMatrix.m11;
			_m.m12 = 0.5f * size.y * (1.0f + unityProjMatrix.m12);
			_size = size;
		}

		public Matrix4x4 GetUnityProjectionMatrix (float nearClipPlane, float farClipPlane) =>
			new Matrix4x4 {
				m00 = 2.0f * _m.m00 / _size.x,
				m02 = 1.0f - 2.0f * _m.m02 / _size.x,
				m11 = -2.0f * _m.m11 / _size.y,
				m12 = 2.0f * _m.m12 / _size.y - 1.0f,
				m22 = (nearClipPlane + farClipPlane) / (nearClipPlane - farClipPlane),
				m23 = 2.0f * nearClipPlane * farClipPlane / (nearClipPlane - farClipPlane),
				m32 = -1.0f
			};

		public static implicit operator Matrix3x3 (PerspectiveProjection pp) => pp._m;

		public Vector3 PixelToRay (Vector2 pixel) =>
			new Vector3 ((pixel.x - _m.m02) / _m.m00, (pixel.y - _m.m12) / _m.m11, 1.0f);

		public Vector2 RayToPixel (Vector3 ray) =>
			new Vector2 (_m.m02 + ray.x * _m.m00 / ray.z, _m.m12 + ray.y * _m.m11 / ray.z);

	}

}