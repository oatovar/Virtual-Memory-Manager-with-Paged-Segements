#ifndef STRUCTS
#define STRUCTS

using namespace std;

struct processObject {
	int pageFault = 0; //Total amount of page faults that have occurred.
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
	int time = 0; //Time since last used
	int futureCounter = 0;
};

#endif
