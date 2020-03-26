using System;
using System.IO;
using System.Net;
using System.Net.Sockets;
using UDlib.App;
using UDlib.App.Exceptions;
using UDlib.Core.Utils;
using UDlib.Net.Internal;
using UDlib.Packet;
using UnityEngine;

namespace UDlib.Net {

	[RequireComponent (typeof(PacketForwardService))]
	public class UdpSendService : Service<UdpSendService> {

		public string remoteIP = "127.0.0.1";
		public int remotePort = 18000;
		public bool showLog;

		private PacketForwardService _packetForwardService;
		private UdpClient _client = new UdpClient ();
		private MemoryStream _buffStream;
		private byte _seq;

		public bool Send (Packet.Packet packet) {
			try {
				if (!IsRunning)
					throw new ServiceException ($"{serviceName} is not running.");
				var remoteEp = new IPEndPoint (IPAddress.Parse (remoteIP), remotePort);
				var packetData = packet.Data;
				var packetCode = _packetForwardService.GetPacketCode (packet.GetType ());
				var writer = new _NetWriter (packetCode, packetData, _seq);
				while (true) {
					try {
						_buffStream.Seek (0, SeekOrigin.Begin);
						var bytesWritten =
							writer.WriteNextSubPacket (new BinaryWriter (_buffStream));
						_client.Send (_buffStream.GetBuffer (), (int) bytesWritten, remoteEp);
					} catch (EndOfStreamException) {
						break;
					}
				}
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
			_client = new UdpClient ();
			_buffStream = new MemoryStream (_NetIoBase.SUB_PACKET_MAX_LENGTH);
			_seq = 0;
		}

		protected override void _Shutdown () {
			_client.Dispose ();
			_client = null;
			_buffStream.Dispose ();
			_buffStream = null;
		}

	}

}