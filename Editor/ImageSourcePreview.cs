using System.Collections.Generic;
using Sirenix.Utilities;
using Sirenix.Utilities.Editor;
using UDlib.Core.Image;
using UnityEditor;
using UnityEngine;

namespace UDlib.Editor {

	[CustomPreview (typeof(ImageSourcePreviewer))]
	public class ImageSourcePreview : ObjectPreview {

		private readonly Dictionary<Object, int> _currentPreviewChannels =
			new Dictionary<Object, int> ();

		private new ImageSourcePreviewer target => (ImageSourcePreviewer) base.target;

		public override void Initialize (Object[] targets) {
			base.Initialize (targets);
			foreach (var t in targets)
				_currentPreviewChannels.Add (t, 0);
		}

		public override GUIContent GetPreviewTitle () {
			return new GUIContent (_currentPreviewChannels.Count <= 1 ?
				"Image Source Preview" :
				$"Image Source Preview ({_currentPreviewChannels.Count} objects selected)");
		}

		public override bool HasPreviewGUI () => true;

		public override void OnPreviewGUI (Rect r, GUIStyle background) {
			var layoutDropdown = new Rect (r.x + 5, r.y + 5, r.width - 10, 20);
			var layoutImage = new Rect (r.x, layoutDropdown.yMax + 5, r.width,
				r.yMax - layoutDropdown.yMax - 5);
			var currentChannel = _currentPreviewChannels[target];
			currentChannel = SirenixEditorFields.Dropdown (layoutDropdown,
				currentChannel, target.GetChannelIdentifiers ());
			if (target.GetImage (currentChannel)?.Texture != null) {
				var image = target.GetImage (currentChannel);
				var s = new GUIStyle {
					normal = { textColor = Color.white },
					alignment = TextAnchor.MiddleCenter,
					fontSize = r.width > 200 ? 12 : 10
				};
				GUI.DrawTexture (layoutImage.SubYMax (s.fontSize * 1.5f), image.Texture,
					ScaleMode.ScaleToFit, true);
				GUI.Box (layoutImage.AlignBottom (s.fontSize * 1.5f),
					$"{image.Width} x {image.Height} {image.Format} Image", s);
			} else {
				var s = new GUIStyle {
					normal = { textColor = Color.gray },
					alignment = TextAnchor.MiddleCenter
				};
				GUI.Box (layoutImage, "Empty Image", s);
			}
			_currentPreviewChannels[target] = currentChannel;
		}

	}

}