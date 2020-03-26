using System;
using System.Collections;
using System.Linq;
using Sirenix.OdinInspector;
using Sirenix.Utilities.Editor;
using UnityEditor;
using UnityEngine;
using Object = UnityEngine.Object;

namespace UDlib.Core.Image {

	[Serializable]
	public class ImageChannel : IEquatable<ImageChannel> {

		[SerializeField, HideLabel, HorizontalGroup ("g1"), ReadOnly, DrawWithUnity]
		private Object _source;
		[HideLabel, HorizontalGroup ("g1", 100)]
		[ValueDropdown (nameof(_Inspector_GetIdList))]
		public int id;

		public TexImage Image => Source?.GetImage (id);
		public IImageSource Source => _source == null ? null : (IImageSource) _source;

		public ImageChannel (IImageSource source, int id = 0) {
			this._source = (Object) source;
			this.id = id;
		}

		public static implicit operator ImageChannel (TexImage image) => new ImageChannel (image);

		private static Object _DrawSourcePicker (Object value, GUIContent label) {
			return (Object) SirenixEditorFields.PolymorphicObjectField (value, typeof(IImageSource),
				true);
		}

		public bool Equals (ImageChannel other) {
			if (ReferenceEquals (null, other)) return false;
			if (ReferenceEquals (this, other)) return true;
			return _source == other._source && id == other.id;
		}

		public override bool Equals (object obj) {
			if (ReferenceEquals (null, obj)) return false;
			if (ReferenceEquals (this, obj)) return true;
			return obj.GetType () == GetType () && Equals ((ImageChannel) obj);
		}

		public override int GetHashCode () {
			unchecked {
				return ((_source != null ? _source.GetHashCode () : 0) * 397) ^ id;
			}
		}

		private IEnumerable _Inspector_GetIdList () =>
			Source?.GetChannelIdentifiers ().Select (
				(name, index) => new ValueDropdownItem (name, index));

	}

}