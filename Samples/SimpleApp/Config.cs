using System;
using UnityEngine;

namespace UDlib.Samples.SimpleApp {

	public class Config : UDlib.App.Config {

		public int a;
		public string b;
		public float[] c;
		public Vector2 d;

		protected override void _Initialize () {
			Bind<int> (nameof(a), value => ServiceA.Get ().a = value);
			try {
				Bind<double> (nameof(b), value => { });
			} catch (Exception e) {
				Debug.LogException (e);
			}
		}

	}

}