using System.Collections.Generic;
using Sirenix.OdinInspector;
using UDlib.Core;
using UDlib.Core.Image;
using UnityEditor;
using UnityEngine;

namespace UDlib.Cv {

	public class Painter {

		public enum Font {

			HERSHEY_SIMPLEX = 0, //!< normal size sans-serif font
			HERSHEY_PLAIN = 1, //!< small size sans-serif font
			HERSHEY_DUPLEX =
				2, //!< normal size sans-serif font (more complex than FONT_HERSHEY_SIMPLEX)
			HERSHEY_COMPLEX = 3, //!< normal size serif font
			HERSHEY_TRIPLEX =
				4, //!< normal size serif font (more complex than FONT_HERSHEY_COMPLEX)
			HERSHEY_COMPLEX_SMALL = 5, //!< smaller version of FONT_HERSHEY_COMPLEX
			HERSHEY_SCRIPT_SIMPLEX = 6, //!< hand-writing style font
			HERSHEY_SCRIPT_COMPLEX = 7, //!< more complex variant of FONT_HERSHEY_SCRIPT_SIMPLEX
			ITALIC = 16 //!< flag for italic font

		}

		public enum MarkerType {

			CROSS = 0, //!< A crosshair marker shape
			TILTED_CROSS = 1, //!< A 45 degree tilted crosshair marker shape
			STAR = 2, //!< A star marker shape, combination of cross and tilted cross
			DIAMOND = 3, //!< A diamond marker shape
			SQUARE = 4, //!< A square marker shape
			TRIANGLE_UP = 5, //!< An upwards pointing triangle marker shape
			TRIANGLE_DOWN = 6 //!< A downwards pointing triangle marker shape

		}

		private readonly TexImage _image;
		private readonly int _shift;

		public Painter (TexImage image, int shift = 2) {
			_image = image;
			_shift = shift;
		}

		public void Line (Vector2 pt1, Vector2 pt2, Color32 color, int thickness = 1) =>
			Plugin.drawLine (_image, pt1, pt2, color, thickness, _shift);

		public void Rectangle (Vector2 tl, Vector2 br, Color32 color, int thickness = 1) =>
			Plugin.drawRectangle (_image, tl, br, color, thickness, _shift);

		public void Rectangle (Rect rect, Color32 color, int thickness = 1) =>
			Plugin.drawRectangle (_image, rect.min, rect.max, color, thickness, _shift);

		public void Circle (Vector2 center, float radius, Color32 color, int thickness = 1) =>
			Plugin.drawCircle (_image, center, radius, color, thickness, _shift);

		public void Ellipse (Vector2 center, Vector2 axes, double angle, double startAngle,
			double endAngle, Color32 color, int thickness = 1) =>
			Plugin.drawEllipse (_image, center, axes, angle, startAngle, endAngle, color, thickness,
				_shift);

		public void Polyline (Array<Vector2> pts, Color32 color, int thickness = 1,
			bool isConvexOrClosed = true) =>
			Plugin.drawPolyline (_image, pts, color, thickness, isConvexOrClosed, _shift);

		public void Triangle (Vector2 pt, float r, float angle, Color32 color, int thickness = 1) =>
			Plugin.drawTriangle (_image, pt, r, angle, color, thickness, _shift);

		public void Marker (Vector2 pt, MarkerType type, int size, Color32 color,
			int thickness = 1) =>
			Plugin.drawMarker (_image, pt, type, size, color, thickness, _shift);

		public void Markers (Array<Vector2> pts, MarkerType type, int size, Color32 color,
			int thickness = 1) =>
			Plugin.drawMarkers (_image, pts, type, size, color, thickness, _shift);

		public void Fill (Color32 background) => Plugin.drawFill (_image, background, _shift);

		public void Text (string text, Vector2Int org, Font fontFace, double fontScale,
			Color32 color, int thickness = 1, bool bottomLeftOrigin = false) =>
			Plugin.drawText (_image, text, org, fontFace, fontScale, color, thickness,
				bottomLeftOrigin, _shift);

	}

}