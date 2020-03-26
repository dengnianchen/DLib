using System;
using UDlib.Core;
using UDlib.Core.Image;
using UnityEngine;

namespace UDlib.Pattern {

	public class Pattern : CObject {

		/// <summary>
		/// 图案中的角点数量
		/// </summary>
		public readonly int nCorners;

		/// <summary>
		/// 图案中的角点坐标
		/// </summary>
		public Array<Vector2> Corners {
			get {
				var corners = new Array<Vector2> ();
				Plugin.Pattern_getCorners (this, corners);
				return corners;
			}
		}

		/// <summary>
		/// 获取关联的检测器对象。
		/// </summary>
		/// <returns>关联的检测器对象</returns>
		/// <remarks>该方法始终返回新建的检测器对象。</remarks>
		public PatternDetector GetDetector () => new PatternDetector (this);

		/// <summary>
		/// 绘制图案至给定的图像。
		/// </summary>
		/// <param name="img">用以绘制图案的图像</param>
		/// <param name="color">图案的前景色</param>
		/// <remarks>绘制的图案始终为黑色背景。</remarks>
		public void Draw (NativeImage img, Color32 color) => Plugin.Pattern_draw (this, img, color);

		/// <summary>
		/// 使用默认前景色（白色）绘制图案至给定的图像。
		/// </summary>
		/// <param name="img">用以绘制图案的图像</param>
		/// <remarks>绘制的图案始终为黑色背景。</remarks>
		/// <seealso cref="Draw(NativeImage,UnityEngine.Color32)"/>
		public void Draw (NativeImage img) => Plugin.Pattern_draw (this, img, Color.white);

		protected override void Dispose (bool disposing) => Plugin.Pattern_delete (this);

		protected Pattern (IntPtr ptr, int nCorners) : base (ptr) => this.nCorners = nCorners;

	}

}