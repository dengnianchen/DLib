using System.IO;

namespace UDlib.Core.Utils {

	public static class FileLogger {

		private static StreamWriter _writer;

		public static void Initialize () {
			if (_writer == null) {
				var fs = File.Open ($"{Framework.PersistencePath}log.txt", FileMode.OpenOrCreate);
				fs.Seek (0, SeekOrigin.End);
				_writer = new StreamWriter (fs);
			}
			_writer.WriteLine ("-------------\n");
			_writer.Flush ();
		}

		public static void Log (string message) {
			if (_writer == null)
				return;
			_writer.WriteLine (message);
			_writer.WriteLine ();
			_writer.Flush ();
		}

	}

}