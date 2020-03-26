using System;
using System.Net;
using System.Net.Sockets;
using UDlib.Core.Utils;
using UDlib.Net.Internal;
using UDlib.Packet;
using UnityEngine;

namespace UDlib.Net {

	[RequireComponent (typeof(PacketForwardService))]
	public class TcpClient : _TcpServiceBase<TcpClient> {

		public string remoteIP = "127.0.0.1";
		public int remotePort = 19000;

		protected override void _Startup () {
			try {
				var remoteEp = new IPEndPoint (IPAddress.Parse (remoteIP), remotePort);
				_client = new System.Net.Sockets.TcpClient ();
				_client.Connect (remoteEp);
				InvokeRepeating (nameof(_CheckConnectionState), CONN_CHECK_INTERVAL,
					CONN_CHECK_INTERVAL);
				_packetForwardService.BindReceiver<_TcpClosePacket> (
					_HandleConnectionClosedByRemote);
				base._Startup ();
				Connected.Trigger (remoteEp);
			} catch (Exception) {
				_client.Dispose ();
				_client = null;
				throw;
			}
		}

		protected override void _Shutdown () {
			if (_client != null) {
				try {
					Send (new _TcpClosePacket ());
				} catch (Exception) {
					// ignored
				}
				_client.Dispose ();
				_client = null;
			}
			CancelInvoke (nameof(_CheckConnectionState));
			_packetForwardService.UnbindReceiver<_TcpClosePacket> (
				_HandleConnectionClosedByRemote);
			base._Shutdown ();
			Disconnected.Trigger ();
		}

		protected override void _Update () {
			if (!_client.Connected) {
				_HandleConnectionClosedByRemote ();
				return;
			}
			_Receive ();
		}

		private void _CheckConnectionState () {
			// This is how you can determine whether a socket is still connected. 
			var blockingState = _client.Client.Blocking;
			try {
				_client.Client.Blocking = false;
				Send (new _TcpCheckPacket ());
				if (showLog)
					ULogger.Info ($"<{serviceName}> Send _TcpCheckPacket.");
			} catch (SocketException e) {
				Debug.Log (e);
				// 10035 == WSAEWOULDBLOCK 
				if (!e.NativeErrorCode.Equals (10035))
					_HandleConnectionClosedByRemote ();
			} finally {
				if (_client != null)
					_client.Client.Blocking = blockingState;
			}
		}

		private void _HandleConnectionClosedByRemote (_TcpClosePacket packet = null) {
			ULogger.Info (packet != null ?
				$"<{serviceName}> Connection closed by remote." :
				$"<{serviceName}> Connection to remote is lost.");
			_client.Dispose ();
			_client = null;
			Shutdown ();
			if (retryInterval > 0)
				Startup ();
		}

	}

}