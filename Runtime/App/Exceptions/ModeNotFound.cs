using System;

namespace UDlib.App.Exceptions {

	public class ModeNotFound : Exception {

		public ModeNotFound () : base ("Mode is not found") { }

		public ModeNotFound (string message) : base (message) { }

		public ModeNotFound (string message, Exception innerException) :
			base (message, innerException) { }

	}

}