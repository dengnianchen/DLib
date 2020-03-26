using System.Net;
using System.Net.Sockets;
using UDlib.Core.Utils;
using UDlib.Net.Internal;
using UnityEngine;

namespace UDlib.Net {

	public class TcpHost : _TcpServiceBase<TcpHost> {

		public int listenPort = 19000;

		private TcpListener _listener;
		private float _lastCheckTime;

		protected override void _Startup () {
			_listener = new TcpListener (IPAddress.Any, listenPort);
			_listener.Start ();
			_packetForwardService.BindReceiver<_TcpClosePacket> (
				_CloseConnectionToClient);
			_packetForwardService.BindReceiver<_TcpCheckPacket> (
				_CheckPacketReceived);
		}

		protected override void _Shutdown () {
			if (_client != null)
				_CloseConnectionToClient ();
			_listener.Stop ();
			_listener = null;
			_packetForwardService.UnbindReceiver<_TcpClosePacket> (
				_CloseConnectionToClient);
			_packetForwardService.UnbindReceiver<_TcpCheckPacket> (
				_CheckPacketReceived);
			base._Shutdown ();
		}

		private void _CheckPacketReceived (_TcpCheckPacket obj) {
			_lastCheckTime = Time.realtimeSinceStartup;
		}

		private void _CloseConnectionToClient (_TcpClosePacket packet = null) {
			if (packet != null) {
				ULogger.Info ($"<{serviceName}> Close connection to client.");
				Send (new _TcpClosePacket ());
			} else
				ULogger.Info ($"<{serviceName}> Connection to client is closed.");
			_client.Dispose ();
			_client = null;
			_listener.Start ();
			Disconnected.Trigger ();
		}

		private void _AcceptConnectionToClient () {
			_client = _listener.AcceptTcpClient ();
			_listener.Stop ();
			_lastCheckTime = Time.realtimeSinceStartup;
			var clientEP = (IPEndPoint) _client.Client.RemoteEndPoint;
			ULogger.Info (
				$"<{serviceName}> Client has been connected: {clientEP.Address}:{clientEP.Port}.");
		}

		protected override void _Update () {
			if (_client == null) {
				if (_listener.Pending ())
					_AcceptConnectionToClient ();
				return;
			}

			if (Time.realtimeSinceStartup - _lastCheckTime > CONN_CHECK_INTERVAL * 5) {
				ULogger.Info ($"<{serviceName}> Connection is timeout.");
				_CloseConnectionToClient ();
				return;
			}
			_Receive ();
		}

	}

}