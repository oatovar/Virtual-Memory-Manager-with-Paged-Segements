#ifndef ALGORITHMS
#define ALGORITHMS

#include <iostream>
#include <iomanip>
#include <list>

using namespace std;

void fifo(list<frame> currentFrames, list<memoryRequest> currentRequests, processObject processProperties[], int framesPerProcess, int processNum)
{
	list<memoryRequest>::iterator requestItr = currentRequests.begin();
	list<frame>::iterator frameItr = currentFrames.begin();
	int addressSpace = requestItr->addressSpace - 100;
	while (requestItr != currentRequest.end()) {
        //If you run into the last request for the process just exit out
        if ((requestItr->addressSpace == processNum) && (requestItr->offset == -1)) {
            break;
        }
        //Check to see if the next request belongs to the process
		else if ((requestItr->addressSpace == processNum) && (requestItr->offset != -1)) {
			if (processProperties[addressSpace].currentFrameCount < framesPerProcess) {
				while(frameItr != currentFrames.end()) {
					if ((frameItr->addressSpace == requestItr->addressSpace) && (frameItr->segment == requestItr->segment) && (frameItr->page == requestItr->page)) {
                        //Frame already contains page needed. No need to use a new frame buffer.
                        break;
					}
                    else if (frameItr->addressSpace == 0) {
                        //Copy page to the frame buffer
                        cout << "Copying page into frame buffer." << endl;
                        frameItr->addressSpace = requestItr->addressSpace;
                        frameItr->segment = requestItr->segment;
                        frameItr->page = requestItr->page;
                        //Increase the count of frames that currently belong to the process
                        processProperties[addressSpace].currentFrameCount++;
                        //Break out of loop and move on to the next request
                        break;
                    }
					else {
                        //If frame occupied by another page either by the same process or another process
						frameItr++; //Advance to next frame.
					}
				}
			} else {
				while(frameItr != currentFrames.end()) {
					if (frameItr->addressSpace == requestItr->addressSpace) {
                        cout << "Replacing frame buffer." << endl;
						//Remove the first page that came in
                        frameItr = currentFrames.erase(frameItr);
                        //Update frames table to reflect the new order of pages in frame buffers
                        frame newFrame;
                        newFrame.addressSpace = requestItr->addressSpace;
                        newFrame.segment = requestItr->segment;
                        newFrame.page = requestItr->page;
                        currentFrames.push_back(newFrame);
                         //Break out of loop and move on to the next request
                        break;
					}
					else {
						frameItr++;
					}
				}
			}
		}
        //If the request does not belong to the process move to the next process
        else {
			requestItr++;
		}
	}
}

void lru()
{

}

void lifo()
{
}

void ldf()
{
}

void opt()
{
}

void workingSet()
{
}

#endif
