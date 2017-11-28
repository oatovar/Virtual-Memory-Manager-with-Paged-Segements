#include <iostream>
#include <regex>
#include <string>
#include <fstream>

using namespace std;

struct processObject {
	int pid = 0;
	char temp[2];
};

int main(int argc, char **argv) {
	ifstream commandFile(argv[1]);
	
	//Error Handling for input file.
	if (!commandFile.is_open()) {
		cerr << "Error opening file." << endl;
		exit(1);
	}
	
	int totalPages = 0; //Total amount of pages
	int segmentLength = 0; //Segment length
	int pageSize = 0; //Size of a page
	int framesPerProcess = 0; //Frames per process
	int x = 0; //Used for LRU-X and OPT-X
	int min = 0, max = 0;
	int processCount = 0;
	
	string bufferString = ""; //Used for parsing
	regex pidSize("(\\d+)\\s+(\\d+)", regex::ECMAScript);
	regex pidAddr("(\\d+)\\s+0x([A-F0-9]+)", regex::ECMAScript);
	regex end("(\\d+)\\s(-1)", regex::ECMAScript);
	smatch matchResults;
	
	getline(commandFile, bufferString);
	totalPages = stoi(bufferString);
	cout << totalPages << endl;
	getline(commandFile, bufferString);
	segmentLength = stoi(bufferString);
	cout << segmentLength << endl;
	getline(commandFile, bufferString);
	pageSize = stoi(bufferString);
	cout << pageSize << endl;
	getline(commandFile, bufferString);
	framesPerProcess = stoi(bufferString);
	cout << framesPerProcess << endl;
	getline(commandFile, bufferString);
	x = stoi(bufferString);
	cout << x << endl;
	getline(commandFile, bufferString);
	min = stoi(bufferString);
	cout << min << endl;
	getline(commandFile, bufferString);
	max = stoi(bufferString);
	cout << max << endl;
	getline(commandFile, bufferString);
	processCount = stoi(bufferString);
	cout << processCount << endl;
	
	processObject processList[processCount];
	
	for (int i = 0; i < processCount; i++) {
		//getline(commandFile, bufferString);
		//cout << bufferString << endl;
		getline(commandFile, bufferString);
		regex_search(bufferString, matchResults, pidSize);
		cout << "PID: " << matchResults[1].str() << " Size: "
		<< matchResults[2].str() << endl;
	}
	
	while(getline(commandFile, bufferString)) {
		//cout << bufferString << endl;
		if (regex_search(bufferString, matchResults, end)) {
			cout << "PID: " << matchResults[1].str() <<
			" Result: " << matchResults[2].str() << endl;			
		}
		else {
			regex_search(bufferString, matchResults, pidAddr);
			for (int i = 1; i < matchResults.size(); i++) {
				cout << matchResults[i].str() << " ";
			}
			cout << endl;
		}
	}
	
	return 0;
}
