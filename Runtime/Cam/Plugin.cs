using System;
using System.Runtime.InteropServices;
using UDlib.Core;
using UDlib.Core.Image;
using UDlib.Cv;
using UnityEngine;

namespace UDlib.Cam {

	internal static class Plugin {

		[DllImport ("UDlib_Cam")]
		public static extern IntPtr Camera_GetInstance (string deviceType, int id);

		[DllImport ("UDlib_Cam")]
		public static extern void Camera_ReleaseInstance (IntPtr p_inst);

		[DllImport ("UDlib_Cam")]
		public static extern void Camera_ReleaseAllInstances ();

		[DllImport ("UDlib_Cam")]
		public static extern void Camera_AllGrabFrames ();

		[DllImport ("UDlib_Cam")]
		public static extern void Camera_init (IntPtr p_inst);

		[DllImport ("UDlib_Cam")]
		public static extern void Camera_start (IntPtr p_inst);

		[DllImport ("UDlib_Cam")]
		public static extern void Camera_stop (IntPtr p_inst);

		[DllImport ("UDlib_Cam")]
		public static extern string Camera_getDeviceInfo (IntPtr p_inst);

		[DllImport ("UDlib_Cam")]
		public static extern Device.EStatus Camera_getStatus (IntPtr p_inst);

		[DllImport ("UDlib_Cam")]
		public static extern IntPtr Camera_getIntrinsic (IntPtr p_inst);

		[DllImport ("UDlib_Cam")]
		public static extern void Camera_loadIntrinsic (IntPtr p_inst, string file);

		[DllImport ("UDlib_Cam")]
		public static extern void Camera_getFrameSize (IntPtr p_inst, out Vector2Int ret);

		[DllImport ("UDlib_Cam")]
		public static extern void Camera_setFrameSize (IntPtr p_inst, Vector2Int size);

		[DllImport ("UDlib_Cam")]
		public static extern PixelFormat.EFormat Camera_getFormat (IntPtr p_inst);

		[DllImport ("UDlib_Cam")]
		[return: MarshalAs (UnmanagedType.I1)]
		public static extern bool Camera_setFormat (IntPtr p_inst, PixelFormat.EFormat format);

		[DllImport ("UDlib_Cam")]
		[return: MarshalAs (UnmanagedType.I1)]
		public static extern bool Camera_getPropertyRange (IntPtr p_inst, Property.EType type,
			out Property.Range ret);

		[DllImport ("UDlib_Cam")]
		[return: MarshalAs (UnmanagedType.I1)]
		public static extern bool Camera_getPropertyValue (IntPtr p_inst, Property.EType type,
			out Property ret);

		[DllImport ("UDlib_Cam")]
		[return: MarshalAs (UnmanagedType.I1)]
		public static extern bool Camera_setPropertyValue (IntPtr p_inst, Property.EType type,
			Property value);

		[DllImport ("UDlib_Cam")]
		[return: MarshalAs (UnmanagedType.I1)]
		public static extern bool Camera_grabFrame (IntPtr p_inst);

		[DllImport ("UDlib_Cam")]
		[return: MarshalAs (UnmanagedType.I1)]
		public static extern bool Camera_updateFrame (IntPtr p_inst);

		[DllImport ("UDlib_Cam")]
		public static extern IntPtr Camera_getFrame (IntPtr p_inst);

		[DllImport ("UDlib_Cam")]
		public static extern long Frame_getTimestamp (IntPtr p_inst);

		[DllImport ("UDlib_Cam")]
		public static extern void Frame_getImage (IntPtr p_inst, NativeImage outImage);

		[DllImport ("UDlib_Cam")]
		public static extern void Frame_getUndistortedImage (IntPtr p_inst, NativeImage outImage);

		/// <summary>
		/// 获取内参数据
		/// </summary>
		/// <param name="p_inst">内参对象</param>
		/// <param name="outCameraMatrix">相机内参矩阵（3 x 3）</param>
		/// <param name="outDistCeoffs">相机畸变系数（1 x 5）</param>
		/// <returns>内参对象中是否包含有效数据</returns>
		[DllImport ("UDlib_Cam")]
		[return: MarshalAs (UnmanagedType.I1)]
		public static extern bool Intrinsic_getData (IntPtr p_inst, Matrix<float> outCameraMatrix,
			Array<float> outDistCeoffs);

		/// <summary>
		/// 用指定内参对图像进行畸变矫正
		/// </summary>
		/// <param name="p_inst">内参对象</param>
		/// <param name="srcImage">源图像</param>
		/// <param name="outUndistortedImage">矫正后的图像</param>
		[DllImport ("UDlib_Cam")]
		public static extern void Intrinsic_undistortImage (IntPtr p_inst, NativeImage srcImage,
			NativeImage outUndistortedImage);

		/// <summary>
		/// 用指定内参对点集坐标进行畸变矫正
		/// </summary>
		/// <param name="p_inst">内参对象</param>
		/// <param name="inPoints">原始点集坐标</param>
		/// <param name="outPoints">矫正后的点集坐标</param>
		[DllImport ("UDlib_Cam")]
		public static extern void Intrinsic_undistortPoints (IntPtr p_inst,
			Array<Vector2> inPoints, Array<Vector2> outPoints);

	}

}