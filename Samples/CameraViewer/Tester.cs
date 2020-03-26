using UDlib.App;
using UDlib.Cam;
using UnityEngine;
using UnityEngine.UI;

namespace UDlib.Samples.CameraViewer {

	public class Tester : MonoBehaviour {

		public Text uiTimestampText;
		private CameraService _cameraService;

		private void Start () {
			_cameraService = CameraService.Get ();
			_cameraService.FrameUpdated.Bind (frame => {
				uiTimestampText.text = $"Timestamp: {frame.Timestamp}";
			});
			if (_cameraService.CurrentState == ServiceState.Running) {
				var range = _cameraService.Device.GetPropertyRange (Property.EType.FrameRate);
				Debug.Log (
					$"Frame rate range is {range.low} - {range.high}");
			}
		}

	}

}