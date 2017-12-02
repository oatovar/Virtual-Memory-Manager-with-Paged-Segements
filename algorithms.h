#ifndef ALGORITHMS
#define ALGORITHMS

#include <iostream>
#include <iomanip>
#include <list>

using namespace std;

void reset(list<frame> currentFrames, processObject processProperties[], int k)
{
	list<frame>::iterator frameItr = currentFrames.begin();
	//Reset the frames table to simulate a fresh start
	while(frameItr != currentFrames.end()) {
		frameItr->addressSpace = 0;
		frameItr->segment = 0;
		frameItr->page = 0;
		frameItr++;
	}
	//Reset process objects to reinitialize frame count and page faults to zero
	for (int i = 0; i < k; i++) {
		processProperties[i].pageFault = 0;
		processProperties[i].currentFrameCount = 0;
	}
	return;
}

void printFramesTable(list<frame> currentFrames)
{
	cout << endl << "Frames Table" << endl;
	for (list<frame>::iterator itr = currentFrames.begin(); itr != currentFrames.end(); ++itr) {
		cout << "Address Space: " << setw(4) << itr->addressSpace << " "
			 << "Segment #: " << setw(3) << itr->segment << " "
			 << "Page #: " << setw(3) << itr->page
			 << endl;
	}
	cout << endl;
}

void fifo(list<frame> currentFrames, list<memoryRequest> currentRequests, processObject processProperties[], int framesPerProcess, int k)
{
	reset(currentFrames, processProperties, k);
	cout << endl << "Starting FIFO replacement algorithm." << endl;
	list<memoryRequest>::iterator requestItr = currentRequests.begin();
	list<frame>::iterator frameItr = currentFrames.begin();
	int totalPageFaults = 0;
	bool found = false;
	//int addressSpace = requestItr->addressSpace - 100;
	//printFramesTable(currentFrames);
	while (requestItr != currentRequests.end()) {
		found = false;
		if (requestItr->offset != -1) {
			if (processProperties[requestItr->addressSpace - 100].currentFrameCount < framesPerProcess) {
				while(frameItr != currentFrames.end()) {
                    if ((frameItr->addressSpace == requestItr->addressSpace) && (frameItr->segment == requestItr->segment) && (frameItr->page == requestItr->page)) {
                        //Frame already contains page needed. No need to use a new frame buffer.
                        //cout << "Page in frame buffer." << endl;
                        found = true;
                        frameItr = currentFrames.begin();
                        requestItr++;
                        break;
                    }
                    else {
                        //If frame occupied by another page either by the same process or another process
                        frameItr++; //Advance to next frame. In this case moves on to an older frame.
                    }
				}
				frameItr = currentFrames.begin();
				while((frameItr != currentFrames.end()) && (found == false)) {
					if (frameItr->addressSpace == 0) {
                        //Copy page to the frame buffer
                        //cout << "Copying page into frame buffer." << endl;
                        frameItr->addressSpace = requestItr->addressSpace;
                        frameItr->segment = requestItr->segment;
                        frameItr->page = requestItr->page;
                        //Increase the count of frames that currently belong to the process
                        processProperties[requestItr->addressSpace - 100].currentFrameCount++;
                        processProperties[requestItr->addressSpace - 100].pageFault++;
                        //Break out of loop and move on to the next request
                        found = true;
                        frameItr = currentFrames.begin();
                        requestItr++;
                        break;
                    }
                    else {
                    	frameItr++;
                    }
            	}
			}
            else {
            	frameItr = currentFrames.begin();
            	while(frameItr != currentFrames.end()) {
					if ((frameItr->addressSpace == requestItr->addressSpace) && (frameItr->segment == requestItr->segment) && (frameItr->page == requestItr->page)) {
						//Frame already contains page needed. No need to use a new frame buffer.
						//cout << "Page in frame buffer." << endl;
						found = true;
						frameItr = currentFrames.begin();
						requestItr++;
						break;
					}
					else {
						frameItr++;
					}
				}
				frameItr = currentFrames.begin();
				while(frameItr != currentFrames.end() && (found == false)) {
					if (frameItr->addressSpace == requestItr->addressSpace) {
						//cout << "Replacing frame buffer." << endl;
			            //Remove the first page that came in
			            frameItr = currentFrames.erase(frameItr);
			            //Update frames table to reflect the new order of pages in frame buffers
			            frame newFrame;
			            newFrame.addressSpace = requestItr->addressSpace;
			            newFrame.segment = requestItr->segment;
			            newFrame.page = requestItr->page;
			            currentFrames.push_back(newFrame);
			            processProperties[requestItr->addressSpace - 100].pageFault++;
			            //Break out of loop and move on to the next request
			            found = true;
			            frameItr = currentFrames.begin();
			            requestItr++;
             		    break;
            	    }
            	    else {
            	        frameItr++;//Go to the next frame. Since it's LIFO we move to an older frame.
            	    }
                }
            }
		}
        //If the request does not belong to the process move to the next process
        else {
        	//cout << "Process " << requestItr->addressSpace << " finished!" << endl;
        	frameItr = currentFrames.begin();
			requestItr++;
		}
		//printFramesTable(currentFrames);
	}
	cout << "RESULTS" << endl;
	for (int i = 0; i < k; i++) {
		cout << "Page Faults for Process " << i + 100 << ": "
			 << processProperties[i].pageFault << endl;
		totalPageFaults += processProperties[i].pageFault;
	}
	cout << "Total Page Faults: " << totalPageFaults << endl;
}

