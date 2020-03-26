using UDlib.App.Internal;

namespace UDlib.App {

	public abstract class Mode<T> : _Mode where T : Mode<T> {

		private static T _instance;

		public static string Name => typeof(T).Name;
		public static T Instance => _instance = _instance ?? (T) _App.Instance.GetMode (Name);

	}

}