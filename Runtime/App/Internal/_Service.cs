using System;
using System.Collections.Generic;
using Sirenix.OdinInspector;
using UDlib.App.Exceptions;
using UDlib.Core.Utils;
using UnityEngine;
using Event = UDlib.Core.Utils.Event;

namespace UDlib.App.Internal {

	[ExecuteAlways, HideMonoScript]
	public abstract class _Service : MonoBehaviour {

#region Fields In Inspector

		/// <summary>
		/// 服务的名称
		/// </summary>
		[DisableInPlayMode]
		public string serviceName;
		/// <summary>
		/// 若初始化失败，自动重新初始化的时间间隔。0表示不自动重新初始化。
		/// </summary>
		[DisableInPlayMode]
		public float retryInterval;

		/// <summary>
		/// 服务当前的状态
		/// </summary>
		[ShowInInspector, DisplayAsString, EnableGUI, PropertyOrder (-100)]
		[InfoBox ("$" + nameof(_validateDependenciesErrorMessage), InfoMessageType.Error,
			nameof(_ValidateDependenciesShowError))]
		[InfoBox ("$" + nameof(RuntimeErrorMessage), InfoMessageType.Error,
			nameof(HasRuntimeError))]
		public ServiceState CurrentState =>
			!_started ? ServiceState.Stopped :
			isActiveAndEnabled ? ServiceState.Running : ServiceState.Paused;

#endregion

#region Other Fields

		[SerializeField, HideInInspector]
		internal int executeOrder;
		private bool _started;
		private List<Type> _dependServiceTypes;
		private string _validateDependenciesErrorMessage;

#endregion

#region Events

		/// <summary>
		/// 服务成功初始化后触发该事件
		/// </summary>
		public Event Started = new Event ();
		/// <summary>
		/// 服务终止后触发该事件
		/// </summary>
		public Event Stopped = new Event ();
		/// <summary>
		/// 服务暂停运行后触发该事件
		/// </summary>
		public Event Paused = new Event ();
		/// <summary>
		/// 服务恢复运行后触发该事件
		/// </summary>
		public Event Resumed = new Event ();

#endregion

#region Properties

		/// <summary>
		/// 服务当前是否处于关闭状态
		/// </summary>
		public bool IsStopped => CurrentState == ServiceState.Stopped;
		/// <summary>
		/// 服务当前是否处于暂停运行状态
		/// </summary>
		public bool IsPaused => CurrentState == ServiceState.Stopped;
		/// <summary>
		/// 服务当前是否处在正常运行状态
		/// </summary>
		public bool IsRunning => CurrentState == ServiceState.Running;
		/// <summary>
		/// 运行时错误信息，若不存在错误信息，则该值为null。
		/// </summary>
		public string RuntimeErrorMessage { get; private set; }
		/// <summary>
		/// 服务当前是否存在错误信息。
		/// </summary>
		public bool HasRuntimeError => !string.IsNullOrEmpty (RuntimeErrorMessage);
		/// <summary>
		/// 从注解属性获取当前服务所依赖的服务类
		/// </summary>
		/// 
		/// <returns>服务类列表</returns>
		/// <author>Nianchen Deng</author>
		public IEnumerable<Type> DependServiceTypes {
			get {
				if (_dependServiceTypes != null)
					return _dependServiceTypes;
				_dependServiceTypes = new List<Type> ();
				var attrs = GetType ().GetCustomAttributes (typeof(RequireComponent), true);
				_dependServiceTypes = new List<Type> ();
				foreach (RequireComponent attr in attrs) {
					if (attr.m_Type0 != null && attr.m_Type0.IsSubclassOf (typeof(_Service)))
						_dependServiceTypes.Add (attr.m_Type0);
					if (attr.m_Type1 != null && attr.m_Type1.IsSubclassOf (typeof(_Service)))
						_dependServiceTypes.Add (attr.m_Type1);
					if (attr.m_Type2 != null && attr.m_Type2.IsSubclassOf (typeof(_Service)))
						_dependServiceTypes.Add (attr.m_Type2);
				}
				return _dependServiceTypes;
			}
		}
		private bool _ValidateDependenciesShowError =>
			!string.IsNullOrEmpty (_validateDependenciesErrorMessage);

#endregion

#region Service Main Operations

