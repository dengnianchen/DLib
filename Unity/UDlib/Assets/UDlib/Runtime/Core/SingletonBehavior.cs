using UDlib.Core.Exceptions;
using UnityEngine;

namespace UDlib.Core {

	public abstract class SingletonBehavior<T> : MonoBehaviour
		where T : SingletonBehavior<T> {

		public static T Instance { get; private set; }

		protected virtual void Awake () {
			if (Instance != null)
				throw new DuplicateInitializationException (
					$"Singleton behavior {typeof(T).Name} can only have one instance");
			Instance = (T) this;
		}

	}

}