using System;
using UDlib.Core;
using UDlib.Core.Mathmatics;
using UnityEngine;

namespace UDlib.Pattern {

	public class GridPattern : Pattern {

		/// <summary>
		/// 图案中网格的行列数
		/// </summary>
		public readonly Vector2Int patternSize;
		/// <summary>
		/// 图案中的网格列的横坐标
		/// </summary>
		public Array<float> XGrids {
			get {
				var values = new Array<float> ();
				Plugin.GridPattern_getXGrids (this, values);
				return values;
			}
		}
		/// <summary>
		/// 图案中的网格行的纵坐标
		/// </summary>
		public Array<float> YGrids {
			get {
				var values = new Array<float> ();
				Plugin.GridPattern_getYGrids (this, values);
				return values;
			}
		}

		protected GridPattern (IntPtr ptr, Vector2Int patternSize) :
			base (ptr, patternSize.Area ()) =>
			this.patternSize = patternSize;

	}

}