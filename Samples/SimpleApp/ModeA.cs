using UDlib.App;
using UnityEngine;

namespace UDlib.Samples.SimpleApp {

	public class ModeA : Mode<ModeA> {

		protected override void _Initialize () { }

		protected override bool _CheckBeforeEnter () {
			return true;
		}

		protected override void _Enter () {
			Debug.Log ($"{name} entered");
		}

		protected override void _Exit () {
			Debug.Log ($"{name} exited");
		}

		private void Update () {
			if (Input.GetKeyDown (KeyCode.Space))
				App.CurrentMode = ModeB.Instance;
		}

	}

}