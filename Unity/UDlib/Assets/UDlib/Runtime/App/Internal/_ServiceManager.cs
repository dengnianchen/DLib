using System;
using System.Collections.Generic;
using System.Linq;
using JetBrains.Annotations;
using UDlib.Core;
using UDlib.Core.Utils;
using Object = UnityEngine.Object;

namespace UDlib.App.Internal {

	internal class _ServiceManager : Singleton<_ServiceManager> {

		/// <summary>
		/// 服务列表是一个服务类型到具体对象的映射表，相同类型的服务对象会以其服务名称构建
		/// 子映射表。在服务初始化时会自动加入列表，在服务销毁时会自动从该列表中移除。通过
		/// 全局方法查询服务对象时会访问该列表。
		/// </summary>
		private readonly Dictionary<Type, Dictionary<string, _Service>> _serviceList =
			new Dictionary<Type, Dictionary<string, _Service>> ();
		/// <summary>
		/// 系统在启动时所能找到的所有服务对象的数组，该变量主要用于批量调用服务对象的方法，
		/// 例如<see cref="_Service.DoMainLoop"/>。
		/// </summary>
		private readonly List<_Service> _allServices;

		/// <summary>
		/// 创建服务管理器对象。构建服务查询表并自启动所有未被禁用的服务。
		/// </summary>
		/// 
		/// <author>Nianchen Deng</author>
		public _ServiceManager () {
			// 获取所有服务实例并根据executeOrder从小到大排序
			_allServices = Object.FindObjectsOfType<_Service> ().ToList ();
			if (_allServices.Any (service => service.executeOrder < 0)) {
				ULogger.Error ("Some services have missing dependencies!");
				_allServices.Clear ();
				Framework.Quit ();
				return;
			}
			_allServices.Sort ((s1, s2) => s1.executeOrder - s2.executeOrder);

			// 构建映射表
			foreach (var service in _allServices) {
				if (!_serviceList.TryGetValue (service.GetType (), out var subList)) {
					_serviceList.Add (service.GetType (),
						subList = new Dictionary<string, _Service> ());
				}
				subList.Add (service.name, service);
			}
		}

		public void StartServices () {
			// 启动所有未禁用的服务
			foreach (var service in _allServices) {
				if (service.isActiveAndEnabled)
					service.Startup ();
			}
		}

		/// <summary>
		/// 调用所有服务的运行代码，跳过当前不处于运行状态的服务实例。
		/// </summary>
		/// 
		/// <author>Nianchen Deng</author>
		public void Update () {
			foreach (var service in _allServices) {
				if (service.isActiveAndEnabled)
					service.DoMainLoop ();
			}
		}

		/// <summary>
		/// 获取指定类的服务实例。若指定类型有多个服务实例且未指定服务名称，则返回任意一个实例。
		/// </summary>
		/// 
		/// <param name="type">服务类</param>
		/// <param name="serviceName">（可选）服务名称</param>
		/// 
		/// <returns>服务实例</returns>
		/// <author>Nianchen Deng</author>
		[CanBeNull]
		public _Service Get (Type type, string serviceName = null) {
			if (!_serviceList.TryGetValue (type, out var subList))
				return null;
			if (serviceName != null)
				return !subList.TryGetValue (serviceName, out var service) ? null : service;
			return subList.Values.FirstOrDefault ();
		}

		/// <summary>
		/// 获取指定类的服务实例。
		/// </summary>
		/// 
		/// <typeparam name="T">服务对象的类</typeparam>
		/// 
		/// <returns>服务实例</returns>
		/// <author>Nianchen Deng</author>
		[CanBeNull]
		public T Get<T> () where T : Service<T> => (T) Get (typeof(T));

		/// <summary>
		/// 获取指定类并具有指定名称的服务实例
		/// </summary>
		/// <typeparam name="T">服务对象的类</typeparam>
		/// <param name="serviceName">服务名称</param>
		/// 
		/// <returns>服务实例</returns>
		/// <author>Nianchen Deng</author>
		[CanBeNull]
		public T Get<T> ([NotNull] string serviceName) where T : Service<T> =>
			(T) Get (typeof(T), serviceName);

		protected override void Dispose (bool disposing) {
			base.Dispose (disposing);
			for (var i = _allServices.Count - 1; i >= 0; --i)
				_allServices[i].Shutdown ();
		}

	}

}