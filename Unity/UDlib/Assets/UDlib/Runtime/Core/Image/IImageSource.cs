using UnityEngine;

namespace UDlib.Core.Image {

	public interface IImageSource {

		/// <summary>
		/// 获取图像源的通道标识符列表。
		/// </summary>
		/// <returns>图像源的通道标识符列表</returns>
		string[] GetChannelIdentifiers ();

		/// <summary>
		/// 获取指定通道的图像。若指定的序号超出范围，则返回<c>null</c>。
		/// </summary>
		/// <param name="channelId">通道的序号</param>
		/// <returns>指定通道的图像</returns>
		TexImage GetImage (int channelId);

	}

	public static class IImageSourceEx {

		/// <summary>
		/// 添加<see cref="ImageSourcePreviewer"/>组件以启用预览窗口。
		/// <para>
		/// 若对象不是<see cref="MonoBehaviour"/>的子类或<see cref="ImageSourcePreviewer"/>已经存在，
		/// 则不执行任何操作。
		/// </para>
		/// </summary>
		public static void AddPreviewComponent (this IImageSource obj) {
			if (!(obj is MonoBehaviour component))
				return;
			if (component.GetComponent<ImageSourcePreviewer> () != null)
				return;
			component.gameObject.AddComponent<ImageSourcePreviewer> ();
		}

	}

}