using UDlib.App;
using UnityEngine;

namespace UDlib.Samples.SimpleApp {

	[RequireComponent (typeof(ServiceA), typeof(ServiceC))]
	public class ServiceB : Service<ServiceB> {
		
	}

}