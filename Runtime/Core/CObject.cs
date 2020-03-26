using System;

namespace UDlib.Core {

	public abstract class CObject : IDisposable {
		
		/// <summary>
		/// 非托管资源指针
		/// </summary>
		protected IntPtr Pointer { get; private set; } = IntPtr.Zero;

		/// <summary>
		/// 构造空指针包装对象
		/// </summary>
		protected CObject () {
		}

		/// <summary>
		/// 构造指定非托管资源指针的包装对象
		/// </summary>
		/// <param name="pointer">指向非托管资源的指针</param>
		protected CObject (IntPtr pointer) {
			Pointer = pointer;
		}

		/// <summary>
		/// 析构方法由GC自动调用。
		/// </summary>
		~CObject () {
			Dispose (false);
		}

		/// <summary>
		/// 主动释放托管/非托管资源。该方法由用户主动调用，因此可以由用户控制资源释放的时机。
		/// </summary>
		public void Dispose () {
			if (Pointer == IntPtr.Zero) return;
			Dispose (true);
			GC.SuppressFinalize (this);
			Pointer = IntPtr.Zero;
		}

		/// <summary>
		/// 实现<see cref="CObject"/>到<see cref="IntPtr"/>的隐式转换。
		/// </summary>
		/// <param name="obj">包装非托管资源的对象</param>
		/// <returns>对应的非托管资源指针</returns>
		public static implicit operator IntPtr (CObject obj) {
			return obj.Pointer;
		}

		/// <summary>
		/// 释放托管/非托管资源。若该方法可能被用户通过Dispose方法调用或GC通过析构方法自动调用，参数<see cref="disposing"/>指示了当前是何种
		/// 调用方式。若被GC通过析构方法自动调用（<see cref="disposing"/>=true），则GC会负责释放托管资源。因此子类实现需对参数
		/// <see cref="disposing"/>加以判断，决定是否要显式释放托管资源。
		/// </summary>
		/// <param name="disposing">是否在Dispose方法中被调用</param>
		protected abstract void Dispose (bool disposing);

	}

}