void lru()
{

}

void lifo(list<frame> currentFrames, list<memoryRequest> currentRequests, processObject processProperties[], int framesPerProcess, int k)
{
	reset(currentFrames, processProperties, k);
	cout << endl << "Starting LIFO replacement algorithm." << endl;
    list<memoryRequest>::iterator requestItr = currentRequests.begin();
    list<frame>::iterator frameItr = currentFrames.begin();
    int totalPageFaults = 0;
    bool found = false;
    //int addressSpace = requestItr->addressSpace - 100;
    while (requestItr != currentRequests.end()) {
    	found = false;
        if (requestItr->offset != -1) {
            if (processProperties[requestItr->addressSpace - 100].currentFrameCount < framesPerProcess) {
				while(frameItr != currentFrames.end()) {
                    if ((frameItr->addressSpace == requestItr->addressSpace) && (frameItr->segment == requestItr->segment) && (frameItr->page == requestItr->page)) {
                        //Frame already contains page needed. No need to use a new frame buffer.
                        //cout << "Page in frame buffer." << endl;
                        found = true;
                        frameItr = currentFrames.begin();
                        requestItr++;
                        break;
                    }
                    else {
                        //If frame occupied by another page either by the same process or another process
                        frameItr++; //Advance to next frame. In this case moves on to an older frame.
                    }
				}
				frameItr = currentFrames.begin();
				while((frameItr != currentFrames.end()) && (found == false)) {
					if (frameItr->addressSpace == 0) {
                        //Copy page to the frame buffer
                        //cout << "Copying page into frame buffer." << endl;
                        frameItr->addressSpace = requestItr->addressSpace;
                        frameItr->segment = requestItr->segment;
                        frameItr->page = requestItr->page;
                        //Increase the count of frames that currently belong to the process
                        processProperties[requestItr->addressSpace - 100].currentFrameCount++;
                        processProperties[requestItr->addressSpace - 100].pageFault++;
                        //Break out of loop and move on to the next request
                        found = true;
                        frameItr = currentFrames.begin();
                        requestItr++;
                        break;
                    }
                    else {
                    	frameItr++;
                    }
            	}
            }
            else {
            	frameItr = currentFrames.begin();
            	while(frameItr != currentFrames.end()) {
					if ((frameItr->addressSpace == requestItr->addressSpace) && (frameItr->segment == requestItr->segment) && (frameItr->page == requestItr->page)) {
						//Frame already contains page needed. No need to use a new frame buffer.
						//cout << "Page in frame buffer." << endl;
						found = true;
						frameItr = currentFrames.begin();
						requestItr++;
						break;
					}
					else {
						frameItr++;
					}
				}
				frameItr = currentFrames.begin();
				while((frameItr != currentFrames.end()) && (found == false)) {
					if (frameItr->addressSpace == requestItr->addressSpace) {
						//cout << "Replacing frame buffer." << endl;
			            //Remove the first page that came in
			            frameItr = currentFrames.erase(frameItr);
			            //Update frames table to reflect the new order of pages in frame buffers
			            frame newFrame;
			            newFrame.addressSpace = requestItr->addressSpace;
			            newFrame.segment = requestItr->segment;
			            newFrame.page = requestItr->page;
			            currentFrames.push_front(newFrame);
			            processProperties[requestItr->addressSpace - 100].pageFault++;
			            //Break out of loop and move on to the next request
			            found = true;
			            frameItr = currentFrames.begin();
			            requestItr++;
             		    break;
            	    }
            	    else {
            	        frameItr++;//Go to the next frame. Since it's LIFO we move to an older frame.
            	    }
                }
            }
        }
        //If the request does not belong to the process move to the next process
        else {
        	//cout << "Process " << requestItr->addressSpace << " finished!" << endl;
        	frameItr = currentFrames.begin();
            requestItr++;
        }
    }
	cout << "RESULTS" << endl;
	for (int i = 0; i < k; i++) {
		cout << "Page Faults for Process " << i + 100 << ": "
			 << processProperties[i].pageFault << endl;
		totalPageFaults += processProperties[i].pageFault;
	}
	cout << "Total Page Faults: " << totalPageFaults << endl;
}

