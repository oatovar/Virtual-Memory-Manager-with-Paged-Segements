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
	int addressSpace = requestItr->segment - 100;
	while (requestItr != currentRequest.end()) {
		if (requestItr->segment == frameItr->a) {
			if (processProperties[addressSpace].currentFrameCount < framesPerProcess) {
				while(frameItr != currentFrames.end()) {
					if (frameItr->a == 0) {
						frameItr->a = requestItr->segment
					}
					else {
						frameItr++; //Advance to next frame.
					}
				}
			} else {
				while(frameItr != currentFrames.end()) {
					if (frameItr->a == requestItr->segment) {
						//do replacement stuff
					}
					else {
						frameItr++;
					}
				}
			}
		} else {
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
