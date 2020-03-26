using System;
using System.Runtime.InteropServices;
using UDlib.Core;
using UDlib.Core.Image;
using UnityEngine;

namespace UDlib.Pattern {

	internal static class Plugin {

		[DllImport ("UDlib_Pattern")]
		public static extern IntPtr Charuco_new (Vector2Int patternSize, float squareLength);

		[DllImport ("UDlib_Pattern")]
		public static extern IntPtr LineGrid_new (Rect patternRegion, Vector2Int patternSize);

		[DllImport ("UDlib_Pattern")]
		public static extern void Pattern_delete (IntPtr p_inst);

		[DllImport ("UDlib_Pattern")]
		public static extern void Pattern_draw (IntPtr p_inst, NativeImage img, Color32 color);

		[DllImport ("UDlib_Pattern")]
		public static extern IntPtr Pattern_getDetector (IntPtr p_inst);

		[DllImport ("UDlib_Pattern")]
		public static extern void Pattern_getCorners (IntPtr p_inst, [Out] Array<Vector2> o_corners);

		[DllImport ("UDlib_Pattern")]
		public static extern void GridPattern_getXGrids (IntPtr p_inst, [Out] Array<float> o_values);

		[DllImport ("UDlib_Pattern")]
		public static extern void GridPattern_getYGrids (IntPtr p_inst, [Out] Array<float> o_values);
		
		[DllImport ("UDlib_Pattern")]
		public static extern void Detector_delete (IntPtr p_inst);

		[DllImport ("UDlib_Pattern")]
		[return: MarshalAs (UnmanagedType.I1)]
		public static extern bool Detector_run (IntPtr p_inst, NativeImage img,
			Array<Vector2> resultsOut, NativeImage drawLayerOut,
			[MarshalAs (UnmanagedType.I1)] bool isDebug);

		[DllImport ("UDlib_Pattern")]
		[return: MarshalAs (UnmanagedType.I1)]
		public static extern bool Detector_run2 (IntPtr p_inst, IntPtr p_frame,
			[MarshalAs (UnmanagedType.I1)] bool useUndistorted,
			Array<Vector2> resultsOut, NativeImage drawLayerOut,
			[MarshalAs (UnmanagedType.I1)] bool isDebug);

	}

}