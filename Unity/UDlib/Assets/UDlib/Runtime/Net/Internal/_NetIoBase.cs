namespace UDlib.Net.Internal {

	internal abstract class _NetIoBase {

		/// <summary>
		/// 小包头信息长度（2字节长度、2字节代码、4字节时间戳）
		/// </summary>
		protected const ushort SMALL_HEAD_LENGTH = 2 + 2 + 4;
		/// <summary>
		/// 包头信息长度（4字节长度、2字节代码、4字节时间戳）
		/// </summary>
		protected const ushort HEAD_LENGTH = 4 + 2 + 4;
		/// <summary>
		/// 拆分子包时每个子包的最大长度
		/// </summary>
		public const ushort SUB_PACKET_MAX_LENGTH = 30000;

	}

}