using Sirenix.OdinInspector;
using UDlib.Core.Image;
using UnityEngine;

namespace UDlib.Cam {

	[ExecuteAlways]
	public class CameraObject : MonoBehaviour {

		[OnValueChanged (nameof(_Inspector_OnCameraServiceChanged))]
		[DisableInPlayMode]
		public CameraService cameraService;
		[OnValueChanged (nameof(_Inspector_OnDepthOfImageChanged))]
		public float depthOfImage = 10;
		private Viewport _viewport;
		private MultiLayerImages _multiLayerImages;

		private void Awake () {
			_viewport = GetComponentInChildren<Viewport> ();
			_multiLayerImages = GetComponentInChildren<MultiLayerImages> ();
			_multiLayerImages[0] = cameraService == null ? null : new ImageChannel (cameraService);
			_viewport.cameraService = cameraService;
		}

		private void Update () {
			if (!Application.IsPlaying (this))
				return;
			_viewport.DepthToDraw = depthOfImage * 1.5f;
			var intrinsic = cameraService == null ? null : cameraService.Device?.intrinsic;
			if (intrinsic == null || !intrinsic.IsValid) {
				_multiLayerImages.transform.localPosition = new Vector3 (0.0f, 0.0f, depthOfImage);
				_multiLayerImages.size = Vector2.one * 10.0f;
			} else {
				var blRay = intrinsic.Projection.BottomLeftRay * depthOfImage;
				var trRay = intrinsic.Projection.TopRightRay * depthOfImage;
				_multiLayerImages.transform.localPosition = (blRay + trRay) / 2.0f;
				_multiLayerImages.size = trRay - blRay;
			}
		}

		private void _Inspector_OnCameraServiceChanged () {
			_multiLayerImages[0] = cameraService == null ? null : new ImageChannel (cameraService);
		}

		private void _Inspector_OnDepthOfImageChanged () {
			var p = _multiLayerImages.transform.position;
			p.z = depthOfImage;
			_multiLayerImages.transform.position = p;
		}

	}

}