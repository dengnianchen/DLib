using System.Linq;
using Sirenix.OdinInspector;
using UnityEngine;

namespace UDlib.Core.Image {

	[ExecuteAlways, HideMonoScript]
	public class ImageSourcePreviewer : MonoBehaviour {

		private Component _sourceObject;

		public string[] GetChannelIdentifiers () {
			if (_sourceObject == null)
				_sourceObject = _TryGetImageSourceComponent ();
			return ((IImageSource) _sourceObject).GetChannelIdentifiers ();
		}

		public TexImage GetImage (int channelId) {
			if (_sourceObject == null)
				_sourceObject = _TryGetImageSourceComponent ();
			return ((IImageSource) _sourceObject).GetImage (channelId);
		}

		private Component _TryGetImageSourceComponent () =>
			gameObject.GetComponents (typeof(MonoBehaviour))
				.FirstOrDefault (c => c is IImageSource);
		
		private void Start () {
			if (Application.IsPlaying (gameObject))
				return;
			DestroyImmediate (this);
		}

	}

}