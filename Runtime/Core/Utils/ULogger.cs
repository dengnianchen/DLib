using System;
using UnityEngine;

namespace UDlib.Core.Utils {

	public static class ULogger {

		public static void Info (string message) {
			Debug.Log ($"[UDlib] {message}");
		}

		public static void Warning (string message, Exception ex = null) {
			Debug.LogWarning (ex == null
				? $"[UDlib] {message}"
				: $"[UDlib] {message} ({ex.GetType ().FullName}: {ex.Message}");
		}

		public static void Error (string message, Exception ex = null) {
			Debug.LogError (ex == null
				? $"[UDlib] {message}"
				: $"[UDlib] {message} ({ex.GetType ().FullName}: {ex.Message}");
		}

	}

}