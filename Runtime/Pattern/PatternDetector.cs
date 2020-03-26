using UDlib.Cam;
using UDlib.Core;
using UDlib.Core.Image;
using UnityEngine;

namespace UDlib.Pattern {

	public class PatternDetector : CObject {

		/// <summary>
		/// 关联的图案对象
		/// </summary>
		public readonly Pattern pattern;

		/// <summary>
		/// 运行检测算法，在给定图像中检测关联图案的角点。
		/// </summary>
		/// <param name="img">检测算法的目标图像</param>
		/// <param name="drawLayer">用以绘制检测结果或其他信息的绘制图层</param>
		/// <param name="isDebug">是否开启调试模式（开启将会显示额外的调试窗口）</param>
		/// <returns>检测结果，<c>null</c>表示未检测到有效的关联图案</returns>
		/// <remarks>返回的角点坐标中<see cref="float.NaN"/>表示未检测到图案中对应位置的角点</remarks>
		public Array<Vector2> Run (NativeImage img, NativeImage drawLayer, bool isDebug = false) {
			var results = new Array<Vector2> (pattern.nCorners);
			return Plugin.Detector_run (this, img, results, drawLayer, isDebug) ?
				results :
				null;
		}

		/// <summary>
		/// 运行检测算法，在给定画面帧中检测关联图案的角点。
		/// </summary>
		/// <param name="frame">相机的画面帧对象</param>
		/// <param name="drawLayer">用以绘制检测结果或其他信息的绘制图层</param>
		/// <param name="isDebug">是否开启调试模式（开启将会显示额外的调试窗口）</param>
		/// <returns>检测结果，<c>null</c>表示未检测到有效的关联图案</returns>
		/// <remarks>返回的角点坐标中<see cref="float.NaN"/>表示未检测到图案中对应位置的角点</remarks>
		public Array<Vector2> Run (Frame frame, NativeImage drawLayer, bool isDebug = false) {
			var results = new Array<Vector2> (pattern.nCorners);
			return Plugin.Detector_run2 (this, frame, frame.isUndistorted, results, drawLayer,
				isDebug) ?
				results :
				null;
		}

		internal PatternDetector (Pattern pattern) : base (Plugin.Pattern_getDetector (pattern)) =>
			this.pattern = pattern;

		protected override void Dispose (bool disposing) {
			Plugin.Detector_delete (this);
		}

	}

}