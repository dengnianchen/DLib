using System;
using System.Runtime.InteropServices;
using UDlib.Core;
using UDlib.Core.Image;
using UnityEngine;

namespace UDlib.Cv {

	internal static class Plugin {

		[DllImport ("UDlib_Cv")]
		[return: MarshalAs (UnmanagedType.I1)]
		public static extern float solveLinear (Matrix<float> matA, Matrix<float> vecB,
			Matrix<float> vecX, int flags);

		[DllImport ("UDlib_Cv")]
		public static extern void triangulatePoints (Matrix<float> projMat1, Matrix<float> projMat2,
			Matrix<Vector2> projPoints1, Matrix<Vector2> projPoints2,
			Matrix<float> points4dOut, out float o_error1, out float o_error2);
		
		[DllImport ("UDlib_Cv")]
		public static extern IntPtr PolyFunc_Fit (int degrees, Array<Vector2> points,
			out float o_error);

		[DllImport ("UDlib_Cv")]
		public static extern void PolyFunc_delete (IntPtr p_inst);

		[DllImport ("UDlib_Cv")]
		public static extern float PolyFunc_eval (IntPtr p_inst, float x);

		[DllImport ("UDlib_Cv")]
		public static extern IntPtr PolyFunc2_Fit (int degrees, Array<Vector2> x,
			Array<float> y, out float o_error);

		[DllImport ("UDlib_Cv")]
		public static extern void PolyFunc2_delete (IntPtr p_inst);

		[DllImport ("UDlib_Cv")]
		public static extern float PolyFunc2_eval (IntPtr p_inst, Vector2 x);

		[DllImport ("UDlib_Cv")]
		public static extern void drawLine (NativeImage img, Vector2 pt1, Vector2 pt2, Color32 color,
			int thickness, int shift);

		[DllImport ("UDlib_Cv")]
		public static extern void drawRectangle (NativeImage img, Vector2 tl, Vector2 br, Color32 color,
			int thickness, int shift);

		[DllImport ("UDlib_Cv")]
		public static extern void drawCircle (NativeImage img, Vector2 center, float radius,
			Color32 color, int thickness, int shift);

		[DllImport ("UDlib_Cv")]
		public static extern void drawEllipse (NativeImage img, Vector2 center, Vector2 axes,
			double angle, double startAngle, double endAngle, Color32 color, int thickness,
			int shift);

		[DllImport ("UDlib_Cv")]
		public static extern void drawPolyline (NativeImage img, Array<Vector2> points,
			Color32 color, int thickness, [MarshalAs (UnmanagedType.I1)] bool isConvexOrClosed,
			int shift);

		[DllImport ("UDlib_Cv")]
		public static extern void drawTriangle (NativeImage img, Vector2 pt, float r, float angle,
			Color32 color, int thickness, int shift);

		[DllImport ("UDlib_Cv")]
		public static extern void drawMarker (NativeImage img, Vector2 pt, Painter.MarkerType type,
			int size,
			Color32 color, int thickness, int shift);

		[DllImport ("UDlib_Cv")]
		public static extern void drawMarkers (NativeImage img, Array<Vector2> pts,
			Painter.MarkerType type, int size, Color32 color, int thickness, int shift);

		[DllImport ("UDlib_Cv")]
		public static extern void drawFill (NativeImage img, Color32 background, int shift);

		[DllImport ("UDlib_Cv")]
		public static extern void drawText (NativeImage img, string text, Vector2Int org,
			Painter.Font fontFace, double fontScale, Color32 color, int thickness,
			[MarshalAs (UnmanagedType.I1)] bool bottomLeftOrigin, int shift);

	}

}