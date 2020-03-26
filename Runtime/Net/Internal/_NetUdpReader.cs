using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using UDlib.Core.Utils;
using UDlib.Packet.Internal;
using static UDlib.Net.Internal._SubPacketHead;

namespace UDlib.Net.Internal {

	internal class _NetUdpReader : _NetReader {

		private const ulong _TIME_OUT = 5000;
		private readonly SortedList<_SubPacketHead, byte[]>[] _subPackets =
			new SortedList<_SubPacketHead, byte[]>[255];
		private readonly ulong[] _subPacketTimestamps = new ulong[255];

		public override _RawPacket Read (BinaryReader reader, int avaliableBytes) {
			var subPacketHead = ReadFrom (reader);
			avaliableBytes -= Size (subPacketHead.PacketType);
			if (subPacketHead.PacketType == EPacketType.Tcp)
				throw new InvalidDataException ("Received packet is not UDP type");

			_RawPacket retPacket;
			if (subPacketHead.PacketType == EPacketType.UdpSmall) {
				int length = reader.ReadUInt16 ();
				if (length != avaliableBytes) {
					throw new InvalidDataException ("Packet length check failed");
				}
				retPacket = new _RawPacket {
					code = reader.ReadUInt16 (),
					timestamp = _ExpandTimestamp (reader.ReadUInt32 ()),
					data = reader.ReadBytes (avaliableBytes - SMALL_HEAD_LENGTH)
				};
				return retPacket;
			}

			// 读取并添加子包
			var subPacketData = reader.ReadBytes (avaliableBytes);
			_AddSubPacket (subPacketHead, subPacketData);
			
			// 检查并尝试装配子包序列
			retPacket = _TryAssembly (_subPackets[subPacketHead.seq]);
			if (retPacket != null)
				_subPackets[subPacketHead.seq] = null;
			return retPacket;
		}

		private static _RawPacket _TryAssembly (SortedList<_SubPacketHead, byte[]> subPackets) {
			// 若子包序列已经收齐，则组装后返回
			var firstHead = subPackets.First ().Key;
			if (firstHead.PacketType != EPacketType.UdpLargeFirst ||
			    firstHead.sub != subPackets.Count)
				return null;
			var dataList = subPackets.Values;
			var length = (int) BitConverter.ToUInt32 (dataList[0], 0);
			if (length != dataList.Sum (bytes => bytes.Length))
				throw new InvalidDataException ("Packet length check failed");
			var retPacket = new _RawPacket {
				code = BitConverter.ToUInt16 (dataList[0], 4),
				timestamp = _ExpandTimestamp (BitConverter.ToUInt32 (dataList[0], 6)),
				data = new byte[length - HEAD_LENGTH]
			};
			var buffStream = new MemoryStream (retPacket.data);
			buffStream.Write (dataList[0], HEAD_LENGTH, dataList[0].Length - HEAD_LENGTH);
			for (var i = 1; i < dataList.Count; ++i)
				buffStream.Write (dataList[i], 0, dataList[i].Length);
			return retPacket;
		}

		private void _AddSubPacket (_SubPacketHead head, byte[] data) {
			var now = DateTime.Now.GetTimestamp ();

			// 若相同seq的子包序列已经超时，则先清除之
			if (_subPackets[head.seq] != null && now - _subPacketTimestamps[head.seq] > _TIME_OUT)
				_subPackets[head.seq] = null;

			// 若相同seq的子包序列不存在，则创建之
			if (_subPackets[head.seq] == null) {
				_subPackets[head.seq] = new SortedList<_SubPacketHead, byte[]> ();
				_subPacketTimestamps[head.seq] = now;
			}
			try {
				_subPackets[head.seq].Add (head, data);
			} catch (ArgumentException) { }
		}

	}

}