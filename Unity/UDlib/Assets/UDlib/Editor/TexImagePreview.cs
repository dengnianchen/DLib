using UDlib.Core.Image;
using UnityEditor;
using UnityEngine;

namespace UDlib.Editor {

	[CustomPreview (typeof(TexImage))]
	public class TexImagePreview : ObjectPreview {

		public override bool HasPreviewGUI () => true;

		public override void OnPreviewGUI (Rect r, GUIStyle background) {
			if (((TexImage) target).Texture != null)
				GUI.DrawTexture (r, ((TexImage) target).Texture, ScaleMode.ScaleToFit, true);
			else {
				var s = new GUIStyle {
					normal = { textColor = Color.gray },
					alignment = TextAnchor.MiddleCenter
				};
				GUI.Box (r, "Empty Image", s);
			}
		}

	}

}