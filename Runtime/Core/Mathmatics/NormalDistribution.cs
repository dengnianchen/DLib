using System;
using UnityEngine;

namespace UDlib.Core.Mathmatics {

	[Serializable]
	public struct NormalDistribution : IEquatable<NormalDistribution> {

		public readonly float sigma;
		public readonly float mu;

		public NormalDistribution (float sigma, float mu) {
			this.sigma = sigma;
			this.mu = mu;
		}

		public float GetValue (float x) {
			return 1.0f / (sigma * Mathf.Sqrt (2.0f * Mathf.PI)) *
			       Mathf.Exp (-Mathf.Pow (x - mu, 2.0f) / (2.0f * sigma * sigma));
		}

		public bool Equals (NormalDistribution other) {
			return sigma.Equals (other.sigma) && mu.Equals (other.mu);
		}

		public override bool Equals (object obj) {
			return obj is NormalDistribution other && Equals (other);
		}

		public override int GetHashCode () {
			unchecked {
				return (sigma.GetHashCode () * 397) ^ mu.GetHashCode ();
			}
		}

	}

}