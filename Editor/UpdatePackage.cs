using System.IO;
using LitJson;
using UDlib.Core.Utils;
using UnityEditor;

namespace UDlib.Editor {

	public static class UpdatePackage {

		private const string _PACKAGE = "com.dalab.udlib";
		private const string _MANIFEST_PATH = "Packages/manifest.json";

		[MenuItem ("Tools/UDlib/Update")]
		private static void _Run () {
			// 读取manifest.json文件内容
			var reader = File.OpenText (_MANIFEST_PATH);
			var jsonData = JsonMapper.ToObject (reader);
			reader.Close ();
			
			// 删除lock字段中有关当前包的条目
			if (jsonData.ContainsKey ("lock"))
				jsonData["lock"].Remove (_PACKAGE);
			
			// 保存更新后的内容至manifest.json
			var writer = File.CreateText (_MANIFEST_PATH);
			var jsonWriter = new JsonWriter (writer) { PrettyPrint = true };
			JsonMapper.ToJson (jsonData, jsonWriter);
			writer.Close ();
			
			// 通过打开并关闭一个无操作命令行窗口触发Unity的包管理器刷新操作
			new ExternCommand ("cmd", "/C echo:").Execute ();
		}

	}

}