using System.Runtime.InteropServices;

namespace UDlib.Core {

	internal static class Plugin {

		[DllImport ("UDlib_Core")]
		public static extern void Framework_RegisterExHandler (
			Framework.ExceptionHandle h, Framework.ExceptionHandleW hw);

	}

}