using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using System.Text;
using Sirenix.OdinInspector;
using UDlib.App;
using UDlib.App.Internal;
using UDlib.Core;
using UDlib.Core.Image;
using UDlib.Core.Mathmatics;
using UDlib.Core.Utils;
using Unity.Collections.LowLevel.Unsafe;
using UnityEditor.PackageManager;
using UnityEngine;

namespace UDlib.Cam {

	public class CameraService : Service<CameraService>, IImageSource {

		[Serializable]
		public struct PropertyConfig {

			[HideLabel, HorizontalGroup ("g1", 120), DisplayAsString]
			public Property.EType type;
			[LabelText ("Auto"), LabelWidth (5), ToggleLeft, HorizontalGroup ("g1")]
			public bool isAuto;
			[Delayed, HideLabel, DisableIf (nameof(isAuto)), HorizontalGroup ("g1")]
			public float value;

			public PropertyConfig (Property.EType type, Property property) {
				this.type = type;
				this.isAuto = property.isAuto;
				this.value = property.value;
			}

			public static implicit operator Property (PropertyConfig pc) =>
				new Property (pc.isAuto, pc.value);

		}

		public Event<Frame> FrameUpdated = new Event<Frame> ();

		[Delayed, EnableIf (nameof(CurrentState), ServiceState.Stopped)]
		public int deviceId;
		[EnableIf (nameof(CurrentState), ServiceState.Stopped)]
		public string deviceType;
		[EnableIf (nameof(CurrentState), ServiceState.Stopped)]
		public Vector2Int frameSize;
		[EnableIf (nameof(CurrentState), ServiceState.Stopped)]
		public string intrinsicFile;
		[PropertySpace (SpaceAfter = 8)]
		public bool doUndistortion;
		[ShowIf (nameof(CurrentState), ServiceState.Stopped)]
		[ValueDropdown (nameof(_Inspector_GetPropertyConfigCandidates),
			IsUniqueList = true,
			DrawDropdownForListElements = false)]
		public List<PropertyConfig> properties;

		[HideIfGroup (nameof(CurrentState), ServiceState.Stopped)]
		[BoxGroup ("CurrentState/Runtime", GroupName = "Runtime Properties")]
		[ShowInInspector, HideLabel]
		public PropertyConfig RuntimeFrameRate {
			get =>
				new PropertyConfig (Property.EType.FrameRate,
					(Device?.GetPropertyValue (Property.EType.FrameRate)).GetValueOrDefault ());
			set => Device?.SetPropertyValue (Property.EType.FrameRate, value);
		}
		[ShowInInspector, HideLabel, BoxGroup ("CurrentState/Runtime")]
		public PropertyConfig RuntimeExposureTime {
			get =>
				new PropertyConfig (Property.EType.ExposureTime,
					(Device?.GetPropertyValue (Property.EType.ExposureTime)).GetValueOrDefault ());
			set => Device?.SetPropertyValue (Property.EType.ExposureTime, value);
		}
		[ShowInInspector, HideLabel, BoxGroup ("CurrentState/Runtime")]
		public PropertyConfig RuntimeGain {
			get =>
				new PropertyConfig (Property.EType.Gain,
					(Device?.GetPropertyValue (Property.EType.Gain)).GetValueOrDefault ());
			set => Device?.SetPropertyValue (Property.EType.Gain, value);
		}

		public Device Device { get; private set; }

		private void Awake () => this.AddPreviewComponent ();

		protected override void _Startup () {
			Device = new Device (deviceType, deviceId);
			try {
				foreach (var propertyConfig in properties)
					Device.SetPropertyValue (propertyConfig.type, propertyConfig);
				if (frameSize.Area () != 0)
					Device.FrameSize = frameSize;
				Device.Start ();
			} catch (Exception) {
				Device.Dispose ();
				Device = null;
				throw;
			}
			try {
				Device.LoadIntrinsic ($"{Framework.PersistencePath}{intrinsicFile}");
			} catch (Exception e) {
				ULogger.Warning ("Failed to load intrinsic file", e);
			}
		}

		protected override void _Shutdown () {
			Device?.Dispose ();
			Device = null;
		}

		protected override void _Update () {
			if (Device.UpdateFrame ())
				FrameUpdated.Trigger (Device.frame);
		}

		public string[] GetChannelIdentifiers () => new[] { "Frame" };

		public TexImage GetImage (int channelId) =>
			channelId == 0 && Device != null ? Device.frame.Image : null;

		private IEnumerable _Inspector_GetPropertyConfigCandidates () =>
			new[] {
				Property.EType.FrameRate,
				Property.EType.ExposureTime,
				Property.EType.Gain
			}.Where (x => properties.All (prop => x != prop.type)).Select (
				x => new ValueDropdownItem (x.ToString (),
					new PropertyConfig (x, new Property (true, 0))));

		/*
		public Vector3 PixelPointToRayInParent (Vector2 p2_pixelPoint) {

			var ray = PixelPointToGlobalRay (p2_pixelPoint);
			if (transform.parent != null)
				return transform.parent.InverseTransformVector (ray);
			return ray;

		}



		public Vector3 PixelPointToGlobalRay (Vector2 p2_pixelPoint) {

			return transform.TransformVector (PixelPointToLocalRay (p2_pixelPoint));

		}



		public Vector3 PixelPointToLocalRay (Vector2 p2_pixelPoint) {

			var cmat = _videoCameraService.cameraMatrix;
			return cmat.PixelToRay (p2_pixelPoint);

		}
		*/

	}

}