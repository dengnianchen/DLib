using System.Runtime.InteropServices;
using UDlib.Core.Exceptions;
using UDlib.Core.Utils;
using UnityEditor;
using UnityEngine;

namespace UDlib.Core {

	public static class Framework {

		public delegate void ExceptionHandle (string message);

		public delegate void ExceptionHandleW (
			[MarshalAs (UnmanagedType.LPWStr)] string message);

		/// <summary>
		/// 获取数据存储路径，该路径与平台、场景名以及构建时的应用名设置有关。
		/// </summary>
		public static string PersistencePath =>
#if UNITY_EDITOR || UNITY_STANDALONE
			$"{Application.dataPath}/../../Data/{Application.productName}/";
#else
			$"{Application.persistentDataPath}/";
#endif

		[RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.BeforeSceneLoad)]
		private static void _Initialize () {
			Plugin.Framework_RegisterExHandler (
				message => throw new CThrownException (message),
				message => throw new CThrownException (message));
			ULogger.Info ("Framework is initialized.");
		}

		public static void Quit () {
#if UNITY_EDITOR
			EditorApplication.isPlaying = false;
#else
			Application.Quit ();
#endif
		}

	}

}