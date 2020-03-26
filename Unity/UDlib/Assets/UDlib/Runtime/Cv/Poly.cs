using System;
using System.Collections.Generic;
using UDlib.Core.Mathmatics;
using UnityEngine;

namespace UDlib.Cv {

	public class Poly : ICloneable {

		public int InChannels { get; }
		public int OutChannels => Ceofs.GetLength (0);
		public int Degrees { get; }
		public float MaxError { get; private set; }
		public float MeanError { get; private set; }
		public float[,] Ceofs { get; }

		public Poly (int degrees, int inChannels, float[,] a, float maxError, float meanError) {
			InChannels = inChannels;
			Degrees = degrees;
			Ceofs = a;
			MaxError = maxError;
			MeanError = meanError;
		}

		public float[] Evaluate (Matrix<float> x) {
			if (!(x.NCols == 1 && x.NRows == InChannels || x.NRows == 1 && x.NCols == InChannels))
				throw new ArgumentException (
					$"input must be a 1x{InChannels} or {InChannels}x1 vector", nameof(x));
			var x1 = x.NRows == 1 ? x : x.Reshape (1, InChannels);
			var items = GetItems (Degrees, x1);
			var val = new float[OutChannels];
			for (var c = 0; c < OutChannels; ++c) {
				val[c] = 0.0f;
				for (var i = 0; i < items.Length; ++i)
					val[c] += Ceofs[c, i] * items[i];
			}
			return val;
		}

		public float[] Evaluate (float[] x) => Evaluate (new Matrix<float> (x.Length, 1, x));

		public static Poly Fit (int degrees, int channels, Matrix<float> x, Matrix<float> y) {
			var nItems = GetItemCount (degrees, channels);
			Debug.Assert (x.NRows == y.NRows && x.NRows >= GetItemCount (degrees, channels));
			var nSamples = x.NRows;

			/* ********【预声明phy超定矩阵】************************/
			/* 多项式拟合的函数为多项幂函数
			 * f(x)=a0*x^2 + a1*y^2 + a2*xy + a3*x + a4*y + a5
			 *
			 * 超定矩阵phy=x1^2 y1^2 x1y1 x1 y1 1
			 *            x2^2 y2^2 x2y2 x2 y2 1
			 *              ... ... ... ...
			 *              ... ... ... ...
			 *            xn^2 yn^2 xnyn xn yn 1
			 *
			 * *************************************************/
			var phy = new Matrix<float> (nSamples, nItems);
			for (var i = 0; i < nSamples; i++) {
				var items = GetItems (degrees, x.GetRow (i));
				for (var j = 0; j < items.Length; ++j)
					phy[i, j] = items[j];
			}

			var a = new float[channels, nItems];
			for (var c = 0; c < channels; ++c) {
				var yc = y.GetColumn (c);
				var ac = new Matrix<float> (nItems, 1);
				Solver.SolveLinear (phy, yc, ac, (int) Solver.DecompTypes.DECOMP_NORMAL);
				for (var i = 0; i < nItems; ++i)
					a[c, i] = ac[i, 0];
			}

			// 构造PolyFunc对象
			var f = new Poly (degrees, channels, a, 0.0f, 0.0f);

			// 计算Max Error和RMSE
			for (var i = 0; i < nSamples; ++i) {
				var evalValue = f.Evaluate (x.GetRow (i));
				var sqrErr = 0.0f;
				for (var j = 0; j < evalValue.Length; ++j)
					sqrErr = (evalValue[j] - y[i, j]) * (evalValue[j] - y[i, j]);
				var err = Mathf.Sqrt (sqrErr);
				f.MaxError = Mathf.Max (err, f.MaxError);
				f.MeanError += sqrErr;
			}
			f.MeanError = Mathf.Sqrt (f.MeanError / nSamples);

			return f;
		}

		public static int GetItemCount (int degrees, int channels) {
			// n = \sum_d((L+d-1)!/((L-1)!d!)) = \sum_d((L*...*(L+d-1))/d!)
			var cFact = 1;
			var dFact = 1;
			var n = 1;
			for (var d = 1; d <= degrees; ++d) {
				cFact *= (channels - 1 + d);
				dFact *= d;
				n += cFact / dFact;
			}
			return n;
		}

		public static float[] GetItems (int degrees, Matrix<float> x) {
			// Precalculate different orders of input variable
			var var_n = new float[x.Size.Area (), degrees + 1];
			for (var c = 0; c < var_n.GetLength (0); ++c) {
				var_n[c, 0] = 1.0f;
				var_n[c, 1] = x[c];
				for (var order = 2; order <= degrees; ++order)
					var_n[c, order] = x[c] * var_n[c, order - 1];
			}

			var items = new float[GetItemCount (degrees, var_n.GetLength (0))];
			var i = 0;
			_GetItems (degrees, var_n, 0, 1.0f, items, ref i);
			return items;
		}

		public object Clone () {
			return new Poly (Degrees, InChannels,
				(float[,]) Ceofs.Clone (), MaxError, MeanError);
		}

		private static void _GetItems (int degrees, float[,] var_n, int c,
			float itemValue, IList<float> items, ref int i) {
			for (var order = degrees; order >= 0; --order) {
				if (c < var_n.GetLength (0) - 1)
					_GetItems (degrees - order, var_n, c + 1,
						itemValue * var_n[c, order], items, ref i);
				else
					items[i++] = itemValue * var_n[c, order];
			}
		}

	}

}