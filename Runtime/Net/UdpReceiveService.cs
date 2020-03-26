using System;
using System.Collections.Generic;
using System.IO;
using System.Net;
using System.Net.Sockets;
using UDlib.App;
using UDlib.Core.Utils;
using UDlib.Net.Internal;
using UDlib.Packet;
using UDlib.Packet.Internal;
using UnityEngine;

namespace UDlib.Net {

	[RequireComponent (typeof(PacketForwardService))]
	public class UdpReceiveService : Service<UdpReceiveService> {

		public int localPort;
		public bool enableBroadcast;
		public bool showLog;

		private UdpClient _client;
		private Dictionary<IPEndPoint, _NetReader> _netReaders;
		private PacketForwardService _packetForwardService;

		protected override void _Startup () {
			_client = new UdpClient (localPort) {
				EnableBroadcast = enableBroadcast
			};
			_netReaders = new Dictionary<IPEndPoint, _NetReader> ();
			_packetForwardService = PacketForwardService.Get ();
		}

		protected override void _Shutdown () {
			_client.Dispose ();
			_client = null;
			_netReaders = null;
			_packetForwardService = null;
		}

		protected override void _Update () {
			_UDPRead ();
		}

		private void _UDPRead () {
			while (_client.Available != 0) {
				// 接收来自远程主机的数据子包，并获取来源地址
				var remoteEP = new IPEndPoint (IPAddress.Any, localPort);
				var data = _client.Receive (ref remoteEP);
				// [LOG] UDP data length
				if (showLog)
					ULogger.Info ($"<{serviceName}> Udp data received, length = {data.Length}");

				// 获取或创建与远程主机绑定的数据包读取对象
				if (!_netReaders.TryGetValue (remoteEP, out var netReader))
					_netReaders.Add (remoteEP, netReader = _NetReader.Create (NetType.UDP));

				// 读取并尝试组装子包
				_RawPacket rawPacket;
				try {
					rawPacket = netReader.Read (new BinaryReader (new MemoryStream (data)),
						data.Length);
				} catch (Exception e) {
					ULogger.Warning (
						$"<{serviceName}> Received unrecognized or corrupted packet", e);
					continue;
				}
				if (rawPacket == null) continue;
				
				// 填入远程主机地址并分发数据包
				rawPacket.remoteEP = remoteEP;
				_packetForwardService.ForwardPacket (rawPacket);
			}
		}

	}

}