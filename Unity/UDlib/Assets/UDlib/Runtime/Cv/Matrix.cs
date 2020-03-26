using System;
using System.Collections;
using System.Collections.Generic;
using System.Data;
using System.Runtime.InteropServices;
using System.Text;
using UDlib.Core;
using UDlib.Core.Internal;
using UDlib.Core.Mathmatics;
using UnityEngine;

namespace UDlib.Cv {

	[StructLayout (LayoutKind.Sequential)]
	public class Matrix<T> : IDisposable, IEnumerable<T> where T : unmanaged {

		private _NativeArray<T> _array;
		private Vector2Int _size;
		private readonly int _step;

		public Vector2Int Size => _size;
		public int NRows => _size.y;
		public int NCols => _size.x;
		public bool IsContinous => _step == _size.x;
		public T this [int r, int c] {
			get {
				if (r < 0 || r >= _size.y || c < 0 || c >= _size.x)
					throw new IndexOutOfRangeException ();
				return _array[r * _step + c];
			}
			set {
				if (r < 0 || r >= _size.y || c < 0 || c >= _size.x)
					throw new IndexOutOfRangeException ();
				_array[r * _step + c] = value;
			}
		}
		public T this [int i] {
			get => this[i / _size.x, i % _size.x];
			set => this[i / _size.x, i % _size.x] = value;
		}
		public T this [Vector2Int p] {
			get => this[p.y, p.x];
			set => this[p.y, p.x] = value;
		}

		public Matrix () { }

		public Matrix (Vector2Int size, Array<T> data, int step = -1) :
			this (size, data.GetSubArray (0)._InnerArray, step) { }

		public Matrix (int nRows, int nCols, Array<T> data, int step = -1) :
			this (new Vector2Int (nCols, nRows), data, step) { }

		public Matrix (Vector2Int size, T[] data = null) {
			_size = size;
			_array = data == null ? new _NativeArray<T> (size.Area ()) : new _NativeArray<T> (data);
			_step = _size.x;
		}

		public Matrix (int nRows, int nCols, T[] data = null) :
			this (new Vector2Int (nCols, nRows), data) { }

		private Matrix (Vector2Int size, _NativeArray<T> data, int step) {
			_size = size;
			_array = data;
			_step = step == -1 ? size.x : step;
		}

		public Matrix<T> GetRow (int r) => GetSubMatrix (new RectInt (0, r, _size.x, 1));

		public Matrix<T> GetColumn (int c) => GetSubMatrix (new RectInt (c, 0, 1, _size.y));

		public Matrix<T> GetSubMatrix (RectInt region) {
			if (region.xMin < 0 || region.yMin < 0 || region.xMax > _size.x ||
			    region.yMax > _size.y)
				throw new ArgumentOutOfRangeException (nameof(region));
			return new Matrix<T> (region.size, _array.GetSubArray (region.x + region.y * _step),
				_step);
		}

		public Matrix<T> Reshape (Vector2Int newSize) {
			if (!IsContinous)
				throw new DataException ("The matrix is not continous.");
			if (newSize.Area () != _size.Area ())
				throw new ArgumentException ("Count of elements must be same after reshape");
			return new Matrix<T> (newSize, _array.GetSubArray (0), newSize.x);
		}

		public Matrix<T> Reshape (int nRows, int nCols) => Reshape (new Vector2Int (nCols, nRows));

		public void Set (IEnumerable<T> data) {
			var dataEnumerator = data.GetEnumerator ();
			var i = 0;
			while (dataEnumerator.MoveNext () && i < _size.Area ()) {
				this[i] = dataEnumerator.Current;
				++i;
			}
			dataEnumerator.Dispose ();
		}

		public void Set (T value) {
			for (var i = 0; i < _size.Area (); ++i)
				this[i] = value;
		}
		
		/*
		public static void Multiply (Matrix<T> a, Matrix<T> b, ref Matrix<T> c) {
			if (a.NCols != b.NRows)
				throw new ArgumentException ("a.cols != b.rows");
			if (a.NRows != c.NRows)
				throw new ArgumentException ("a.rows != c.rows");
			if (b.NCols != c.NCols)
				throw new ArgumentException ("b.cols != c.cols");
		}
		*/

		public void Dispose () {
			_Dispose ();
			GC.SuppressFinalize (this);
		}

		public IEnumerator<T> GetEnumerator () => new Enumerator (this);

		~Matrix () => _Dispose ();

		IEnumerator IEnumerable.GetEnumerator () => GetEnumerator ();

		private void _Dispose () => _array.Dispose ();

		public override string ToString () {
			if (NRows == 0 || NCols == 0)
				return $"Matrix<{typeof(T).FullName}>[{NRows}x{NCols}] {{ }}";
			var sb = new StringBuilder ();
			sb.Append ($"Matrix<{typeof(T).FullName}>[{NRows}x{NCols}] {{\n");
			for (var r = 0; r < NRows; ++r) {
				sb.Append ("\t");
				for (var c = 0; c < NCols; ++c)
					sb.Append (c == NCols - 1 ? $"{this[r, c]}\n" : $"{this[r, c]}, ");
			}
			sb.Append ("}");
			return sb.ToString ();
		}

		public class Enumerator : IEnumerator<T> {

			private Matrix<T> _m;
			private int _i;

			public Enumerator (Matrix<T> m) {
				_m = m;
				_i = -1;
			}

			public bool MoveNext () {
				_i++;
				return _i < _m._size.Area ();
			}

			public void Reset () {
				_i = 0;
			}

			public T Current => _m[_i];
			object IEnumerator.Current => Current;

			public void Dispose () { }

		}

	}

}