using System;
using UDlib.Core.Exceptions;

namespace UDlib.Core {

	public abstract class Singleton<T> : IDisposable
		where T : Singleton<T> {

		public static T Instance { get; private set; }

		protected Singleton () {
			if (Instance != null)
				throw new DuplicateInitializationException (
					$"{typeof(T).Name} can only be created once");
			Instance = (T) this;
		}

		protected virtual void Dispose (bool disposing) {
			Instance = null;
		}

		public void Dispose () {
			Dispose (true);
			GC.SuppressFinalize (this);
		}

	}

}