using System.IO;
using System.Net;

namespace UDlib.Packet {

	public abstract class Packet {

		/// <summary>
		/// 数据包发送端地址
		/// </summary>
		public IPEndPoint remoteEP;
		/// <summary>
		/// 数据包发送时间戳
		/// </summary>
		public ulong timestamp;

		public byte[] Data {
			get {
				var contentStream = new MemoryStream ();
				_WriteTo (new BinaryWriter (contentStream));
				var retData = new byte[contentStream.Position];
				contentStream.Position = 0;
				contentStream.Read (retData, 0, retData.Length);
				return retData;
			}
			set {
				var contentStream = new MemoryStream (value);
				_ReadFrom (new BinaryReader (contentStream), value.Length);
			}
		}

		public override string ToString () => $"Packet {GetType ().Name}";

		protected abstract void _ReadFrom (BinaryReader reader, int length);

		protected abstract void _WriteTo (BinaryWriter writer);

		/*
		public static bool Receive (double time, byte[] data, IPEndPoint remoteEP) {
			var dataStream = new MemoryStream (data);
			var packReader = new BinaryReader (dataStream);
			var packId = packReader.ReadInt32 ();
			var h = NetPackHandlers.GetHandler (packId);
			if (h == null) {
				if (shouldLog)
					ULogger.Warning ($"ID#{packId} received, no handler");
				return false;
			}
			if (shouldLog)
				ULogger.Info ($"ID#{packId} received");
			h.ReceivePacketData (time, packReader, data.Length - (int) dataStream.Position,
				remoteEP);
			return true;
		}

		public static bool Receive (byte[] data, IPEndPoint remoteEP, bool showLog) {
			var dataStream = new MemoryStream (data);
			var packReader = new BinaryReader (dataStream);
			var packId = packReader.ReadInt32 ();
			var h = NetPackHandlers.GetHandler (packId);
			if (h == null) {
				if (showLog)
					ULogger.Warning ($"ID#{packId} received, no handler");
				return false;
			}
			if (showLog)
				ULogger.Info ($"ID#{packId} received");
			h.ReceivePacketData (packReader, data.Length - (int) dataStream.Position, remoteEP);
			return true;
		}

		public static bool Receive (int packId, long packetLength,
			MemoryStream packetDataStream, IPEndPoint remoteEP, bool showLog) {
			var packReader = new BinaryReader (packetDataStream);
			var h = NetPackHandlers.GetHandler (packId);
			if (h == null) {
				if (showLog)
					ULogger.Warning ($"ID#{packId} received, no handler");
				return false;
			}
			if (showLog)
				ULogger.Info ($"ID#{packId} received");
			h.ReceivePacketData (packReader, (int) packetLength, remoteEP);
			return true;
		}

		public static bool Receive (Packet pack) {
			var h = NetPackHandlers.GetHandler (pack.Id);
			if (h == null) {
				if (shouldLog)
					ULogger.Warning ($"ID#{pack.Id} received, no handler");
				return false;
			}
			if (shouldLog)
				ULogger.Info ($"ID#{pack.Id} received");
			h.ReceivePacket (pack);
			return true;
		}
		*/

		/*
		/// <summary>
		/// 为当前泛型类型<see cref="T"/>生成4字节数据包标识符。
		/// <para>该标识符由类名和命名空间名编码而来。其中第1字节为命名空间编码，2-4字节为类名编码。</para>
		/// </summary>
		/// 
		/// <returns>4字节数据包标识符</returns>
		/// <author>Nianchen Deng</author>
		private static int _GenerateId () {
			// 取命名空间的所有首字母编写成命名空间短名
			var fullNamespace = typeof(T).Namespace;
			var shortNamespace = new List<byte> ();
			for (var i = 0; i < fullNamespace?.Length; ++i) {
				if (i == 0 || i > 0 && fullNamespace[i - 1] == '.')
					shortNamespace.Add ((byte) fullNamespace[i]);
			}
			// 取类名所有大写字母组成类短名
			var shortClassname = (from c in typeof(T).Name
				where char.IsUpper (c) || c == '.'
				select (byte) c).ToArray ();

			// 编码使用4字节整形
			var code = new byte[] { 0, 0, 0, 0 };

			// 命名空间短名编码为1个字节
			switch (shortNamespace.Count) {
				case 0:
					break;
				case 1:
					code[0] = shortNamespace[0];
					break;
				default: {
					var mul = shortNamespace.Aggregate (1, (current, b) => current * b);
					code[0] = shortNamespace.Count == 2 ?
						(byte) (mul + mul >> 8) :
						(byte) (mul + mul >> 16);
					break;
				}
			}

			// 类短名取前3个字节
			for (var i = 0; i < shortClassname.Length && i < 3; ++i)
				code[i + 1] = shortClassname[i];

			// 编码转为整形返回
			return BitConverter.ToInt32 (code, 0);
		}
		*/

	}

}