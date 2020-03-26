using System;
using System.Runtime.InteropServices;

namespace UDlib.Cam {

	[StructLayout(LayoutKind.Sequential)]
	public struct Property {

		public enum EType {

			FrameRate,
			ExposureTime,
			Gain

		}
		
		[StructLayout(LayoutKind.Sequential)]
		public struct Range {

			public float low;
			public float high;

			public Range (float low = float.NaN, float high = float.NaN) {
				this.low = low;
				this.high = high;
			}

		}

		[MarshalAs(UnmanagedType.I1)]
		public bool isValid;
		[MarshalAs(UnmanagedType.I1)]
		public bool isAuto;
		public float value;

		public Property (bool isAuto, float value) {
			this.isAuto = isAuto;
			this.value = value;
			isValid = true;
		}

	}

}