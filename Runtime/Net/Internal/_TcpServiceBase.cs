using System;
using System.IO;
using System.Net;
using UDlib.App;
using UDlib.App.Exceptions;
using UDlib.Core.Utils;
using UDlib.Packet;
using UnityEngine;
using Event = UDlib.Core.Utils.Event;

namespace UDlib.Net.Internal {

	[RequireComponent (typeof(PacketForwardService))]
	public class _TcpServiceBase<T> : Service<T> where T : _TcpServiceBase<T> {

		public Event<IPEndPoint> Connected = new Event<IPEndPoint> ();
		public Event Disconnected = new Event ();

		public bool showLog;

		protected const float CONN_CHECK_INTERVAL = 1.0f;
		protected System.Net.Sockets.TcpClient _client;
		private _NetReader _netReader;
		protected PacketForwardService _packetForwardService;

		public bool Send (Packet.Packet packet) {
			try {
				if (!IsRunning)
					throw new ServiceException ($"{serviceName} is not running.");
				if (_client == null || !_client.Connected)
					throw new ServiceException ($"{serviceName} is not in connection.");
				var packetData = packet.Data;
				var packetCode = _packetForwardService.GetPacketCode (typeof(T));
				var writer = new _NetWriter (packetCode, packetData);
				writer.WriteNextSubPacket (new BinaryWriter (_client.GetStream ()));
				if (showLog)
					ULogger.Info ($"<{serviceName}> {packet.GetType ().FullName} was sent.");
				return true;
			} catch (Exception e) {
				ULogger.Warning ($"<{serviceName}> Send {packet.GetType ().FullName} failed.", e);
				return false;
			}
		}

		protected override void _Startup () {
			_packetForwardService = PacketForwardService.Get ();
			_netReader = _NetReader.Create (NetType.TCP);
		}

		protected override void _Shutdown () {
			_netReader = null;
			_packetForwardService = null;
		}

		protected void _Receive () {
			try {
				while (_client.Available > 0) {
					var rawPacket = _netReader.Read (new BinaryReader (_client.GetStream ()),
						_client.Available);
					if (rawPacket == null) continue;
					rawPacket.remoteEP = (IPEndPoint) _client.Client.RemoteEndPoint;
					_packetForwardService.ForwardPacket (rawPacket);
				}
			} catch (Exception ex) {
				ULogger.Warning ($"<{serviceName}> An error occurs when read packet data.", ex);
				_client?.GetStream ().Seek (0, SeekOrigin.End);
			}
		}

	}

}