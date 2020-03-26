using System;
using System.IO;
using UDlib.Core.Utils;
using UDlib.Packet;
using UnityEngine;
using static UDlib.Net.Internal._SubPacketHead;

namespace UDlib.Net.Internal {

	internal class _NetWriter : _NetIoBase {

		/// <summary>
		/// 数据包的时间戳（低32位），在发送端创建对象时自动生成
		/// </summary>
		private readonly uint _timestamp = (uint) DateTime.Now.GetTimestamp ();
		/// <summary>
		/// 数据包的代码，用以表示当前数据包的具体类型
		/// </summary>
		/// <seealso cref="PacketForwardService"/>
		/// <seealso cref="Protocal"/>
		private readonly ushort _code;
		/// <summary>
		/// 数据包的内容
		/// </summary>
		private readonly byte[] _data;
		/// <summary>
		/// 数据长度，若<see cref="_data"/>为<c>null</c>，则值为0
		/// </summary>
		private readonly uint _dataLength;
		private readonly byte _seq;
		private readonly byte _subTot;
		private uint _cursorPos;
		private byte _subIdx;

		public _NetWriter (ushort code, byte[] data) {
			_code = code;
			_data = data;
			_dataLength = (uint) (_data?.Length ?? 0);
			_subTot = 0;
		}

		public _NetWriter (ushort code, byte[] data, byte seq) {
			_code = code;
			_data = data;
			_dataLength = (uint) (_data?.Length ?? 0);
			_seq = seq;

			// 若数据包长度小于子包最大可承载字节数，则以UDP小包的形式发送数据
			if (Size (EPacketType.UdpSmall) + SMALL_HEAD_LENGTH + _dataLength <=
			    SUB_PACKET_MAX_LENGTH)
				_subTot = 1;
			else {
				// 计算所需拆分发送的子包总数
				var subTotF = (float) (HEAD_LENGTH + _dataLength) /
				              (SUB_PACKET_MAX_LENGTH - Size (EPacketType.UdpLargeFirst));
				if (subTotF > 255)
					throw new ArgumentException ("Data is too large for UDP", nameof(data));
				_subTot = (byte) Mathf.Ceil (subTotF);
			}
		}

		public uint WriteNextSubPacket (BinaryWriter writer) {
			if (_cursorPos >= _dataLength)
				throw new EndOfStreamException ("All packet data have been written");

			// TCP方式对数据包无大小限制，直接以整包发送
			if (_subTot == 0) {
				new _SubPacketHead (EPacketType.Tcp).WriteTo (writer);
				writer.Write (HEAD_LENGTH + _dataLength);
				writer.Write (_code);
				writer.Write (_timestamp);
				if (_data?.Length > 0)
					writer.Write (_data);
				_cursorPos = _dataLength;
				return (uint) (Size (EPacketType.Tcp) + HEAD_LENGTH + _dataLength);
			}

			// 若只有1个子包，则以UDP小包的形式发送数据
			if (_subTot == 1) {
				new _SubPacketHead (EPacketType.UdpSmall).WriteTo (writer);
				writer.Write ((ushort) (SMALL_HEAD_LENGTH + _dataLength));
				writer.Write (_code);
				writer.Write (_timestamp);
				if (_data?.Length > 0)
					writer.Write (_data);
				_cursorPos = _dataLength;
				return (uint) (Size (EPacketType.UdpSmall) + SMALL_HEAD_LENGTH + _dataLength);
			}

			// UDP大包形式：将数据写入子包
			uint bytesWritten = 0;

			// 若为首包，则需写入数据包大小、代码和时间戳信息
			if (_subIdx == 0) {
				new _SubPacketHead (EPacketType.UdpLargeFirst, _seq, _subTot).WriteTo (writer);
				writer.Write (HEAD_LENGTH + _dataLength);
				writer.Write (_code);
				writer.Write (_timestamp);
				bytesWritten += (ushort) (Size (EPacketType.UdpLargeFirst) + HEAD_LENGTH);
			} else {
				new _SubPacketHead (EPacketType.UdpLargeOther, _seq, _subIdx).WriteTo (writer);
				bytesWritten += Size (EPacketType.UdpLargeOther);
			}

			// 在子包剩余空间中填入包数据
			var bytesOfDataToWrite = Math.Min (SUB_PACKET_MAX_LENGTH - bytesWritten,
				(uint) _data.Length - _cursorPos);
			writer.Write (_data, (int) _cursorPos, (int) bytesOfDataToWrite);
			bytesWritten += bytesOfDataToWrite;
			_cursorPos += bytesOfDataToWrite;
			_subIdx++;
			return bytesWritten;
		}

	}

}