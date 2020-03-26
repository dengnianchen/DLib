namespace UDlib.Packet.Internal {

	internal abstract class _PacketHandlerBase {

		public abstract void Receive (_RawPacket packet);

		public abstract void Receive (Packet packet);

	}

}