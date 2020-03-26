using System.IO;
using Sirenix.OdinInspector;
using UDlib.App.Internal;
using UDlib.Core;
using UDlib.Core.Utils;
using UnityEditor;
using UnityEngine;
using UnityEngine.SceneManagement;

namespace UDlib.App {

	[DefaultExecutionOrder (-10000)]
	[ExecuteAlways]
	[HideMonoScript]
	public abstract class App<T, T_Config> : _App
		where T_Config : Config where T : App<T, T_Config> {

		/// <summary>
		/// 应用程序配置信息
		/// </summary>
		protected T_Config config;
		/// <summary>
		/// 服务对象管理器
		/// </summary>
		private _ServiceManager _serviceManager;
		/// <summary>
		/// 计时器管理器
		/// </summary>
		private _TimerManager _timerManager;

		/// <summary>
		/// 单例引用
		/// </summary>
		public new static T Instance => (T) _App.Instance;
		/// <summary>
		/// 应用程序配置信息
		/// </summary>
		public static T_Config Config => Instance.config;
		/// <summary>
		/// 应用程序当前模式，修改该属性会触发模式转换
		/// </summary>
		public static _Mode CurrentMode {
			get => Instance.currentMode;
			set => Instance.SwitchToMode (value);
		}
		/// <summary>
		/// 获取数据存储路径，该路径与平台以及应用名设置有关。
		/// </summary>
		public static string PersistencePath => Framework.PersistencePath;

		public static void Quit () => Framework.Quit ();
		
#region Unity Events

		protected override void Awake () {
			base.Awake ();
			
			if (!Application.IsPlaying (gameObject))
				return;

			// 获取配置对象，若不存在则退出运行
			config = GetComponent<T_Config> ();
			if (config == null) {
				ULogger.Error ($"Missing required component {typeof(T_Config).FullName}!");
				Quit ();
				return;
			}

			// 若数据存储目录不存在，则创建之
			if (!Directory.Exists (PersistencePath))
				Directory.CreateDirectory (PersistencePath);

			_timerManager = new _TimerManager ();
			_serviceManager = new _ServiceManager ();
			
			// 加载配置数据
			config.Load ();
		}

		private void OnDestroy () {
			if (!Application.IsPlaying (gameObject))
				return;
			_serviceManager?.Dispose ();
			_timerManager?.Dispose ();
		}

		private void Start () {
			if (!Application.IsPlaying (gameObject))
				return;
			_serviceManager?.StartServices ();
			SwitchToMode (initialMode);
		}

		private void Update () {
			if (!Application.IsPlaying (gameObject)) {
				errorMessageInInspector = GetComponent<T_Config> () == null ?
					$"Missing required component {typeof(T_Config).FullName}!" :
					null;
				PlayerSettings.productName = applicationName;
				return;
			}
			_timerManager?.Update ();
			_serviceManager?.Update ();
		}

		private void Reset () {
			// 若未添加关联的配置类实例，则自动添加之
			if (GetComponent<T_Config> () == null)
				gameObject.AddComponent<T_Config> ();
			// 应用名默认设置为当前场景名
			applicationName = SceneManager.GetActiveScene ().name;
		}

#endregion

	}

}