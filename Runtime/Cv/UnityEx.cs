using UnityEngine;

namespace UDlib.Cv {

	public static class UnityEx {

		public static void SetProjection (this Camera c, PerspectiveProjection pp) =>
			c.projectionMatrix = pp.GetUnityProjectionMatrix (c.nearClipPlane, c.farClipPlane);

		public static PerspectiveProjection GetProjection (this Camera c) =>
			new PerspectiveProjection (c.projectionMatrix,
				new Vector2 (c.pixelWidth, c.pixelHeight));

	}

}