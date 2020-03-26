using System;
using Sirenix.OdinInspector;
using UDlib.Core.Image;
using UDlib.Pattern;
using UnityEngine;

namespace UDlib.Samples.PatternDetect {

	public class PatternDetectSelf : MonoBehaviour, IImageSource {

		public enum PatternType { Charuco, LineGrid }

		[DisableInPlayMode]
		public PatternType patternType;
		[DisableInPlayMode]
		public Vector2Int patternSize;
		[ShowIf (nameof(patternType), PatternType.Charuco), DisableInPlayMode]
		public float squareLength;
		[ShowIf (nameof(patternType), PatternType.LineGrid), DisableInPlayMode]
		public Rect patternRegion;
		public Vector2Int patternImageSize;
		public bool isDebug;

		private TexImage patternImage;
		private TexImage drawLayerImage;

		private PatternDetector _patternPatternDetector;

		private void Awake () => this.AddPreviewComponent ();

		private void Start () {
			patternImage = TexImage.Create (patternImageSize, PixelFormat.EFormat.BGR32);
			drawLayerImage = TexImage.Create (patternImageSize, PixelFormat.EFormat.BGR32);

			_patternPatternDetector =
				(patternType == PatternType.Charuco ?
					(Pattern.Pattern) new Charuco (patternSize, squareLength) :
					new LineGrid (patternSize, patternRegion)).GetDetector ();
			var pattern = (GridPattern) _patternPatternDetector.pattern;
			Debug.Log ($"Pattern {patternType} Corners: {pattern.Corners}");
			Debug.Log ($"Pattern {patternType} XGrids: {pattern.XGrids}");
			Debug.Log ($"Pattern {patternType} YGrids: {pattern.YGrids}");

			_patternPatternDetector.pattern.Draw (patternImage);
			Debug.Log (_patternPatternDetector.Run (patternImage, drawLayerImage, isDebug));
		}

		private void OnDestroy () {
			_patternPatternDetector?.pattern.Dispose ();
			_patternPatternDetector?.Dispose ();
		}

		public string[] GetChannelIdentifiers () => new[] { "Pattern", "Draw Layer" };

		public TexImage GetImage (int channelId) =>
			channelId == 0 ? patternImage : channelId == 1 ? drawLayerImage : null;

	}

}