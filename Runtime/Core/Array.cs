using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;
using UDlib.Core.Internal;

namespace UDlib.Core {

	[StructLayout(LayoutKind.Sequential)]
	public class Array<T> : IDisposable, IEnumerable<T> where T : unmanaged {

		private _NativeArray<T> _array;

		internal _NativeArray<T> _InnerArray => _array;
		public int Length => _array.Length;
		public T this [int i] {
			get => _array[i];
			set => _array[i] = value;
		}

		public Array () => _array = default;

		public Array (int length) => _array = new _NativeArray<T> (length);

		public Array (T[] array) => _array = new _NativeArray<T> (array);

		private Array (_NativeArray<T> array) => _array = array;

		public Array<T2> To<T2> () where T2 : unmanaged => new Array<T2> (_array.To<T2> ());

		public void CopyFrom (T[] array) => _array.CopyFrom (array);

		public void CopyTo (T[] array) => _array.CopyTo (array);

		public Array<T> GetSubArray (int offset, int length = -1) =>
			new Array<T> (_array.GetSubArray (offset, length));

		public void Dispose () {
			_Dispose ();
			GC.SuppressFinalize (this);
		}

		public IEnumerator<T> GetEnumerator () => _array.GetEnumerator ();

		IEnumerator IEnumerable.GetEnumerator () => _array.GetEnumerator ();

		~Array () => _Dispose ();

		private void _Dispose () => _array.Dispose ();

		public static implicit operator Array<T> (T[] array) => new Array<T> (array);

		public static explicit operator T[] (Array<T> array) {
			var ret = new T[array.Length];
			array.CopyTo (ret);
			return ret;
		}

		public override string ToString () {
			var sb = new StringBuilder ();
			sb.Append ($"Array<{typeof(T).FullName}>[{Length}] {{ ");
			for (var i = 0; i < Length; ++i) {
				sb.Append (i == Length - 1 ? $"{this[i]} " : $"{this[i]}, ");
			}
			sb.Append ("}");
			return sb.ToString ();
		}

	}

}