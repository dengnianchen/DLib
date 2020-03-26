using System;
using Sirenix.OdinInspector;
using UnityEngine;

namespace UDlib.Core.Image {

	[CreateAssetMenu (menuName = "UDlib/Image")]
	[HideMonoScript]
	public class TexImage : ScriptableObject, IImageSource {

		[ShowInInspector, VerticalGroup ("row1/left"), DisplayAsString, EnableGUI]
		public int Width => Texture == null ? 0 : Texture.width;
		[ShowInInspector, VerticalGroup ("row1/left"), DisplayAsString, EnableGUI]
		public int Height => Texture == null ? 0 : Texture.height;
		[ShowInInspector, VerticalGroup ("row1/left"), DisplayAsString, EnableGUI]
		public PixelFormat.EFormat Format =>
			Texture == null ? PixelFormat.EFormat.Unknown : Texture.format.GetPixelFormat ();

		[ShowInInspector, ReadOnly, HideLabel]
		[PreviewField (60, ObjectFieldAlignment.Right)]
		[HorizontalGroup ("row1", 60), VerticalGroup ("row1/right")]
		public Texture2D Texture { get; private set; }
		public Vector2Int Size => new Vector2Int (Width, Height);
		
		private bool _isTextureDirty;

		public static TexImage CreateWithTexture (Texture2D texture) {
			try {
				texture.format.GetPixelFormat ();
				var image = CreateInstance<TexImage> ();
				image.Texture = texture;
				return image;
			} catch (InvalidOperationException) {
				throw new ArgumentException ("Texture's format is unsupported yet",
					nameof(texture));
			}
		}

		public static TexImage Create (Vector2Int size, PixelFormat.EFormat format) {
			try {
				var texture = new Texture2D (size.x, size.y, format.GetTextureFormat (), false);
				var image = CreateInstance<TexImage> ();
				image.Texture = texture;
				return image;
			} catch (InvalidOperationException) {
				throw new ArgumentException ("Specified format is unsupported yet", nameof(format));
			}
		}

		public void Update () {
			if (!_isTextureDirty) return;
			Texture.Apply ();
			_isTextureDirty = false;
		}

		public static implicit operator NativeImage (TexImage a) {
			a._isTextureDirty = true;
			return new NativeImage (a.Texture);
		}

		public string[] GetChannelIdentifiers () => new[] { "Main" };

		public TexImage GetImage (int channelId) => channelId == 0 ? this : null;
		
		public void Reset () { }

	}

}