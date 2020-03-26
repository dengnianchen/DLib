using System;
using System.IO;
using UDlib.Packet.Internal;
using static UDlib.Net.Internal._SubPacketHead;

namespace UDlib.Net.Internal {

	internal class _NetTcpReader : _NetReader {

		private MemoryStream _buffStream;
		private _RawPacket _currentReadingPacket;

		public override _RawPacket Read (BinaryReader reader, int avaliableBytes) {
			if (_buffStream == null) {
				if (avaliableBytes < Size (EPacketType.Tcp) + HEAD_LENGTH)
					return null;
				avaliableBytes -= _ReadHeadAndCreateBuff (reader);
			}
			var bytesToRead = Math.Min (_buffStream.Capacity - (int) _buffStream.Position,
				avaliableBytes);
			_buffStream.Write (reader.ReadBytes (bytesToRead), 0, bytesToRead);
			if (_buffStream.Position != _buffStream.Capacity) return null;
			var retPacket = _currentReadingPacket;
			_buffStream = null;
			_currentReadingPacket = null;
			return retPacket;
		}

		private int _ReadHeadAndCreateBuff (BinaryReader reader) {
			var head = ReadFrom (reader);
			if (head.PacketType != EPacketType.Tcp)
				throw new InvalidDataException ("Received packet is not TCP type");
			var data = new byte[(int) reader.ReadUInt32 ()];
			_buffStream = new MemoryStream (data);
			_currentReadingPacket = new _RawPacket {
				code = reader.ReadUInt16 (),
				timestamp = _ExpandTimestamp (reader.ReadUInt32 ()),
				data = data
			};
			return Size (head.PacketType) + HEAD_LENGTH;
		}

	}

}