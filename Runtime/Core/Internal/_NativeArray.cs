using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using Unity.Collections.LowLevel.Unsafe;

namespace UDlib.Core.Internal {

	[StructLayout (LayoutKind.Sequential)]
	internal struct _NativeArray<T> : IDisposable, IEnumerable<T> where T : unmanaged {

		private enum Type {

			RefNative, RefNativeStdVector, RefArray, SelfOwn

		};

		private unsafe T* _data;
		private int _length;
		private readonly Type _type;
		private GCHandle _gcHandle;

		//private static Dictionary<ulong, string> _AllocSet = new Dictionary<ulong, string> ();

		public int Length => _length;
		public T this [int i] {
			get {
				unsafe {
					if (i < 0 || i >= _length)
						throw new IndexOutOfRangeException ();
					return _data[i];
				}
			}
			set {
				unsafe {
					if (i < 0 || i >= _length)
						throw new IndexOutOfRangeException ();
					_data[i] = value;
				}
			}
		}

		public _NativeArray (int length) {
			unsafe {
				if (length == 0) {
					_type = Type.RefNative;
					_length = 0;
					_data = null;
				} else {
					_type = Type.SelfOwn;
					_length = length;
					_data = (T*) Marshal.AllocCoTaskMem (length * sizeof(T));
				}
			}
		}

		private unsafe _NativeArray (T* data, int length) {
			_type = Type.RefNative;
			_data = data;
			_length = length;
		}

		public _NativeArray (T[] array) {
			unsafe {
				_type = Type.RefArray;
				_gcHandle = GCHandle.Alloc (array, GCHandleType.Pinned);
				_length = array.Length;
				_data = (T*) _gcHandle.AddrOfPinnedObject ();
			}
		}

		public _NativeArray<T2> To<T2> () where T2 : unmanaged {
			unsafe {
				return new _NativeArray<T2> ((T2*) _data, _length * sizeof(T) / sizeof(T2));
			}
		}

		public void CopyFrom (T[] array) {
			unsafe {
				if (array.Length != _length)
					throw new ArgumentException ("Length mismatch", nameof(array));
				fixed (T* arrayPointer = array)
					UnsafeUtility.MemCpy (_data, arrayPointer,
						_length * sizeof(T));
			}
		}

		public void CopyTo (T[] array) {
			unsafe {
				if (array.Length != _length)
					throw new ArgumentException ("Length mismatch", nameof(array));
				fixed (T* arrayPointer = array)
					UnsafeUtility.MemCpy (arrayPointer, _data,
						_length * sizeof(T));
			}
		}

		public _NativeArray<T> GetSubArray (int offset, int length = -1) {
			unsafe {
				if (offset < 0 || offset >= _length)
					throw new ArgumentOutOfRangeException (nameof(offset));
				if (length == -1)
					length = _length - offset;
				if (length < 0 || length > _length)
					throw new ArgumentOutOfRangeException (nameof(length));
				return new _NativeArray<T> (_data + offset, length);
			}
		}

		public void Dispose () {
			unsafe {
				if (_data == null)
					return;
				switch (_type) {
					case Type.RefArray:
						_gcHandle.Free ();
						break;
					case Type.SelfOwn:
						/*try {
							_AllocSet.Add ((ulong) _data, StackTraceUtility.ExtractStackTrace ());
						} catch (Exception) {
							FileLogger.Log (_AllocSet[(ulong) _data]);
							FileLogger.Log (StackTraceUtility.ExtractStackTrace ());
						}*/
						Marshal.FreeCoTaskMem ((IntPtr) _data);
						break;
				}
				_data = null;
				_length = 0;
			}
		}

		public static explicit operator T[] (_NativeArray<T> array) {
			var ret = new T[array._length];
			array.CopyTo (ret);
			return ret;
		}

		public IEnumerator<T> GetEnumerator () => new Enumerator (ref this);

		IEnumerator IEnumerable.GetEnumerator () => GetEnumerator ();

		public class Enumerator : IEnumerator<T> {

			private readonly _NativeArray<T> _container;
			private readonly unsafe T* _beginPointer;
			private readonly unsafe T* _endPointer;
			private unsafe T* _dataPointer;

			internal unsafe Enumerator (ref _NativeArray<T> container) {
				_container = container;
				_dataPointer = _beginPointer = container._data - 1;
				_endPointer = container._data + container._length;
			}

			public void Dispose () { }

			public bool MoveNext () {
				unsafe {
					++_dataPointer;
					return _dataPointer < _endPointer;
				}
			}

			public void Reset () {
				unsafe {
					_dataPointer = _beginPointer;
				}
			}

			public T Current {
				get {
					unsafe {
						if (_dataPointer <= _beginPointer || _dataPointer >= _endPointer)
							throw new IndexOutOfRangeException ();
						if (_container._data == null)
							throw new ObjectDisposedException (nameof(_container));
						return *_dataPointer;
					}
				}
			}

			object IEnumerator.Current => Current;

		}

	}

}