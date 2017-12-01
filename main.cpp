	#include <iostream>
	#include <iomanip>
	#include <regex>
	#include <string>
	#include <fstream>
	#include <list>
	#include <cmath>
	#include <cstdlib>
	#include "algorithms.h"
	#include "tables.h"

	using namespace std;

	/*
	 * TO-DO!
	 * Create Semaphore
	 * Implement Algorithms
	 * Create Disk Scheduler
	 */

	struct processObject {
		int pid = 0; //PID which is used as the address space/segment
		int size = 0; //Max frames that can be allocated to address space
		int currentFrameCount = 0; //Holds current frame count
	};

	struct memoryRequest {
		int addressSpace = 0; //Address Space/PID
		int segment = 0; //Segment #. In this case PID is the segment #.
		int page = 0; //Page #
		int offset = 0; //Offset
	};

	struct frame {
		int addressSpace = 0; //Address Space Number/PID
		int segment = 0; //Segment Number
		int page = 0; //Page Number
	};

	int main(int argc, char **argv) {
		ifstream commandFile(argv[1]);
	
		//Error Handling for input file.
		if (!commandFile.is_open()) {
			cerr << "Error opening file." << endl;
			exit(1);
		}
	
		int totalPages = 0; //Total amount of page frames
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

		getline(commandFile, bufferString);
		segmentLength = stoi(bufferString);

		getline(commandFile, bufferString);
		pageSize = stoi(bufferString);

		getline(commandFile, bufferString);
		framesPerProcess = stoi(bufferString);

		getline(commandFile, bufferString);
		x = stoi(bufferString);

		getline(commandFile, bufferString);
		min = stoi(bufferString);

		getline(commandFile, bufferString);
		max = stoi(bufferString);

		getline(commandFile, bufferString);
		processCount = stoi(bufferString);

		//Data Structures Needed
		processObject processList[processCount];
		list<memoryRequest> pageRequests;
		list<frame> framesTable;
	
		frame emptyFrame;
		framesTable.assign(totalPages, emptyFrame);
	
		for (int i = 0; i < processCount; i++) {
			getline(commandFile, bufferString); //Grab the line from the file
			regex_search(bufferString, matchResults, pidSize); //Grab PID and Size
		
			int tempPID = stoi(matchResults[1].str());
			int tempSize = stoi(matchResults[2].str());
		
			/*cout << "PID: " << matchResults[1].str() << " Size: "
			<< matchResults[2].str() << endl;*/
		
			processList[tempPID - 100].pid = tempPID;
			processList[tempPID - 100].size = tempSize;
		}
	
		cout << "Total Number of Pages: " << totalPages << " "
			 << "Segment Length: " << segmentLength << " "
			 << "Page Size: " << pageSize << " "
			 << "Frames Per Process: " << framesPerProcess << " "
			 << "X: " << x << " "
			 << "Min: " << min << " " << "Max: " << max << " "
			 << "Process Count: " << processCount << endl << endl;
	
		/*for (int i = 0; i < processCount; i++) {
			cout << "PID: " << processList[i].pid << " SIZE: " << processList[i].size << endl << endl;
		}*/
	
		while(getline(commandFile, bufferString)) {

			if (regex_search(bufferString, matchResults, end)) {
				memoryRequest temp;
				temp.addressSpace = stoi(matchResults[1].str());
				temp.offset = -1;
			
				pageRequests.insert(pageRequests.end(), temp);
			}
			else {
				regex_search(bufferString, matchResults, pidAddr);
			
				int tempAddress = stoi(matchResults[2].str(), nullptr, 16);
				cout << "HEX: 0x" << matchResults[2].str() << " DEC: " << tempAddress << endl;
			
				int segmentSize = (segmentLength * pageSize);
			
				memoryRequest temp;
			
				temp.addressSpace = stoi(matchResults[1].str());
				temp.segment = tempAddress / segmentSize;
				tempAddress -= (temp.segment * segmentSize);
				temp.page = tempAddress / pageSize;
				temp.offset = tempAddress - (temp.page * pageSize);
			
				pageRequests.insert(pageRequests.end(), temp);
			}
		}
		cout << endl;
	
		cout << "Request Count: " << pageRequests.size() << endl;
	
		for (list<memoryRequest>::iterator itr = pageRequests.begin(); itr != pageRequests.end(); ++itr) {
			cout << "PID: " << setw(3) << itr->addressSpace << " "
				 << "Segment: " << setw(3) << itr->segment << " "
				 << "Page: " << setw(2) << itr->page << " "
				 << "Offset: " << setw(3) << itr->offset
				 << endl;
		}
		cout << endl;
	
		cout << "Frames Table" << endl;
		for (list<frame>::iterator itr = framesTable.begin(); itr != framesTable.end(); ++itr) {
			cout << "Address Space: " << setw(4) << itr->addressSpace << " "
				 << "Segment #: " << setw(3) << itr->segment << " "
				 << "Page #: " << setw(3) << itr->page
				 << endl;
		}
	
		return 0;
	}

