using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using ServiceAreaClientLib.DeviceInquirer;

namespace ServiceAreaUnitTestProject
{
	[TestClass]
	public class UnitTest1
	{
		[TestMethod]
		public void Test_SaveToLocalFile()
		{
			DeviceInquirer di = new DeviceInquirer();
			di.SaveToLocalFile("QNNND?");
			di.SaveToLocalFile("lalala?");
			di.SaveToLocalFile("hahaha?");
			Assert.AreEqual(2, 1 + 1);
		}
	}
}
