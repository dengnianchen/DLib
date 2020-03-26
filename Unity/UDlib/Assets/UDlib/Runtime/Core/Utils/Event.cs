using System;
using UnityEngine;

namespace UDlib.Core.Utils {

	public class Event {

		private event Action _actions = delegate { };

		public void Trigger () {
			foreach (var action in _actions.GetInvocationList ()) {
				try {
					((Action) action) ();
				} catch (Exception e) {
					Debug.LogException (e);
				}
			}
		}

		public void Bind (Action action) => _actions += action;

		public void Unbind (Action action) => _actions -= action;

	}

	public class Event<T> {

		private event Action<T> _actions = delegate { };

		public void Trigger (T param1) {
			foreach (var action in _actions.GetInvocationList ()) {
				try {
					((Action<T>) action) (param1);
				} catch (Exception e) {
					Debug.LogException (e);
				}
			}
		}

		public void Bind (Action<T> action) => _actions += action;

		public void Unbind (Action<T> action) => _actions -= action;

	}

	public class Event<T, T2> {

		private event Action<T, T2> _actions = delegate { };

		public void Trigger (T param1, T2 param2) {
			foreach (var action in _actions.GetInvocationList ()) {
				try {
					((Action<T, T2>) action) (param1, param2);
				} catch (Exception e) {
					Debug.LogException (e);
				}
			}
		}

		public void Bind (Action<T, T2> action) => _actions += action;

		public void Unbind (Action<T, T2> action) => _actions -= action;

	}

	public class Event<T, T2, T3> {

		private event Action<T, T2, T3> _actions = delegate { };

		public void Trigger (T param1, T2 param2, T3 param3) {
			foreach (var action in _actions.GetInvocationList ()) {
				try {
					((Action<T, T2, T3>) action) (param1, param2, param3);
				} catch (Exception e) {
					Debug.LogException (e);
				}
			}
		}

		public void Bind (Action<T, T2, T3> action) => _actions += action;

		public void Unbind (Action<T, T2, T3> action) => _actions -= action;

	}

	public class Event<T, T2, T3, T4> {

		private event Action<T, T2, T3, T4> _actions = delegate { };

		public void Trigger (T param1, T2 param2, T3 param3, T4 param4) {
			foreach (var action in _actions.GetInvocationList ()) {
				try {
					((Action<T, T2, T3, T4>) action) (param1, param2, param3, param4);
				} catch (Exception e) {
					Debug.LogException (e);
				}
			}
		}

		public void Bind (Action<T, T2, T3, T4> action) => _actions += action;

		public void Unbind (Action<T, T2, T3, T4> action) => _actions -= action;

	}

}