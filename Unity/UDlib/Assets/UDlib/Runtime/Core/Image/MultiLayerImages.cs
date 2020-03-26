using System.Collections;
using System.Collections.Generic;
using System.Linq;
using Sirenix.OdinInspector;
using UDlib.Core.Utils;
using UnityEditor;
using UnityEngine;
using UnityEngine.UI;
using static UnityEditor.AssetDatabase;

namespace UDlib.Core.Image {

	[DefaultExecutionOrder (9999)]
	[ExecuteAlways]
	public class MultiLayerImages : MonoBehaviour {

		public Color color = Color.white;
		[ValidateInput (nameof(_Inspector_ValidateRoi), "Roi must be in range of 0 - 1")]
		public Rect roi = new Rect (0.0f, 0.0f, 1.0f, 1.0f);
		[ValidateInput (nameof(_Inspector_ValidateSize), "Size must be positive")]
		[OnValueChanged (nameof(_Inspector_OnSizeChanged))]
		public Vector2 size;

		[ValidateInput (nameof(_Inspector_ValidateImage), "At most 3 images can be assigned")]
		[SerializeField, PropertySpace (SpaceBefore = 8)]
		[ListDrawerSettings (Expanded = true)]
		[ValueDropdown (nameof(_Inspector_GetAllImageSources),
			DropdownTitle = "Select Image Source",
			DrawDropdownForListElements = false,
			IsUniqueList = true,
			ExpandAllMenuItems = true)]
		internal ImageChannel[] _images = new ImageChannel[0];

		private static readonly int RoiId = Shader.PropertyToID ("_ROI");
		private static readonly int[] _texIds = {
			Shader.PropertyToID ("_TexLayer0"),
			Shader.PropertyToID ("_TexLayer1"),
			Shader.PropertyToID ("_TexLayer2")
		};
		private Material _material;
	
#region Fields to check changes

		private Color _color = Color.white;
		private Rect _roi = new Rect (0.0f, 0.0f, 1.0f, 1.0f);
		private Vector2 _size;
		private readonly Texture2D[] _textures = new Texture2D[3];

#endregion

		public ImageChannel this [int layer] {
			get => _images.Length <= layer ? null : _images[layer];
			set {
				if (_images.Length <= layer) {
					var newArray = new ImageChannel[layer + 1];
					_images.CopyTo (newArray, 0);
					_images = newArray;
				}
				_images[layer] = value;
			}
		}

		private bool _IsUiObject => transform is RectTransform;

		private void _UpdateAspectRatio (int textureIndex) {
			Vector2 size_;
			if (textureIndex == -1)
				size_ = size;
			else {
				var textureSize = _textures[textureIndex].GetSize ();
				size_ = textureSize.x > textureSize.y ?
					new Vector2 (size.x, size.x / textureSize.x * textureSize.y) :
					new Vector2 (size.y / textureSize.y * textureSize.x, size.y);
			}

			if (_IsUiObject) {
				var rectTransform = (RectTransform) transform;
				rectTransform.SetSizeWithCurrentAnchors (RectTransform.Axis.Horizontal, size_.x);
				rectTransform.SetSizeWithCurrentAnchors (RectTransform.Axis.Vertical, size_.y);
			} else {
				var scale = transform.localScale;
				scale.x = size_.x;
				scale.y = size_.y;
				transform.localScale = scale;
			}
		}

		private void Awake () {
			if (!Application.IsPlaying (this))
				return;

			if (GetComponent<MeshRenderer> () != null) {
				GetComponent<MeshRenderer> ().material =
					_material = Instantiate (GetComponent<MeshRenderer> ().material);
			} else if (GetComponent<UnityEngine.UI.Image> () != null) {
				GetComponent<UnityEngine.UI.Image> ().material =
					_material = Instantiate (GetComponent<UnityEngine.UI.Image> ().material);
			} else if (GetComponent<RawImage> () != null) {
				GetComponent<RawImage> ().material =
					_material = Instantiate (GetComponent<RawImage> ().material);
			} else
				throw new MissingComponentException (string.Format (
					"Component {0}, {1} or {2} is required.", typeof(MeshRenderer).FullName,
					typeof(UnityEngine.UI.Image).FullName, typeof(RawImage).FullName));
			
			foreach (var texId in _texIds)
				_material.SetTexture (texId, Texture2D.blackTexture);
		}

		private void LateUpdate () {
			if (!Application.IsPlaying (this))
				return;
			if (_color != color)
				_material.color = _color = color;
			if (_roi != roi) {
				_material.SetVector (RoiId, new Vector4 (roi.xMin, roi.yMin, roi.xMax, roi.yMax));
				_roi = roi;
			}
			var shouldUpdateSize = false;
			var updateSizeAccordingTo = -1;
			if (_size != size) {
				shouldUpdateSize = true;
				_size = size;
			}
			for (var i = 0; i < _textures.Length; ++i) {
				if (i >= _images.Length || !_images[i]?.Image) {
					if (_textures[i]) {
						_textures[i] = null;
						_material.SetTexture (_texIds[i], Texture2D.blackTexture);
						shouldUpdateSize = true;
					}
					continue;
				}
				_images[i].Image.Update ();
				if (updateSizeAccordingTo == -1)
					updateSizeAccordingTo = i;
				if (_textures[i] == _images[i].Image.Texture)
					continue;
				_material.SetTexture (_texIds[i], _textures[i] = _images[i].Image.Texture);
			}
			if (shouldUpdateSize)
				_UpdateAspectRatio (updateSizeAccordingTo);
		}

		private static bool _Inspector_ValidateImage (ImageChannel[] value) => value.Length <= 3;

		private static bool _Inspector_ValidateRoi (Rect value) =>
			value.xMin >= 0 && value.yMin >= 0 && value.xMax <= 1 && value.yMax <= 1;

		private static bool _Inspector_ValidateSize (Vector2 size) => size.x > 0 && size.y > 0;

		private IEnumerable _Inspector_GetAllImageSources () {
			string GetPath (Transform x) =>
				x.parent ? $"{GetPath (x.parent)}/{x.gameObject.name}" : $"{x.gameObject.name}";

			var dropdownItems = new List<ValueDropdownItem> ();
			if (!EditorUtility.IsPersistent (this)) {
				// 添加场景中存在的ImageChannel
				var imageSourceComponents =
					FindObjectsOfType<MonoBehaviour> ().Where (x => x is IImageSource);
				foreach (var isc in imageSourceComponents) {
					var channelIdentifiers = ((IImageSource) isc).GetChannelIdentifiers ();
					dropdownItems.AddRange (channelIdentifiers.Select ((identifier, index) =>
						new ValueDropdownItem (
							$"Scene/{GetPath (isc.transform)} ({isc.GetType ().Name}: {identifier})",
							new ImageChannel ((IImageSource) isc, index))));
				}
			}
			// 添加库中存在的TexImage
			dropdownItems.AddRange (FindAssets ("t:TexImage").Select (GUIDToAssetPath).Select (
				x => new ValueDropdownItem (x, new ImageChannel (LoadAssetAtPath<TexImage> (x)))));

			// 移除当前列表中已经存在的项目并返回最终列表
			return dropdownItems.Where (item => !_images.Any (x => x.Equals (item.Value)));
		}

		private void _Inspector_OnSizeChanged () {
			if (Application.IsPlaying (this))
				return;
			_UpdateAspectRatio (-1);
		}

	}

}