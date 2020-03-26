using UDlib.Core.Image;
using UDlib.Cv;
using UnityEngine;
using Random = UnityEngine.Random;

namespace UDlib.Samples.CvPainter {

	public class PaintExample : MonoBehaviour, IImageSource {

		private TexImage _image;
		
		public void Awake () => this.AddPreviewComponent ();

		public void Start () {
			_image = TexImage.Create (new Vector2Int (800, 600), PixelFormat.EFormat.BGR32);
			var painter = new Painter (_image);
			painter.Fill (new Color32 (255, 255, 255, 100));
			painter.Line (new Vector2 (10, 10), new Vector2 (100, 10), Color.red);
			painter.Line (new Vector2 (10, 10), new Vector2 (10, 100), Color.green, 2);
			painter.Triangle (new Vector2 (100, 10), 5, 0.0f, Color.red, -1);
			painter.Triangle (new Vector2 (10, 100), 5, 90.0f, Color.green, -1);
			painter.Text ("X", new Vector2Int (110, 20), Painter.Font.HERSHEY_PLAIN, 2.0f,
				Color.red, 1);
			painter.Text ("Y", new Vector2Int (5, 130), Painter.Font.HERSHEY_PLAIN, 2.0f,
				Color.green, 1);
			painter.Rectangle (new Rect (150, 150, 100, 200), Color.black, -1);
			painter.Rectangle (new Rect (150, 150, 100, 200), Color.magenta, 3);
			painter.Marker (new Vector2 (200, 250), Painter.MarkerType.STAR, 30,
				new Color32 (255, 126, 0, 255), 2);
			var markerPositions = new[] {
				new Vector2 (Random.Range (300, 750), Random.Range (50, 550)),
				new Vector2 (Random.Range (300, 750), Random.Range (50, 550)),
				new Vector2 (Random.Range (300, 750), Random.Range (50, 550)),
				new Vector2 (Random.Range (300, 750), Random.Range (50, 550)),
				new Vector2 (Random.Range (300, 750), Random.Range (50, 550)),
				new Vector2 (Random.Range (300, 750), Random.Range (50, 550))
			};
			painter.Markers (markerPositions, Painter.MarkerType.DIAMOND, 10, Color.yellow);
			var polylines = new[] {
				new[] { new Vector2 (350, 100), new Vector2 (425, 200), new Vector2 (360, 300) },
				new[] {
					new Vector2 (521, 322), new Vector2 (618, 294), new Vector2 (425, 309),
					new Vector2 (512, 323)
				}
			};
			foreach (var polyline in polylines) {
				painter.Polyline (polyline, new Color32 (0, 88, 255, 255), 2);
				painter.Polyline (polyline, new Color32 (94, 200, 255, 255), -1, false);
			}
		}

		public string[] GetChannelIdentifiers () => new[] { "Main" };

		public TexImage GetImage (int channelId) => channelId == 0 ? _image : null;

	}

}