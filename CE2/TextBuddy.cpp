/**
* Program: TextBuddy.exe
*
* Requirements to run:
* - Microsoft Visual Studio runtime
*
* Command parsing assumptions:
* - No other commands other than Add, Clear, Display, Delete, Exit
* - Add only has one argument
* - Clear has no arguments
* - Display has no arguments
* - Delete has one integer argument
* - Exit has no arguments
*
* Command line assumptions:
* - Program is run using TextBuddy.exe targetFileName.exe
* - If only TextBuddy.exe is run, mytextfile.txt is used
*
* @author Nguyen Quoc Dat (A0116703N)
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

const enum COMMAND_TYPE {
	ADD, CLEAR, DELETE, DISPLAY, SORT, EXIT, INVALID
};
const string ERROR_NO_FILENAME = "No filename specified! Usage: %s <FILENAME>\nUsing default filename: %s\n";
const string ERROR_CANNOT_OPEN_FILE = "Cannot open file %s";
const string WELCOME_MESSAGE = "Welcome to TextBuddy. %s is ready for use\n";
const string OPERATION_SUCCESSFUL = "Operation successful.\n";
const string PRINT_BUFFER = "Print buffer.\n";
const string ADD_COMPLETE = "added to %s: \"%s\"\n";
const string DELETE_COMPLETE = "deleted from %s: \"%s\"\n";
const string SORT_COMPLETE = "%s has been sorted alphabetically.\n";
const string CLEAR_COMPLETE = "all content deleted from %s\n";
const string FILE_EMPTY = "%s is empty.\n";
const string WRONG_COMMAND_FORMAT = "Command error.\n";
const string PROMPT_FOR_COMMAND = "command: ";
const string DEFAULT_FILENAME = "mytextfile.txt";

class TextBuddy {
public:

	TextBuddy(string targetFilename = "") {
		targetFile = targetFilename;
	}

	string processCommand(string command) {
		string commandWord = getFirstWord(command);
		COMMAND_TYPE commandType = determineCommandType(commandWord);

		switch (commandType) {
		case(ADD) : {
			return addNewLine(command);
		} break;
		case(CLEAR) : {
			return clearFile(command);
		} break;
		case(DELETE) : {
			return deleteEntry(command);
		} break;
		case(DISPLAY) : {
			return display(command);
		} break;
		case (SORT) : {
			return sortAlphabetically(command);
		} break;
		case(EXIT) : {
			exit(0);
		} break;
		default: {
			return WRONG_COMMAND_FORMAT;
		} break;
		}
		return WRONG_COMMAND_FORMAT;
	}

	void initializeDatabase(char* argv[]) {
		if (targetFile == "") targetFile = argv[1];
		textData.clear();
		loadFile();
	}

	// Prints buffer to user
	string printBuffer(string message) {
		cout << message;
		return buffer;
	}

	// Gets the user input
	string getUserInput() {
		printBuffer(PROMPT_FOR_COMMAND);
		string userCommand;
		getline(cin, userCommand);
		return userCommand;
	}

	// Prepares to load the specified file. Supplies a default filename if user doesn't run
	// TextBuddy.exe with a filename in command line argument.
	void checkFile(int argc, char* argv[]) {
		if (argc < 2) {
			targetFile = DEFAULT_FILENAME;
			sprintf_s(buffer, ERROR_NO_FILENAME.c_str(), argv[0], DEFAULT_FILENAME.c_str());
			printBuffer(buffer);
		}
		else {
			sprintf_s(buffer, WELCOME_MESSAGE.c_str(), argv[0]);
			printBuffer(buffer);
		}
	}

	// Method for testing. Make it private later!
	vector<string>* giveData() {
		return &textData;
	}

private:

	string targetFile;
	vector<string> textData;
	char buffer[255];

	// I-O methods
	void loadFile() {
		ifstream physicalFile(targetFile);

		while (physicalFile) {
			string inputLine;
			getline(physicalFile, inputLine);
			textData.push_back(inputLine);
		}
		if (textData.size() > 0) textData.pop_back(); //because of extra line at endfile
	}

	void rewriteFile() {
		ofstream physicalFile(targetFile);
		for (vector<string>::const_iterator i = textData.begin(); i != textData.end(); ++i) {
			physicalFile << *i << "\n";
		}
		physicalFile.close();
	}

	void insertToFile(string toBeInserted) {
		textData.push_back(toBeInserted);
		rewriteFile();
	}

	void clearAllData() {
		textData.clear();
		rewriteFile();
	}

	void deleteFromFile(int index) {
		textData.erase(textData.begin() + index);
		rewriteFile();
	}

	// Individual command methods
	string addNewLine(string command) {
		int firstWordPos = firstSpacePosition(command);
		if (firstWordPos == -1) return WRONG_COMMAND_FORMAT;

		string toBeInserted = command.substr(firstWordPos + 1);
		insertToFile(toBeInserted);

		sprintf_s(buffer, ADD_COMPLETE.c_str(), targetFile.c_str(), toBeInserted.c_str());

		return buffer;
	}

	string clearFile(string command) {
		int firstWordPos = firstSpacePosition(command);
		if (firstWordPos != -1) return WRONG_COMMAND_FORMAT;

		clearAllData();

		sprintf_s(buffer, CLEAR_COMPLETE.c_str(), targetFile.c_str());

		return buffer;
	}

	string display(string command) {
		int firstWordPos = firstSpacePosition(command);
		if (firstWordPos != -1) return WRONG_COMMAND_FORMAT;

		if (textData.size() == 0) {
			sprintf_s(buffer, FILE_EMPTY.c_str(), targetFile.c_str());
			return buffer;
		}

		int index = 0;
		for (vector<string>::const_iterator i = textData.begin(); i != textData.end(); ++i) {
			index++;
			cout << index << ". " << *i << "\n";
		}

		return OPERATION_SUCCESSFUL;
	}

	string sortAlphabetically(string command) {
		int firstWordPos = firstSpacePosition(command);
		if (firstWordPos != -1) return WRONG_COMMAND_FORMAT;

		textData.shrink_to_fit();
		quickSort(&textData[0], 0, textData.size()-1);

		sprintf_s(buffer, SORT_COMPLETE.c_str(), targetFile.c_str());
		return buffer;
	}

	string deleteEntry(string command) {
		int firstWordPos = firstSpacePosition(command);
		if (firstWordPos == -1) return WRONG_COMMAND_FORMAT;

		int index = atoi(command.substr(firstWordPos).c_str()) - 1;
		if (index < 0 || index > textData.size()) return WRONG_COMMAND_FORMAT;
		string toBeDeleted = textData[index];

		deleteFromFile(index);

		sprintf_s(buffer, DELETE_COMPLETE.c_str(), targetFile.c_str(), toBeDeleted.c_str());

		return buffer;
	}

	// Command processing methods
	COMMAND_TYPE determineCommandType(string commandWord) {
		if (commandWord == "add") {
			return ADD;
		}
		else if (commandWord == "clear") {
			return CLEAR;
		}
		else if (commandWord == "delete") {
			return DELETE;
		}
		else if (commandWord == "display") {
			return DISPLAY;
		}
		else if (commandWord == "sort") {
			return SORT;
		}
		else if (commandWord == "exit" || commandWord == "") {
			return EXIT;
		}
		else {
			return INVALID;
		}
	}

	// Auxiliary functions

	static int firstSpacePosition(string command) {
		return command.find_first_of(" \n\r\f");
	}

	static string getFirstWord(string command) {
		int firstWordPos = firstSpacePosition(command);
		if (firstWordPos == -1) return command;
		return command.substr(0, firstWordPos);
	}

	/*static string toLowerCase(string mixedCaseString) {
		for (in )
	}*/

	// Basic quickSort implementation
	void quickSort(string unsortedArray[], int low, int high) {
		int i = low;
		int j = high;
		string pivot = unsortedArray[low + (high - low) / 2];

		while (i < j) {
			while (unsortedArray[i] < pivot) {
				i++;
			}
			while (unsortedArray[j] > pivot) {
				j--;
			}

			if (i <= j) {
				string temp = unsortedArray[i];
				unsortedArray[i] = unsortedArray[j];
				unsortedArray[j] = temp;
			}
		}

		if (low < j) {
			quickSort(unsortedArray, low, j - 1);
		}

		if (high > i) {
			quickSort(unsortedArray, i + 1, high);
		}
	}
};


int main(int argc, char* argv[]) {

	TextBuddy* currSession = new TextBuddy();

	currSession->checkFile(argc, argv);
	currSession->initializeDatabase(argv);

	while (true) {
		string result = currSession->processCommand(currSession->getUserInput());
		if (result != OPERATION_SUCCESSFUL) {
			currSession->printBuffer(result);
		}
	}
	return 0;
}