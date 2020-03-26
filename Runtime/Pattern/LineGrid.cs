using UnityEngine;

namespace UDlib.Pattern {

	public class LineGrid : GridPattern {

		public readonly Rect patternRegion;

		public LineGrid (Vector2Int patternSize, Rect patternRegion) :
			base (Plugin.LineGrid_new (patternRegion, patternSize), patternSize) =>
			this.patternRegion = patternRegion;

	}

}