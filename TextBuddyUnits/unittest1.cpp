#include "stdafx.h"
#include "CppUnitTest.h"
#include "TextBuddy.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TextBuddyUnits
{		
	TEST_CLASS(TextBuddyAtd)
	{
	public:
		
		// Tests if the database is initialized correctly
		TEST_METHOD(testInitialize) {
			TextBuddy* testSession = new TextBuddy("testFile.txt"); 

			string result = testSession->processCommand("display");
			Assert::AreEqual("testFile.txt is empty.\n", result.c_str());
		}

		// Tests for response to bad command
		TEST_METHOD(testBadCommand) {
			TextBuddy* testSession = new TextBuddy("testFile.txt");
			string result = testSession->processCommand("brains");
			Assert::AreEqual(WRONG_COMMAND_FORMAT, result);
		}

		// Tests for adding items to list
		TEST_METHOD(testAddSuccess) {
			TextBuddy* testSession = new TextBuddy("testFile.txt");

			string result1 = testSession->processCommand("add some water");
			Assert::AreEqual("added to testFile.txt: \"some water\"\n", result1.c_str());

			string result2 = testSession->processCommand("add some more water");
			Assert::AreEqual("added to testFile.txt: \"some more water\"\n", result2.c_str());

			vector<string>* result3 = testSession->giveData();
			unsigned expectedSize = 2;
			Assert::AreEqual(expectedSize, result3->size());
			Assert::AreEqual("some water", result3->at(0).c_str());
			Assert::AreEqual("some more water", result3->at(1).c_str());
		}

		// Tests for bad input in adding items to list
		TEST_METHOD(testAddFail) {
			TextBuddy* testSession = new TextBuddy("testFile.txt");
			string result = testSession->processCommand("add");
			Assert::AreEqual(WRONG_COMMAND_FORMAT, result);
		}

		// Tests for deleting items from list
		TEST_METHOD(testDeleteSuccess) {
			TextBuddy* testSession = new TextBuddy("testFile.txt");

			testSession->processCommand("add some water");
			testSession->processCommand("add some more water");
			testSession->processCommand("add orange juice");

			string result1 = testSession->processCommand("delete 2");
			Assert::AreEqual("deleted from testFile.txt: \"some more water\"\n", result1.c_str());

			vector<string>* result2 = testSession->giveData();
			unsigned expectedSize = 2;
			Assert::AreEqual(expectedSize, result2->size());
			Assert::AreEqual("some water", result2->at(0).c_str());
			Assert::AreEqual("orange juice", result2->at(1).c_str());

			string result3 = testSession->processCommand("delete 1");
			Assert::AreEqual("deleted from testFile.txt: \"some water\"\n", result3.c_str());
			expectedSize = 1;
			Assert::AreEqual(expectedSize, result2->size());
			Assert::AreEqual("orange juice", result2->at(0).c_str());
		}

		// Tests for bad input in deleting items from list
		TEST_METHOD(testDeleteFail) {
			TextBuddy* testSession = new TextBuddy("testFile.txt");

			string result1 = testSession->processCommand("delete");
			Assert::AreEqual(WRONG_COMMAND_FORMAT, result1);

			string result2 = testSession->processCommand("delete 2");
			Assert::AreEqual(WRONG_COMMAND_FORMAT, result2);
		}

		// Tests for clearing the database
		TEST_METHOD(testClearSuccess) {
			TextBuddy* testSession = new TextBuddy("testFile.txt");

			testSession->processCommand("add some water");
			testSession->processCommand("add some more water");
			testSession->processCommand("add orange juice");

			string result1 = testSession->processCommand("clear");
			Assert::AreEqual("all content deleted from testFile.txt\n", result1.c_str());

			string result2 = testSession->processCommand("display");
			Assert::AreEqual("testFile.txt is empty.\n", result2.c_str());

			vector<string>* result3 = testSession->giveData();
			unsigned expectedSize = 0;
			Assert::AreEqual(expectedSize, result3->size());
		}
	};
}
