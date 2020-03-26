using UnityEngine;
using Event = UDlib.Core.Utils.Event;

namespace UDlib.App.Internal {

	public abstract class _Mode : MonoBehaviour {

		public Event Entered = new Event ();
		public Event Exited = new Event ();
		private bool _initialized;

		internal bool CheckBeforeEnter () {
			if (!_initialized)
				_Initialize ();
			_initialized = true;
			return _CheckBeforeEnter ();
		}

		internal void Enter () {
			gameObject.SetActive (true);
			_Enter ();
			Entered.Trigger ();
		}

		internal void Exit () {
			_Exit ();
			gameObject.SetActive (false);
			Exited.Trigger ();
		}

		private void Awake () {
			_App.Instance.AddMode (this);
			gameObject.SetActive (false);
		}

		private void Reset () {
			name = GetType ().Name;
		}

		/// <summary>
		/// 执行初始化操作。
		/// <para>该方法在基类<see cref="_Mode"/>.Awake()中调用。</para>
		/// </summary>
		///
		/// <author>Nianchen Deng</author>
		protected abstract void _Initialize ();

		/// <summary>
		/// 执行模式进入前的检查操作。若不满足进入要求，则返回<c>false</c>。
		/// </summary>
		/// 
		/// <returns>是否满足进入要求</returns>
		/// <author>Nianchen Deng</author>
		protected abstract bool _CheckBeforeEnter ();

		/// <summary>
		/// 执行进入模式操作。
		/// </summary>
		/// 
		/// <author>Nianchen Deng</author>
		protected abstract void _Enter ();

		/// <summary>
		/// 执行退出模式操作。
		/// </summary>
		/// <author>Nianchen Deng</author>
		protected abstract void _Exit ();

	}

}