using System;
using System.Collections.Generic;
using Sirenix.OdinInspector;
using UDlib.App.Exceptions;
using UDlib.Core;
using UnityEditor;
using UnityEngine;

namespace UDlib.App.Internal {

	public abstract class _App : SingletonBehavior<_App> {

#region Fields In Inspector

		/// <summary>
		/// 应用名称，在编辑器加载场景时自动设置到
		/// <see cref="PlayerSettings"/>.<see cref="PlayerSettings.productName"/>。
		/// </summary>
		[InfoBox ("$" + nameof(errorMessageInInspector), InfoMessageType.Error,
			nameof(_ShowErrorInInspector))]
		[LabelText ("应用名称"), DisableInPlayMode]
		public string applicationName;
		/// <summary>
		/// 应用程序当前模式
		/// </summary>
		[ShowInInspector, HideInEditorMode, HideInPrefabs]
		[LabelText ("当前模式"), ReadOnly]
		protected _Mode currentMode;
		/// <summary>
		/// 应用程序启动时将进入的模式
		/// </summary>
		[LabelText ("初始模式"), HideInPlayMode]
		public _Mode initialMode;

#endregion

		/// <summary>
		/// 模式对象集，包含系统中所有模式对象，以它们的名字作为索引。
		/// 模式对象会在模式基类<see cref="_Mode"/>.Awake()中通过<see cref="AddMode(_Mode)"/>注册自己。
		/// </summary>
		private readonly Dictionary<string, _Mode> _modes = new Dictionary<string, _Mode> ();
		protected string errorMessageInInspector;

		private bool _ShowErrorInInspector => !string.IsNullOrEmpty (errorMessageInInspector);

		/// <summary>
		/// 切换到指定模式
		/// </summary>
		/// 
		/// <param name="mode">目标模式</param>
		/// 
		/// <author>Nianchen Deng</author>
		protected void SwitchToMode (_Mode mode) {
			if (mode == null)
				return;
			if (!mode.CheckBeforeEnter ())
				return;
			if (currentMode != null)
				currentMode.Exit ();
			var lastMode = currentMode;
			currentMode = mode;
			try {
				currentMode.Enter ();
			} catch (Exception e) {
				// 进入新模式失败，返回之前的模式
				Debug.LogException (e);
				if (lastMode != null)
					lastMode.Enter ();
			}
		}

		/// <summary>
		/// 添加一个模式对象。
		/// <para>模式基类<see cref="_Mode"/>会在Awake()中调用该方法进行注册。</para>
		/// </summary>
		/// 
		/// <param name="mode">模式对象</param>
		///
		/// <author>Nianchen Deng</author>
		internal void AddMode (_Mode mode) => _modes.Add (mode.name, mode);

		/// <summary>
		/// 获取指定名称的模式对象。
		/// </summary>
		/// 
		/// <param name="modeName">模式名称</param>
		/// 
		/// <returns>模式对象</returns>
		/// <exception cref="ModeNotFound">找不到指定模式</exception>
		/// <author>Nianchen Deng</author>
		internal _Mode GetMode (string modeName) {
			if (!_modes.TryGetValue (modeName, out var mode))
				throw new ModeNotFound ($"Mode {modeName} is not found");
			return mode;
		}

	}

}