﻿using System;

 namespace UDlib.App.Exceptions {

	public class ServiceException : Exception {

		public ServiceException () : base ("An error occurred in service") { }

		public ServiceException (string message) : base (message) { }

		public ServiceException (string message, Exception innerException) :
			base (message, innerException) { }

	}

}