		public void Startup () {
			if (CurrentState != ServiceState.Stopped)
				return;
			try {
				_Startup ();
				RuntimeErrorMessage = null;
				_started = true;
				enabled = true;
				ULogger.Info ($"Service {serviceName} started");
				Started.Trigger ();
			} catch (Exception e) {
				// 启动服务失败，若设置了自动重启间隔，则在指定间隔后尝试重启
				RuntimeErrorMessage = $"{e.GetType ().Name}: {e.Message}";
				_started = false;
				enabled = false;
				ULogger.Warning (
					$"Start service {serviceName} failed due to {RuntimeErrorMessage}");
				if (retryInterval != 0)
					Invoke (nameof(Startup), retryInterval);
			}
		}

		public void Shutdown () {
			if (CurrentState == ServiceState.Stopped)
				return;
			try {
				_Shutdown ();
			} catch (Exception) {
				// ignored
			}
			RuntimeErrorMessage = null;
			_started = false;
			enabled = false;
			ULogger.Info ($"Service {serviceName} stopped");
			Stopped.Trigger ();
		}

		public void Pause () {
			if (CurrentState != ServiceState.Running)
				return;
			enabled = false;
			ULogger.Info ($"Service {serviceName} paused");
			Paused.Trigger ();
		}

		public void Resume () {
			if (CurrentState != ServiceState.Paused)
				return;
			enabled = true;
			ULogger.Info ($"Service {serviceName} resumed");
			Resumed.Trigger ();
		}

		public void Restart () {
			Shutdown ();
			Startup ();
		}

		internal void DoMainLoop () {
			if (CurrentState != ServiceState.Running)
				return;
			try {
				_Update ();
			} catch (Exception e) {
				RuntimeErrorMessage = $"{e.GetType ().Name}: {e.Message}";
				ULogger.Warning ($"Service {serviceName} incurs {RuntimeErrorMessage}");
			}
		}

#endregion

		private void _UpdateExecuteOrder () {
			executeOrder = 0;
			var componentsMissing = new List<string> ();
			foreach (var type in DependServiceTypes) {
				var service = (_Service) GetComponent (type);
				if (!service)
					componentsMissing.Add (type.Name);
				else {
					if (service.executeOrder == -1)
						service._UpdateExecuteOrder ();
					executeOrder = Math.Max (executeOrder, service.executeOrder + 1);
				}
			}
			_validateDependenciesErrorMessage = null;
			if (componentsMissing.Count == 0) return;
			_validateDependenciesErrorMessage =
				$"Missing required components: {string.Join (",", componentsMissing)}";
			executeOrder = -1;
		}

		private void _ResetExecuteOrder () => executeOrder = -1;

#region Unity Events

		private void OnEnable () {
			if (!Application.IsPlaying (gameObject))
				return;
			if (CurrentState == ServiceState.Paused)
				Resume ();
		}

		private void OnDisable () {
			if (!Application.IsPlaying (gameObject))
				return;
			if (CurrentState == ServiceState.Running)
				Pause ();
		}

		private void Update () {
			if (Application.IsPlaying (gameObject))
				return;
			if (_dependServiceTypes != null || executeOrder == -1) return;
			SendMessage (nameof(_ResetExecuteOrder));
			SendMessage (nameof(_UpdateExecuteOrder));
		}

		private void Reset () {
			// 自动添加服务名称（默认为当前类名）
			serviceName = GetType ().Name;
		}

#endregion

		/// <summary>
		/// 实现具体服务运行时操作，会被定期循环调用。
		/// </summary>
		///
		/// <exception cref="ServiceException">操作发生错误时抛出</exception>
		/// <author>Nianchen Deng</author>
		protected virtual void _Update () { }

		/// <summary>
		/// 实现具体服务的启动操作。
		/// </summary>
		///
		/// <exception cref="ServiceException">操作发生错误时抛出</exception>
		/// <author>Nianchen Deng</author>
		protected virtual void _Startup () { }

		/// <summary>
		/// 实现具体服务的终止操作。该方法不应抛出任何异常。
		/// </summary>
		///
		/// <author>Nianchen Deng</author>
		protected virtual void _Shutdown () { }

	}

}