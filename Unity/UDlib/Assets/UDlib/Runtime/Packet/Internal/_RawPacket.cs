using System.Net;

namespace UDlib.Packet.Internal {

	internal class _RawPacket {

		public ushort code;
		public ulong timestamp;
		public byte[] data;
		public IPEndPoint remoteEP;

	}

}