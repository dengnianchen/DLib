using System;
using System.Collections.Generic;
using System.Linq;
using Sirenix.OdinInspector;
using Sirenix.Utilities;
using UDlib.Net.Internal;
using UnityEngine;

namespace UDlib.Packet {

	[CreateAssetMenu (menuName = "UDlib/Protocal")]
	public class Protocal : SerializedScriptableObject {

		[ShowInInspector, ReadOnly, Title ("内置映射")]
		[DictionaryDrawerSettings (KeyLabel = "Code", ValueLabel = "Packet")]
		private Dictionary<ushort, Type> packetMappingPreset = new Dictionary<ushort, Type> ();

		[Title ("自定义映射")]
		[DictionaryDrawerSettings (KeyLabel = "Code", ValueLabel = "Packet")]
		public Dictionary<ushort, Type> packetMapping = new Dictionary<ushort, Type> ();

		public Dictionary<ushort, Type> FullMapping {
			get {
				var ret = new Dictionary<ushort, Type> ();
				foreach (var entry in packetMappingPreset)
					ret.Add (entry.Key, entry.Value);
				foreach (var entry in packetMapping)
					ret.Add (entry.Key, entry.Value);
				return ret;
			}
		}

		public Protocal () {
			// 添加内置映射
			packetMappingPreset.Add (0, typeof(_TcpCheckPacket));
			packetMappingPreset.Add (0, typeof(_TcpClosePacket));
		}

		[Button]
		private void Reset () {
			var types = AssemblyUtilities.GetTypes (AssemblyTypeFlags.UserTypes |
			                                        AssemblyTypeFlags.PluginTypes);
			var filteredTypes = types.Where (type =>
				type.IsSubclassOf (typeof(Packet)) &&
				!packetMappingPreset.ContainsValue (type) &&
				!packetMapping.ContainsValue (type)).ToList ();
			var codeOffset = packetMapping.Count == 0 ? 100 : packetMapping.Keys.Max () + 1;
			for (var i = 0; i < filteredTypes.Count; ++i)
				packetMapping.Add ((ushort) (codeOffset + i), filteredTypes[i]);
		}

	}

}