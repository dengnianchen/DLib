using UDlib.App;
using UDlib.Core.Mathmatics;
using UnityEngine;

namespace UDlib.Samples.SimpleApp {

	public class ModeB : Mode<ModeB> {

		protected override void _Initialize () { }

		protected override bool _CheckBeforeEnter () {
			if (!(App.Config.d.Area () <= 0.0f)) return true;
			Debug.Log ("Cannot enter ModeB");
			return false;
		}

		protected override void _Enter () {
			Debug.Log ($"{name} entered");
		}

		protected override void _Exit () {
			Debug.Log ($"{name} exited");
		}

		private void Update () {
			if (Input.GetKeyDown (KeyCode.Space))
				App.CurrentMode = ModeA.Instance;
		}

	}

}