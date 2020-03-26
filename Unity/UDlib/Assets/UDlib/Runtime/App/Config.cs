using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using Sirenix.OdinInspector;
using UDlib.Core;
using UDlib.Core.Utils;
using UnityEngine;
using static UDlib.Core.Utils.ULogger;

namespace UDlib.App {

	[HideMonoScript]
	public abstract class Config : SingletonBehavior<Config> {

		/// <summary>
		/// 配置文件路径，默认为 [数据存储路径/]config.json。
		/// </summary>
		/// 
		/// <seealso cref="App{T,T_Config}.PersistencePath"/>
		[ShowInInspector, HideInEditorMode, HideInPrefabs]
		[LabelText ("存储路径"), DisplayAsString]
		private string _path;
		private readonly Dictionary<string, object> _configMap = new Dictionary<string, object> ();
		private readonly Dictionary<string, Action<object>> _listenerMap =
			new Dictionary<string, Action<object>> ();

		/// <summary>
		/// 为指定字段添加绑定监听方法。当该字段值发生变化时，所有绑定的监听方法都会被调用。
		/// </summary>
		/// 
		/// <param name="fieldName">字段名</param>
		/// <param name="Listener">监听方法</param>
		/// <typeparam name="T">字段类型</typeparam>
		///
		/// <author>Nianchen Deng</author>
		public void Bind<T> (string fieldName, Action<T> Listener) {
			var fieldInfo = GetType ().GetField (fieldName);
			if (fieldInfo == null)
				throw new ArgumentException (
					$"{GetType ().FullName} does not contains field {fieldName}",
					nameof(fieldName));
			if (!typeof(T).IsAssignableFrom (fieldInfo.FieldType))
				throw new ArgumentException (
					$"{GetType ().FullName}#{fieldName} cannot convert to {typeof(T).FullName}",
					nameof(fieldName));
			void WrappedListener (object val) => Listener ((T) val);
			if (_listenerMap.TryGetValue (fieldName, out var listeners)) {
				listeners += WrappedListener;
				_listenerMap[fieldName] = listeners;
			} else
				_listenerMap.Add (fieldName, WrappedListener);
			// 绑定时配置文件已经被加载，则直接调用一次处理方法
			if (_configMap.TryGetValue (fieldName, out var value))
				WrappedListener (value);
		}

		/// <summary>
		/// 为多个字段添加绑定监听方法。
		/// </summary>
		/// 
		/// <param name="fieldNames">字段名列表</param>
		/// <param name="listener">监听方法</param>
		/// 
		/// <seealso cref="Bind{T}(string,System.Action{T})"/>
		/// <author>Nianchen Deng</author>
		public void Bind (IEnumerable<string> fieldNames, Action listener) {
			foreach (var fieldName in fieldNames)
				Bind<object> (fieldName, value => listener ());
		}

		/// <summary>
		/// 加载配置文件。若加载时发生错误，则在Log窗口显示警告信息并使用默认配置数据。
		/// <para>该方法由<see cref="App{AppType,ConfigType}"/>在Awake()中调用。</para>
		/// </summary>
		/// 
		/// <author>Nianchen Deng</author>
		internal void Load () {
			// 检查配置文件是否存在
			var file = new FileInfo ($"{Framework.PersistencePath}config.json");
			_path = file.FullName;
			if (!file.Exists) {
				Warning ($"{_path} does not exist. Use default configuration.");
				return;
			}

			// 尝试打开配置文件
			StreamReader reader;
			try {
				reader = file.OpenText ();
			} catch (Exception ex) {
				Warning ($"Failed to open {_path}. Use default configuration.", ex);
				return;
			}

			// 尝试读取JSON格式的配置内容
			try {
				var json = reader.ReadToEnd ();
				JsonUtility.FromJsonOverwrite (json, this);
				_Initialize ();
				_UpdateValueMap ();
				Info ($"Configuration loaded from {_path}.");
			} catch (Exception ex) {
				Warning ("Failed to load configuration. Use default configuration.", ex);
			} finally {
				reader.Close ();
			}
		}

		protected sealed override void Awake () {
			base.Awake ();
			_CheckFieldSupport ();
			InvokeRepeating (nameof(_CheckAndSaveConfig), 1.0f, 1.0f);
		}

		private void OnDestroy () {
			_Save ();
		}

		private void _Save () {
			if (string.IsNullOrEmpty (_path))
				return;

			// 尝试打开配置文件用以写入
			StreamWriter writer;
			try {
				var file = new FileInfo (_path);
				writer = file.CreateText ();
			} catch (Exception ex) {
				Warning ("Failed to save configuration.", ex);
				return;
			}

			// 尝试写入JSON格式的配置数据
			try {
				writer.Write (JsonUtility.ToJson (this, true));
			} catch (Exception ex) {
				Warning ("Failed to save configuration.", ex);
			} finally {
				writer.Close ();
			}
		}

		private void _CheckAndSaveConfig () {
			if (_UpdateValueMap ())
				_Save ();
		}

		private void _CheckFieldSupport () {
			var fields = GetType ().GetFields (BindingFlags.Instance | BindingFlags.Public);
			var unsupportedFields = (from f in fields
				where !f.FieldType.IsArray && f.FieldType != typeof(string) &&
				      !f.FieldType.IsSubclassOf (typeof(ValueType))
				select f.Name).ToList ();
			if (unsupportedFields.Count > 0)
				Warning ($"Found unsupported field {string.Join (", ", unsupportedFields)} " +
				         "in configuration. Only basic types, structures and arrays is currently supported.");
		}

		private bool _UpdateValueMap () {
			var changed = false;
			var fields = GetType ().GetFields (BindingFlags.Instance | BindingFlags.Public);
			foreach (var f in fields) {
				var value = f.GetValue (this);
				if (f.FieldType.IsArray)
					value = ((Array) value)?.Clone ();
				else if (!f.FieldType.IsSubclassOf (typeof(ValueType)) &&
				         f.FieldType != typeof(string))
					continue;
				if (_configMap.TryGetValue (f.Name, out var oldValue)) {
					if (oldValue == null && value == null)
						continue;
					if (oldValue != null && value != null) {
						if (f.FieldType.IsArray) {
							// 比较数组内容是否发生改变
							if (((Array) oldValue).DeepEquals ((Array) value))
								continue;
						} else if (oldValue.Equals (value)) continue;
					}
					_configMap[f.Name] = value;
					_InvokeHandlers (f.Name, value);
					changed = true;
				} else {
					_configMap[f.Name] = value;
					_InvokeHandlers (f.Name, value);
					changed = true;
				}
			}
			return changed;
		}

		private void _InvokeHandlers (string fieldName, object value) {
			if (_listenerMap.TryGetValue (fieldName, out var handlers))
				handlers (value);
		}

		protected virtual void _Initialize () { }

	}

}