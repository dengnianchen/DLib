using System;
using UnityEngine;

namespace UDlib.Core.Image {

	public static class PixelFormat {

		/// <summary>
		/// 表示颜色空间的枚举类型
		/// </summary>
		///
		/// <author>Nianchen Deng</author>
		public enum EColorSpace {

			/// <summary>
			/// 未知空间
			/// </summary>
			Unknown = -1,

			/// <summary>
			/// 单色（灰度），支持单通道格式
			/// </summary>
			Mono,

			/// <summary>
			/// 彩色空间（B首位顺序），支持3-4通道格式
			/// </summary>
			BGR,

			/// <summary>
			/// 彩色空间（R首位顺序），支持3-4通道格式
			/// </summary>
			RGB,

			/// <summary>
			/// HSV空间，支持3通道格式
			/// </summary>
			HSV

		}

		/// <summary>
		/// 表示像素格式的枚举类型
		/// <para>目前支持的枚举值以及对应纹理格式和字节数如下：</para>
		/// <list type="table">
		/// <listheader>
		/// <term>枚举值</term>
		/// <description>纹理格式（字节数）</description>
		/// </listheader>
		/// <item>
		/// <term><see cref="BGR32"/></term>
		/// <description><see cref="TextureFormat.BGRA32"/>（4字节）</description>
		/// </item>
		/// <item>
		/// <term><see cref="RGB"/></term>
		/// <description><see cref="TextureFormat.RGB24"/>（3字节）</description>
		/// </item>
		/// <item>
		/// <term><see cref="RGB32"/></term>
		/// <description><see cref="TextureFormat.RGBA32"/>（4字节）</description>
		/// </item>
		/// </list>
		/// </summary>
		/// 
		/// <author>Nianchen Deng</author>
		public enum EFormat {

			/// <summary>
			/// 未知格式
			/// </summary>
			Unknown = -1,

			/// <summary>
			/// 8位单色（灰度），无纹理格式与之对应
			/// </summary>
			Mono8,

			/// <summary>
			/// 24位BGR彩色，无纹理格式与之对应
			/// </summary>
			BGR,

			/// <summary>
			/// 32位BGR彩色（带Alpha通道），与纹理格式<see cref="TextureFormat.BGRA32"/>对应
			/// </summary>
			BGR32,

			/// <summary>
			/// 24位RGB彩色，与纹理格式<see cref="TextureFormat.RGB24"/>对应
			/// </summary>
			RGB,

			/// <summary>
			/// 32位RGB彩色（带Alpha通道），与纹理格式<see cref="TextureFormat.RGBA32"/>对应
			/// </summary>
			RGB32,

			/// <summary>
			/// 3通道浮点数HSV彩色，无纹理格式与之对应
			/// </summary>
			HSV

		}

		/// <summary>
		/// 获取像素格式对应的像素字节数
		/// </summary>
		/// 
		/// <param name="format">像素格式</param>
		/// 
		/// <returns>每个像素所占字节数</returns>
		/// <exception cref="ArgumentOutOfRangeException"></exception>
		/// <seealso cref="EFormat"/>
		/// <author>Nianchen Deng</author>
		public static int GetSizePerPixel (this EFormat format) {
			switch (format) {
				case EFormat.Unknown: return 0;
				case EFormat.Mono8: return 1;
				case EFormat.BGR:
				case EFormat.RGB: return 3;
				case EFormat.BGR32:
				case EFormat.RGB32: return 4;
				case EFormat.HSV: return 3 * sizeof(float);
				default:
					throw new ArgumentOutOfRangeException (nameof(format),
						format, null);
			}
		}

		/// <summary>
		/// 获取像素格式对应的纹理格式
		/// </summary>
		/// 
		/// <param name="format">像素格式</param>
		/// 
		/// <returns>纹理格式</returns>
		/// <exception cref="InvalidOperationException">无纹理格式与指定的像素格式对应</exception>
		/// <seealso cref="EFormat"/>
		/// <author>Nianchen Deng</author>
		public static TextureFormat GetTextureFormat (this EFormat format) {
			switch (format) {
				case EFormat.BGR32: return TextureFormat.BGRA32;
				case EFormat.RGB: return TextureFormat.RGB24;
				case EFormat.RGB32: return TextureFormat.RGBA32;
				default:
					throw new InvalidOperationException (
						"Pixel format is not supported by texture");
			}
		}

		/// <summary>
		/// 获取纹理格式对应的像素格式
		/// </summary>
		/// 
		/// <param name="textureFormat">纹理格式</param>
		/// 
		/// <returns>像素格式</returns>
		/// <exception cref="InvalidOperationException">无像素格式与指定的纹理格式对应</exception>
		/// <seealso cref="EFormat"/>
		/// <author>Nianchen Deng</author>
		public static EFormat GetPixelFormat (this TextureFormat textureFormat) {
			switch (textureFormat) {
				case TextureFormat.RGB24: return EFormat.RGB;
				case TextureFormat.RGBA32: return EFormat.RGB32;
				case TextureFormat.BGRA32: return EFormat.BGR32;
				default:
					throw new InvalidOperationException (
						"Texture format is not supported yet");
			}
		}

	}

}