using System;
using System.IO;
using UDlib.Core.Utils;
using UDlib.Packet.Internal;

namespace UDlib.Net.Internal {

	internal abstract class _NetReader : _NetIoBase {

		public static _NetReader Create (NetType netType) {
			if (netType == NetType.TCP)
				return new _NetTcpReader ();
			return new _NetUdpReader ();
		}

		public abstract _RawPacket Read (BinaryReader reader, int avaliableBytes);

		protected static ulong _ExpandTimestamp (uint shortTimestamp) {
			var now = DateTime.Now.GetTimestamp ();
			var timestamp = now & 0xFFFFFFFF00000000 + shortTimestamp;
			if (timestamp > now)
				timestamp -= 0x100000000;
			return timestamp;
		}

	}

}