using System;
using System.Collections.Generic;
using System.IO;

namespace UDlib.Net.Internal {

	internal class _SubPacketHead : IComparable<_SubPacketHead> {

		public enum EPacketType {

			Invalid = -1,
			UdpSmall, UdpLargeFirst, UdpLargeOther, Tcp

		}

		private const byte _FIX_ID = (byte) 'U';
		private static readonly byte[] _TYPE_ID =
			{ (byte) 'A', (byte) 'B', (byte) 'C', (byte) 'D' };

		public byte fid;
		public byte tid;

#region Only for UdpLargeXxx

		public byte seq;
		public byte sub;

#endregion

		public EPacketType PacketType {
			get {
				if (fid != _FIX_ID) return EPacketType.Invalid;
				for (var i = 0; i < _TYPE_ID.Length; ++i)
					if (tid == _TYPE_ID[i])
						return (EPacketType) i;
				return EPacketType.Invalid;
			}
		}

		public bool IsValid => PacketType != EPacketType.Invalid;

		public static ushort Size (EPacketType type) {
			switch (type) {
				case EPacketType.UdpSmall:
				case EPacketType.Tcp:
					return 2;
				case EPacketType.UdpLargeFirst:
				case EPacketType.UdpLargeOther:
					return 4;
				default:
					return ushort.MaxValue;
			}
		}

		public _SubPacketHead (EPacketType packetType, byte seq = 0, byte sub = 0) {
			this.fid = _FIX_ID;
			this.tid = _TYPE_ID[(int) packetType];
			this.seq = seq;
			this.sub = sub;
		}

		public void WriteTo (BinaryWriter writer) {
			var packetType = PacketType;
			if (packetType == EPacketType.Invalid)
				throw new InvalidDataException ("PacketType of this head is invalid");
			writer.Write (fid);
			writer.Write (tid);
			if (packetType == EPacketType.Tcp || packetType == EPacketType.UdpSmall)
				return;
			writer.Write (seq);
			writer.Write (sub);
		}

		public static _SubPacketHead ReadFrom (BinaryReader reader) {
			var ret = new _SubPacketHead {
				fid = reader.ReadByte (),
				tid = reader.ReadByte ()
			};
			switch (ret.PacketType) {
				case EPacketType.Invalid:
					throw new InvalidDataException ("Invalid head was read");
				case EPacketType.Tcp:
				case EPacketType.UdpSmall:
					return ret;
				default:
					ret.seq = reader.ReadByte ();
					ret.sub = reader.ReadByte ();
					return ret;
			}
		}

		private _SubPacketHead () { }

		public int CompareTo (_SubPacketHead other) {
			if (ReferenceEquals (this, other)) return 0;
			if (ReferenceEquals (null, other)) return 1;
			var seqComparison = seq.CompareTo (other.seq);
			if (seqComparison != 0) return seqComparison;
			var subIdx = PacketType == EPacketType.UdpLargeFirst ? 0 : sub;
			var otherSubIdx = other.PacketType == EPacketType.UdpLargeFirst ? 0 : other.sub;
			return subIdx.CompareTo (otherSubIdx);
		}

		public static bool operator < (_SubPacketHead left, _SubPacketHead right) {
			return Comparer<_SubPacketHead>.Default.Compare (left, right) < 0;
		}

		public static bool operator > (_SubPacketHead left, _SubPacketHead right) =>
			Comparer<_SubPacketHead>.Default.Compare (left, right) > 0;

		public static bool operator <= (_SubPacketHead left, _SubPacketHead right) =>
			Comparer<_SubPacketHead>.Default.Compare (left, right) <= 0;

		public static bool operator >= (_SubPacketHead left, _SubPacketHead right) =>
			Comparer<_SubPacketHead>.Default.Compare (left, right) >= 0;

	}

}