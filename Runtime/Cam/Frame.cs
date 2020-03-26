using System;
using UDlib.Core;
using UDlib.Core.Image;
using UnityEngine;

namespace UDlib.Cam {

	public class Frame : CObject {

		public readonly Device device;
		public bool isUndistorted;
		internal bool isDirty;
		private TexImage _image;
		private long _timestamp;

		public TexImage Image {
			get {
				_UpdateIfDirty ();
				return _image;
			}
		}

		public long Timestamp {
			get {
				_UpdateIfDirty ();
				return _timestamp;
			}
		}

		internal Frame (Device device, IntPtr pointer) : base (pointer) => this.device = device;

		internal void _ResetImage () => _image = null;

		private void _UpdateIfDirty () {
			if (!isDirty)
				return;
			// 首次调用将创建用以存储帧图像的纹理对象
			if (_image == null) {
				var format = device.Format;
				// 对于纹理不支持的像素格式，将纹理格式设置为BGRA32
				try {
					format.GetTextureFormat ();
				} catch (Exception) {
					format = TextureFormat.BGRA32.GetPixelFormat ();
				}
				_image = TexImage.Create (device.FrameSize, format);
			}
			// 获取最新的帧图像或反畸变帧图像以及对应的时间戳
			if (isUndistorted)
				Plugin.Frame_getUndistortedImage (this, _image);
			else
				Plugin.Frame_getImage (this, _image);
			_image.Update ();
			_timestamp = Plugin.Frame_getTimestamp (this);
			// 标记数据已更新
			isDirty = false;
		}

		protected override void Dispose (bool disposing) { }

	}

}