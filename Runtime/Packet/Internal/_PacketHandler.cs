using System;
using UDlib.Core.Utils;

namespace UDlib.Packet.Internal {

	internal class _PacketHandler<T> : _PacketHandlerBase
		where T : Packet, new () {

		private readonly Event<T> _PackReceived = new Event<T> ();

		public override void Receive (_RawPacket packet) {
			_PackReceived.Trigger (new T {
				Data = packet.data,
				remoteEP = packet.remoteEP,
				timestamp = packet.timestamp
			});
		}

		public override void Receive (Packet packet) => _PackReceived.Trigger ((T) packet);

		public void Bind (Action<T> receiver) => _PackReceived.Bind (receiver);

		public void Unbind (Action<T> receiver) => _PackReceived.Unbind (receiver);

	}

}