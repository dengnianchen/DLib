using System.Collections.Generic;
using UDlib.Core;

namespace UDlib.App.Internal {

	internal class _TimerManager : Singleton<_TimerManager> {

		private readonly List<Timer> _timers = new List<Timer> ();
		
		protected override void Dispose (bool disposing) {
			base.Dispose (disposing);
			_timers.Clear ();
		}
		
		public void Update () {
			var timersTempArray = _timers.ToArray ();
			foreach (var timer in timersTempArray)
				timer.CheckTrigger ();
		}

		public static void Add (Timer timer) {
			Instance._timers.Add (timer);
		}
		
		public static void Remove (Timer timer) {
			Instance._timers.Remove (timer);
		}

	}

}