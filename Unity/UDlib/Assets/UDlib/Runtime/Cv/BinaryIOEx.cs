using System.IO;

namespace UDlib.Cv {

	public static class BinaryIOEx {
		
		public static Poly ReadPoly (this BinaryReader reader) {
			var degrees = reader.ReadInt32 ();
			var inChannels = reader.ReadInt32 ();
			var outChannels = reader.ReadInt32 ();
			var countA = reader.ReadInt32 ();
			var a = new float[outChannels, countA];
			for (int c = 0; c < outChannels; ++c)
			for (int i = 0; i < countA; ++i)
				a[c, i] = reader.ReadSingle ();
			var maxError = reader.ReadSingle ();
			var meanError = reader.ReadSingle ();
			return new Poly (degrees, inChannels, a, maxError, meanError);
		}

		public static void Write (this BinaryWriter writer, Poly val) {
			writer.Write (val.Degrees);
			writer.Write (val.InChannels);
			writer.Write (val.OutChannels);
			writer.Write (val.Ceofs.GetLength (1));
			for (int c = 0; c < val.Ceofs.GetLength (0); ++c)
			for (int i = 0; i < val.Ceofs.GetLength (1); ++i)
				writer.Write (val.Ceofs[c, i]);
			writer.Write (val.MaxError);
			writer.Write (val.MeanError);
		}

	}

}