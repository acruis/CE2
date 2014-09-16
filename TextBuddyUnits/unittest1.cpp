#include "stdafx.h"
#include "CppUnitTest.h"
#include "TextBuddy.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TextBuddyUnits
{		
	TEST_CLASS(TextBuddyAtd)
	{
	public:
		
		TEST_METHOD(testInitialize)
		{
			TextBuddy* testSession = new TextBuddy("testFile.txt"); 
			string result = testSession->processCommand("display");
			Assert::AreEqual("testFile.txt is empty.\n", result.c_str());
		}

	};
}
