using UDlib.App;
using UDlib.App.Exceptions;

namespace UDlib.Samples.SimpleApp {

	public class ServiceA : Service<ServiceA> {

		public int a;
		private int startCountdown = 3;
		
		protected override void _Startup () {
			if (startCountdown-- > 0)
				throw new ServiceException ("Countdown is not zero");
		}

	}

}