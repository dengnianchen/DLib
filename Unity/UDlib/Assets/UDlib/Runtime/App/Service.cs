using UDlib.App.Internal;

namespace UDlib.App {

	public enum ServiceState { Stopped, Paused, Running }

	public abstract class Service<T> : _Service where T : Service<T> {

		/// <summary>
		/// 获取服务实例。若该服务有多个实例，则返回的结果是不确定的。
		/// </summary>
		/// 
		/// <returns>服务实例。若不存在，返回null</returns>
		/// <author>Nianchen Deng</author>
		public static T Get () => _ServiceManager.Instance.Get<T> ();

		/// <summary>
		/// 获取指定名称的服务实例。
		/// </summary>
		/// <param name="serviceName">服务名称</param>
		/// 
		/// <returns>服务实例。若不存在，返回null</returns>
		/// <author>Nianchen Deng</author>
		public static T Get (string serviceName) => _ServiceManager.Instance.Get<T> (serviceName);

	}

}