using System;
using System.IO;
using System.Text.RegularExpressions;
using Sirenix.OdinInspector;
using UDlib.Core.Utils;
using UnityEngine;
using Debug = UnityEngine.Debug;

public class PackageDeployUtility : ScriptableObject {

	private const string _PACKAGE_FILE = "Assets/UDlib/package.json";

	[InfoBox ("$_errorMessage", InfoMessageType.Error, nameof(_HasErrorMessage))]
	[InfoBox ("$_versionErrorMessage", InfoMessageType.Warning, nameof(_HasVersionErrorMessage))]
	public new string name;
	public string displayName;
	public string author;
	[OnValueChanged (nameof(_OnVersionChanged))]
	public string version;
	public string unity;
	public string description;

	private string _lastDeployedVersion;
	private string _errorMessage;
	private string _versionErrorMessage = "Version is not changed";
	private bool _HasErrorMessage => !string.IsNullOrEmpty (_errorMessage);
	private bool _HasVersionErrorMessage => !string.IsNullOrEmpty (_versionErrorMessage);

	[Button (ButtonSizes.Large), PropertySpace]
	[DisableIf ("_HasVersionErrorMessage")]
	private void Deploy () {
		try {
			_Save ();
		} catch (Exception e) {
			_errorMessage = "Failed to save changes to package.json";
			Debug.LogException (e);
			return;
		}
		if (!_IsPackageFileCommitted ()) {
			new ExternCommand ("TortoiseGitProc", @"/command:commit /path:../../").Execute ();
			if (!_IsPackageFileCommitted ()) {
				_errorMessage = "Please commit main repo before deploy package";
				// 恢复 package.json 文件
				var versionToDeploy = version;
				version = _lastDeployedVersion;
				try {
					_Save ();
				} catch (Exception e) {
					Debug.LogException (e);
				}
				version = versionToDeploy;
				return;
			}
		}
		if (new ExternCommand ("PackageDeploy.cmd", version).Execute () != 0)
			_errorMessage = "Deploy operation is failed";
		else {
			_errorMessage = "";
			PackageDeployWindow.CloseWindow ();
			Debug.Log ($"Package v{version} is successfully deployed");
		}
	}

	public static PackageDeployUtility GetInstance () {
		var instance = CreateInstance<PackageDeployUtility> ();

		// 检查配置文件是否存在
		var file = new FileInfo (_PACKAGE_FILE);
		if (!file.Exists) {
			Debug.LogWarning ($"{file.FullName} does not exist.");
			return instance;
		}

		// 尝试打开配置文件
		StreamReader reader;
		try {
			reader = file.OpenText ();
		} catch (Exception) {
			Debug.LogWarning ($"Failed to open {file.FullName}.");
			return instance;
		}

		// 尝试读取JSON格式的配置内容
		try {
			var json = reader.ReadToEnd ();
			JsonUtility.FromJsonOverwrite (json, instance);
			instance._lastDeployedVersion = instance.version;
		} catch (Exception e) {
			Debug.LogException (e);
			Debug.LogWarning ($"Failed to load package information from {file.FullName}");
		} finally {
			reader.Close ();
		}
		return instance;
	}

	private void _Save () {
		var file = new FileInfo (_PACKAGE_FILE);
		var writer = file.CreateText ();
		try {
			writer.Write (JsonUtility.ToJson (this, true));
		} finally {
			writer.Close ();
		}
	}

	private void _OnVersionChanged () {
		if (version.Equals (_lastDeployedVersion))
			_versionErrorMessage = "Version is not changed";
		else if (!_IsVersionValid (version))
			_versionErrorMessage = "Version is not valid";
		else
			_versionErrorMessage = "";
	}

	private static bool _IsVersionValid (string version) =>
		Regex.IsMatch (version, @"^\d+\.\d+\.\d+$");

	private static bool _IsPackageFileCommitted () {
		new ExternCommand ("git", $"status {_PACKAGE_FILE} -s").Execute (true, out var stdout);
		return string.IsNullOrEmpty (stdout);
	}

}