using System;
using UDlib.Core.Mathmatics;
using Unity.Collections.LowLevel.Unsafe;
using UnityEngine;

namespace UDlib.Core.Image {

	/// <summary>
	/// 包装用以在Unity和C++插件之间传递的图像数据。
	/// <para>该结构体即可以包装Unity创建的纹理图像数据从而传递给C++接口使用，也可以供C++接口
	/// 引用C++创建的图像数据从而加载到纹理对象中。</para>
	/// <para>在C++端与之对应的结构体为<c>Dlib::Unity::UImage</c>。</para>
	/// </summary>
	///
	/// <author>Nianchen Deng</author>
	public struct NativeImage {

		/// <summary>
		/// 图像数据的像素格式
		/// </summary>
		public PixelFormat.EFormat format;

		/// <summary>
		/// 图像大小
		/// </summary>
		public Vector2Int size;

		/// <summary>
		/// 图像数据指针
		/// </summary>
		public IntPtr data;

		/// <summary>
		/// 构造包装纹理的内部数据（RawTextureData）的结构体。通过该构造函数可以直接封送纹理
		/// 数据到C++处理。
		/// </summary>
		/// 
		/// <param name="texture">纹理对象，必须开启可读属性（isReadable=true）</param>
		/// 
		/// <exception cref="ArgumentException">纹理对象的格式不受支持（目前支持的格式见
		/// 	<see cref="PixelFormat.EFormat"/>）</exception>
		/// <author>Nianchen Deng</author>
		public NativeImage (Texture2D texture) {
			try {
				format = texture.format.GetPixelFormat ();
			} catch (InvalidOperationException ex) {
				throw new ArgumentException (
					"Texture's format is not supported yet", nameof(texture), ex);
			}
			size = new Vector2Int (texture.width, texture.height);
			unsafe {
				data = (IntPtr) texture.GetRawTextureData<byte> ()
					.GetUnsafePtr ();
			}
		}

		/// <summary>
		/// 创建纹理对象并加载图像数据
		/// </summary>
		/// 
		/// <returns>新的纹理对象</returns>
		/// <author>Nianchen Deng</author>
		public Texture2D CreateTexture () {
			var ret = new Texture2D (size.x, size.y,
				format.GetTextureFormat (), false);
			LoadDataTo (ret);
			return ret;
		}

		/// <summary>
		/// 加载图像数据到纹理对象
		/// </summary>
		/// 
		/// <param name="texture">纹理对象，必须开启可读属性（isReadable=true）</param>
		/// 
		/// <exception cref="ArgumentException">纹理对象的格式与图像数据格式不匹配</exception>
		/// <author>Nianchen Deng</author>
		public void LoadDataTo (Texture2D texture) {
			if (format.GetTextureFormat () != texture.format)
				throw new ArgumentException (
					"Texture's format does not match with CImage's format", nameof(texture));
			texture.LoadRawTextureData (data,
				format.GetSizePerPixel () * size.Area ());
			texture.Apply ();
		}

	}

}