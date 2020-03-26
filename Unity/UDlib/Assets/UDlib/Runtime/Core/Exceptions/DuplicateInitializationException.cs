namespace UDlib.Core.Exceptions {

	public class DuplicateInitializationException : System.Exception {

		public DuplicateInitializationException () : base (
			"this type can only has one instance") {
		}

		public DuplicateInitializationException (string message) : base (
			message) {
		}

	}

}