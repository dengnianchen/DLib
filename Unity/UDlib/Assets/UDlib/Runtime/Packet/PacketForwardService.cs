using System;
using System.Collections.Generic;
using System.Linq;
using Sirenix.OdinInspector;
using UDlib.App;
using UDlib.App.Exceptions;
using UDlib.Core.Utils;
using UDlib.Packet.Internal;
using UnityEngine;

namespace UDlib.Packet {

	/// <summary>
	/// Class PacketForwardService
	/// </summary>
	/// 
	/// <author>Nianchen Deng</author>
	public class PacketForwardService : Service<PacketForwardService> {

		[DisableInPlayMode]
		public Protocal protocal;
		private Dictionary<ushort, Type> _packetMapping;
		private Dictionary<Type, ushort> _reverseMapping;
		private Dictionary<Type, _PacketHandlerBase> _handlers;

		public ushort GetPacketCode (Type packetType) {
			if (!_reverseMapping.TryGetValue (packetType, out var code))
				throw new ArgumentException ($"{packetType.FullName} has not been set in protocal",
					nameof(packetType));
			return code;
		}

		/// <summary>
		/// 转发数据包至所有绑定的接收者。
		/// </summary>
		/// <param name="packet">数据包</param>
		/// <seealso cref="BindReceiver{T}"/>
		internal void ForwardPacket (_RawPacket packet) {
			if (!_handlers.TryGetValue (packet.GetType (), out var receivers)) {
				ULogger.Warning ($"{packet.GetType ().FullName} has not been set in protocal");
				return;
			}
			receivers?.Receive (packet);
		}

		/// <summary>
		/// 转发数据包至所有绑定的接收者。
		/// </summary>
		/// <param name="packet">数据包</param>
		/// <seealso cref="BindReceiver{T}"/>
		public void ForwardPacket (Packet packet) {
			if (!_handlers.TryGetValue (packet.GetType (), out var receivers)) {
				ULogger.Warning ($"{packet.GetType ().FullName} has not been set in protocal");
				return;
			}
			receivers?.Receive (packet);
		}

		/// <summary>
		/// 为指定类型的数据包绑定接收方法。
		/// </summary>
		/// <param name="Receiver">接收方法</param>
		/// <typeparam name="T">数据包类型</typeparam>
		/// <exception cref="ServiceException">当前服务尚未启动</exception>
		/// <exception cref="ArgumentException">指定数据包类型未在<see cref="protocal"/>配置项中设置</exception>
		public void BindReceiver<T> (Action<T> Receiver) where T : Packet, new () {
			if (CurrentState == ServiceState.Stopped)
				throw new ServiceException ($"Service {serviceName} has not started yet");
			if (!_handlers.TryGetValue (typeof(T), out var handler))
				throw new ArgumentException ($"{typeof(T).FullName} has not been set in protocal");
			if (handler == null)
				_handlers[typeof(T)] = handler = new _PacketHandler<T> ();
			((_PacketHandler<T>) handler).Bind (Receiver);
		}

		/// <summary>
		/// 为指定类型的数据包解绑接收方法。
		/// </summary>
		/// <param name="Receiver">接收方法</param>
		/// <typeparam name="T">数据包类型</typeparam>
		/// <exception cref="ServiceException">当前服务尚未启动</exception>
		/// <exception cref="ArgumentException">指定数据包类型未在<see cref="protocal"/>配置项中设置</exception>
		public void UnbindReceiver<T> (Action<T> Receiver) where T : Packet, new () {
			if (CurrentState == ServiceState.Stopped)
				throw new ServiceException ($"Service {serviceName} has not started yet");
			if (!_handlers.TryGetValue (typeof(T), out var handler))
				throw new ArgumentException ($"{typeof(T).FullName} has not been set in protocal");
			if (handler == null)
				_handlers[typeof(T)] = handler = new _PacketHandler<T> ();
			((_PacketHandler<T>) handler).Unbind (Receiver);
		}

		protected override void _Startup () {
			if (!protocal)
				protocal = ScriptableObject.CreateInstance<Protocal> ();
			_packetMapping = protocal.FullMapping;
			_reverseMapping = _packetMapping.ToDictionary (
				entry => entry.Value, entry => entry.Key);
			_handlers = _reverseMapping.ToDictionary (
				entry => entry.Key, entry => (_PacketHandlerBase) null);
		}

		protected override void _Shutdown () {
			_packetMapping = null;
			_handlers = null;
		}

	}

}