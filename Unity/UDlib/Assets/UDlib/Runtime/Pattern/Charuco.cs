using System.Runtime.CompilerServices;
using UnityEngine;

namespace UDlib.Pattern {

	public class Charuco : GridPattern {

		public readonly float squareLength;

		public Charuco (Vector2Int patternSize, float squareLength) :
			base (Plugin.Charuco_new (patternSize, squareLength), patternSize) =>
			this.squareLength = squareLength;

	}

}