using System;
using System.Diagnostics;

namespace UDlib.Core.Utils {

	public class ExternCommand {

		public readonly string exe;
		public readonly string arguments;

		public ExternCommand (string exe, string arguments = "") {
			if (string.IsNullOrEmpty (exe))
				throw new ArgumentException ("Exe is not specified", nameof(exe));
			this.exe = exe;
			this.arguments = arguments;
		}

		/// <summary>  
		/// 执行外部命令  
		/// </summary>
		/// <returns>返回执行进程的退出代码</returns>  
		public int Execute () => Execute (false, out _);

		/// <summary>  
		/// 执行外部命令
		/// </summary>
		/// <param name="redirectOutput">是否重定向标准输出</param>
		/// <param name="output">执行进程的标准输出</param>
		/// <returns>返回执行进程的退出代码</returns>  
		public int Execute (bool redirectOutput, out string output) {
			var process = new Process {
				StartInfo = new ProcessStartInfo {
					FileName = exe,
					Arguments = arguments,
					UseShellExecute = false,
					RedirectStandardInput = false,
					RedirectStandardOutput = redirectOutput,
					CreateNoWindow = false
				}
			};
			try {
				process.Start ();
				process.WaitForExit ();
				output = redirectOutput ? process.StandardOutput.ReadToEnd () : null;
				return process.ExitCode;
			} finally {
				process.Close ();
			}
		}

	}

}