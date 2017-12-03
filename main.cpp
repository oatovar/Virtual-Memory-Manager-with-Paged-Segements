	#include <iostream>
	#include <iomanip>
	#include <regex>
	#include <string>
	#include <fstream>
	#include <list>
	#include <map>
	#include <cmath>
	#include <cstdlib>
	#include "structures.h"
	#include "tables.h"
	#include "algorithms.h"

	using namespace std;

	/*
	 * TO-DO!
	 * Create Semaphore
	 * Implement Algorithms
	 * Create Disk Scheduler
	 * Need to modify it so that Segment Tables are created by max amount of segments needed.
	 * Also need to randomly generate new physical addresses for each of the segment tables.
	 * When generating them must make sure that there are no duplicate addresses
	 * Can use a hash map for this easily
	 */

	int main(int argc, char **argv) {
		ifstream commandFile(argv[1]);
		srand(time(NULL));
	
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
		
		list<SegmentTable> SegmentTables;
		
		for (int i = 0; i < processCount; i++) {
			SegmentTable tempTable;
			tempTable.addressSpace = 100 + i;
			SegmentTables.insert(SegmentTables.end(), tempTable);
		}
	
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
	
		cout << "Total Number of Pages: " << totalPages << " | "
			 << "Segment Length: " << segmentLength << " | "
			 << "Page Size: " << pageSize << " | "
			 << "Frames Per Process: " << framesPerProcess << " | "
			 << "X: " << x << " | "
			 << "Min: " << min << " | " << "Max: " << max << " | "
			 << "Process Count: " << processCount << endl;
	
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
				int segmentSize = (segmentLength * pageSize);
				//cout << "HEX: 0x" << matchResults[2].str() << " DEC: " << tempAddress << endl;
			
				memoryRequest temp;
				temp.addressSpace = stoi(matchResults[1].str());
				temp.segment = tempAddress / segmentSize;
				tempAddress -= (temp.segment * segmentSize);
				temp.page = tempAddress / pageSize;
				temp.offset = tempAddress - (temp.page * pageSize);
			
				pageRequests.insert(pageRequests.end(), temp);
			}
		}
		/*cout << endl;
	
		cout << "Request Count: " << pageRequests.size() << endl;
		int index = 0;
		for (list<memoryRequest>::iterator itr = pageRequests.begin(); itr != pageRequests.end(); ++itr) {
			index++;
			cout << "Index: " << setw(3) << index << " "
				 << "PID: " << setw(3) << itr->addressSpace << " "
				 << "Segment: " << setw(3) << itr->segment << " "
				 << "Page: " << setw(2) << itr->page << " "
				 << "Offset: " << setw(3) << itr->offset
				 << endl;
		}
		cout << endl;*/
		
		//This shows the Segment Tables created with their respective Page Table and Page addresses
		/*for (list<SegmentTable>::iterator itr = SegmentTables.begin(); itr != SegmentTables.end(); itr++) {
			cout << "Segment Table" << endl;
			itr->print();
		}*/
		
		fifo(framesTable, pageRequests, processList, framesPerProcess, processCount);
		lifo(framesTable, pageRequests, processList, framesPerProcess, processCount);
		ldf(framesTable, pageRequests, processList, framesPerProcess, processCount);
	
		return 0;
	}

