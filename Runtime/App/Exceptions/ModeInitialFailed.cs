using System;

namespace UDlib.App.Exceptions {

	public class ModeInitialFailed : Exception {

		public ModeInitialFailed () : base ("Failed to initialize the mode") { }

		public ModeInitialFailed (string message) : base (message) { }

		public ModeInitialFailed (string message, Exception innerException) :
			base (message, innerException) { }

	}

}