using System;
using UnityEngine;

namespace UDlib.Core.Utils {

	public static class TimeEx {

		public static ulong GetTimestamp (this DateTime d) {
			var startTime = TimeZoneInfo.ConvertTimeFromUtc (
				new DateTime (1970, 1, 1), TimeZoneInfo.Local);
			return (ulong) (d - startTime).TotalMilliseconds;
		}

		public static bool JustPass (float time) {
			var t0 = Time.time;
			return t0 >= time && t0 - Time.deltaTime < time;
		}

	}

}