void ldf(list<frame> currentFrames, list<memoryRequest> currentRequests, processObject processProperties[], int framesPerProcess, int k)
{
	reset(currentFrames, processProperties, k);
	cout << endl << "Starting LIFO replacement algorithm." << endl;
	list<frame>::iterator frameItr = currentFrames.begin();
	list<memoryRequest>::iterator requestItr = currentRequests.begin();
	int totalPageFaults = 0;
	bool found = false;
	while(requestItr != currentRequests.end()) {
		if (requestItr->offset != -1) {
            if (processProperties[requestItr->addressSpace - 100].currentFrameCount < framesPerProcess) {
				while(frameItr != currentFrames.end()) {
                    if ((frameItr->addressSpace == requestItr->addressSpace) && (frameItr->segment == requestItr->segment) && (frameItr->page == requestItr->page)) {
                        //Frame already contains page needed. No need to use a new frame buffer.
                        //cout << "Page in frame buffer." << endl;
                        found = true;
                        frameItr = currentFrames.begin();
                        requestItr++;
                        break;
                    }
                    else {
                        //If frame occupied by another page either by the same process or another process
                        frameItr++; //Advance to next frame. In this case moves on to an older frame.
                    }
				}
				frameItr = currentFrames.begin();
				while((frameItr != currentFrames.end()) && (found == false)) {
					if (frameItr->addressSpace == 0) {
                        //Copy page to the frame buffer
                        //cout << "Copying page into frame buffer." << endl;
                        frameItr->addressSpace = requestItr->addressSpace;
                        frameItr->segment = requestItr->segment;
                        frameItr->page = requestItr->page;
                        //Increase the count of frames that currently belong to the process
                        processProperties[requestItr->addressSpace - 100].currentFrameCount++;
                        processProperties[requestItr->addressSpace - 100].pageFault++;
                        //Break out of loop and move on to the next request
                        found = true;
                        frameItr = currentFrames.begin();
                        requestItr++;
                        break;
                    }
                    else {
                    	frameItr++;
                    }
            	}
            }
            else {
            
            }
		}
		else {
        	frameItr = currentFrames.begin();
            requestItr++;
		}            
	}
	cout << "RESULTS" << endl;
	for (int i = 0; i < k; i++) {
		cout << "Page Faults for Process " << i + 100 << ": "
			 << processProperties[i].pageFault << endl;
		totalPageFaults += processProperties[i].pageFault;
	}
	cout << "Total Page Faults: " << totalPageFaults << endl;
}

void opt()
{
}

void workingSet()
{
}

#endif
