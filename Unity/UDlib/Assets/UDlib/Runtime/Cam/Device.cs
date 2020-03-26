using System.Collections.Generic;
using UDlib.Core;
using UDlib.Core.Image;
using UnityEngine;

namespace UDlib.Cam {

	public class Device : CObject {

		public enum EStatus { None, Initialized, Streaming }
		
		public readonly Frame frame;
		public readonly Intrinsic intrinsic;
		public readonly string identifier;

		private Vector2Int _frameSize;
		private readonly Dictionary<Property.EType, Property> _properties =
			new Dictionary<Property.EType, Property> ();

		public string DeviceInfo => Plugin.Camera_getDeviceInfo (this);
		public EStatus Status => Plugin.Camera_getStatus (this);

		public Vector2Int FrameSize {
			get {
				if (_frameSize.x == 0)
					Plugin.Camera_getFrameSize (this, out _frameSize);
				return _frameSize;
			}
			set {
				Plugin.Camera_setFrameSize (this, value);
				Plugin.Camera_getFrameSize (this, out _frameSize);
				frame._ResetImage ();
				intrinsic._UpdateSize ();
			}
		}

		public PixelFormat.EFormat Format {
			get => Plugin.Camera_getFormat (this);
			set => Plugin.Camera_setFormat (this, value);
		}

		public Device (string deviceType, int id) :
			base (Plugin.Camera_GetInstance (deviceType, id)) {
			frame = new Frame (this, Plugin.Camera_getFrame (this));
			intrinsic = new Intrinsic (this, Plugin.Camera_getIntrinsic (this));
			identifier = $"{deviceType}{id}";
			Debug.Log ($"[UDlib] Camera {identifier} Created\n\n{DeviceInfo}");
		}

		public void Init () => Plugin.Camera_init (this);

		public void Start () => Plugin.Camera_start (this);

		public void Stop () => Plugin.Camera_stop (this);

		public bool GrabFrame () => Plugin.Camera_grabFrame (this);

		public bool UpdateFrame () => Plugin.Camera_updateFrame (this) && (frame.isDirty = true);

		public void LoadIntrinsic (string file) {
			Plugin.Camera_loadIntrinsic (this, file);
			intrinsic._LoadData ();
		}

		public Property.Range GetPropertyRange (Property.EType type) {
			Plugin.Camera_getPropertyRange (this, type, out var ret);
			return ret;
		}

		public Property GetPropertyValue (Property.EType type) {
			if (_properties.TryGetValue (type, out var value)) return value;
			if (!Plugin.Camera_getPropertyValue (this, type, out value))
				return new Property ();
			return _properties[type] = value;
		}

		public bool SetPropertyValue (Property.EType type, Property value) {
			if (!Plugin.Camera_setPropertyValue (this, type, value))
				return false;
			_properties[type] = !Plugin.Camera_getPropertyValue (this, type, out var value1) ?
				value :
				value1;
			return true;
		}

		public static void ReleaseAllInstances () => Plugin.Camera_ReleaseAllInstances ();

		public static void AllGrabFrames () => Plugin.Camera_AllGrabFrames ();

		protected override void Dispose (bool disposing) {
			Plugin.Camera_ReleaseInstance (this);
			Debug.Log ($"[UDlib] Camera {identifier} Released");
		}

	}

}