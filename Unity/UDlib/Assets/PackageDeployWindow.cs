#if UNITY_EDITOR

using UnityEditor;
using System.Collections.Generic;
using Sirenix.OdinInspector.Editor;
using Sirenix.Utilities.Editor;
using Sirenix.Utilities;

public class PackageDeployWindow : OdinEditorWindow {

	private PackageDeployUtility _package;
	private static PackageDeployWindow _instance;
	
	public static void CloseWindow () {
		if (!_instance) return;
		_instance.Close ();
		_instance = null;
	}
	
	[MenuItem ("Tools/Deploy UDlib")]
	private static void OpenWindow () {
		_instance = GetWindow<PackageDeployWindow> ();
		_instance._package = PackageDeployUtility.GetInstance ();
		_instance.position = GUIHelper.GetEditorWindowRect ().AlignCenter (300, 250);
	}
	
	// In the default implemenentation, it simply yield returns it self.
	// But you can also override this behaviour and have your window render any
	// object you like - Unity and non-Unity objects a like.
	protected override IEnumerable<object> GetTargets () {
		yield return _package;
		yield return this;
	}

}

#endif