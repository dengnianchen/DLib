using System;
using UDlib.App.Internal;
using UnityEngine;

namespace UDlib.App {

	public class Timer : IDisposable {

		private readonly float _interval;
		private readonly Action _action;
		private readonly bool _triggerOnInactiveObject;
		private float _lastTriggeredTime;
		private float _pauseTime;

		public Timer (float interval, Action action,
			bool triggerOnInactiveObject = false) {
			if (_TimerManager.Instance == null)
				throw new MissingComponentException (
					"Timer manager is not initialized");
			_interval = interval;
			_action = action;
			_triggerOnInactiveObject = triggerOnInactiveObject;
			_lastTriggeredTime = Time.realtimeSinceStartup;
			_pauseTime = 0.0f;
			_TimerManager.Add (this);
		}

		public void Pause () {
			if (_pauseTime > 0.0f)
				return;
			_pauseTime = Time.realtimeSinceStartup;
		}

		public void Resume () {
			if (_pauseTime <= 0.0f)
				return;
			_lastTriggeredTime += Time.realtimeSinceStartup - _pauseTime;
			_pauseTime = 0.0f;
		}

		public void CheckTrigger () {
			if (_pauseTime > 0.0f)
				return;
			var time = Time.realtimeSinceStartup;
			if (!(time - _lastTriggeredTime >= _interval)) return;
			_lastTriggeredTime +=
				_interval *
				Mathf.Floor ((time - _lastTriggeredTime) / _interval);
			if (_action.Target is Behaviour behaviour) {
				if (behaviour.isActiveAndEnabled || _triggerOnInactiveObject)
					_action ();
			} else
				_action ();
		}

		public void Dispose () {
			_TimerManager.Remove (this);
		}

	}

}